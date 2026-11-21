from __future__ import annotations

import argparse
import csv
import json
import os
import subprocess
import sys
from datetime import datetime
from pathlib import Path
from typing import Any, Dict, List, Tuple

HERE = Path(__file__).resolve().parent
ROOT = HERE.parent
sys.path.insert(0, str(HERE))

from metrics import CSV_FIELDS, build_metrics, csv_row
from parse_log import parse_log
from scenarios import SCENARIOS, Scenario, env_text, get_scenario
from validate import static_model_checks, validate_run


DEFAULT_SITAR = Path("/home/sidd64/Projects/sitar/scripts/sitar")
EXPECTED_INSTRUCTIONS = 1000


def _resolve(path: Path) -> Path:
    path = path.expanduser()
    if path.is_absolute():
        return path
    return (Path.cwd() / path).resolve()


def _run_id(reports_dir: Path) -> str:
    base = datetime.now().strftime("run_%Y%m%d_%H%M%S")
    candidate = base
    suffix = 1
    while (reports_dir / candidate).exists():
        candidate = f"{base}_{suffix:02d}"
        suffix += 1
    return candidate


def _sha256(path: Path) -> str | None:
    if not path.exists():
        return None
    import hashlib

    digest = hashlib.sha256()
    with path.open("rb") as handle:
        for chunk in iter(lambda: handle.read(1024 * 1024), b""):
            digest.update(chunk)
    return digest.hexdigest()


def _git_commit(root: Path) -> str | None:
    try:
        result = subprocess.run(
            ["git", "rev-parse", "--short", "HEAD"],
            cwd=str(root),
            capture_output=True,
            text=True,
            check=False,
        )
    except OSError:
        return None
    return result.stdout.strip() or None


def _module_version(name: str) -> str | None:
    try:
        module = __import__(name)
    except Exception:
        return None
    return getattr(module, "__version__", None)


def _run_logged(
    command: List[str],
    cwd: Path,
    log_path: Path,
    timeout: float,
    append: bool = False,
    env: Dict[str, str] | None = None,
) -> Tuple[int, bool]:
    mode = "a" if append else "w"
    try:
        with log_path.open(mode, encoding="utf-8", errors="replace") as handle:
            handle.write("$ " + " ".join(command) + "\n")
            handle.flush()
            process = subprocess.run(
                command,
                cwd=str(cwd),
                env=env,
                stdout=handle,
                stderr=subprocess.STDOUT,
                timeout=timeout,
                text=True,
                check=False,
            )
        return process.returncode, False
    except subprocess.TimeoutExpired:
        with log_path.open("a", encoding="utf-8") as handle:
            handle.write(f"\nTIMEOUT after {timeout} seconds\n")
        return 124, True
    except OSError as exc:
        with log_path.open("a", encoding="utf-8") as handle:
            handle.write(f"\nLAUNCH ERROR: {exc}\n")
        return 127, False


def _build(args: argparse.Namespace, run_dir: Path) -> bool:
    if not args.sitar.exists():
        print(f"error: sitar not found at {args.sitar}", file=sys.stderr)
        return False
    output_dir = ROOT / "Output"
    output_dir.mkdir(parents=True, exist_ok=True)
    build_log = run_dir / "build.log"
    commands = (
        [str(args.sitar), "translate", str(args.model), "-o", str(output_dir)],
        [str(args.sitar), "compile", "-d", str(output_dir), "-o", str(args.sim)],
    )
    for command in commands:
        code, timed_out = _run_logged(command, ROOT, build_log, args.build_timeout, append=True)
        if code != 0 or timed_out:
            print(f"error: build command failed ({code})", file=sys.stderr)
            return False
    return True


def _config(
    args: argparse.Namespace,
    run_dir: Path,
    selected: List[Scenario],
) -> Dict[str, Any]:
    return {
        "run_id": run_dir.name,
        "timestamp": datetime.now().isoformat(timespec="seconds"),
        "model": str(args.model),
        "model_sha256": _sha256(args.model),
        "simulator": str(args.sim),
        "sim_sha256": _sha256(args.sim),
        "max_time": args.max_time,
        "expected_instructions": EXPECTED_INSTRUCTIONS,
        "build": args.build,
        "sitar": str(args.sitar),
        "git_commit": _git_commit(ROOT),
        "python": sys.version.split()[0],
        "matplotlib": _module_version("matplotlib"),
        "numpy": _module_version("numpy"),
        "scenarios": [
            {
                "id": scenario.scenario_id,
                "description": scenario.description,
                "env": scenario.resolved_env(),
                "tags": list(scenario.tags),
            }
            for scenario in selected
        ],
    }


def _clean_parsed(parsed: Dict[str, Any]) -> Dict[str, Any]:
    cleaned = dict(parsed)
    cleaned.pop("ifetch", None)
    cleaned.pop("atomic", None)
    cleaned["ifetch"] = {
        "mismatches": parsed.get("ifetch", {}).get("mismatches", []),
        "ok_without_req": parsed.get("ifetch", {}).get("ok_without_req", 0),
    }
    cleaned["atomic"] = {
        "mismatches": parsed.get("atomic", {}).get("mismatches", []),
        "ok_without_req": parsed.get("atomic", {}).get("ok_without_req", 0),
        "missing_mem": parsed.get("atomic", {}).get("missing_mem", 0),
        "mem_records": parsed.get("atomic", {}).get("mem_records", 0),
    }
    return cleaned


def _write_json(path: Path, payload: Dict[str, Any]) -> None:
    path.write_text(json.dumps(payload, indent=2, sort_keys=True) + "\n")


def _write_csv(path: Path, rows: List[Dict[str, Any]]) -> None:
    with path.open("w", newline="", encoding="utf-8") as handle:
        writer = csv.DictWriter(handle, fieldnames=CSV_FIELDS)
        writer.writeheader()
        for row in rows:
            writer.writerow(csv_row(row))


def _overall_status(static_checks: List[Dict[str, Any]], results: List[Dict[str, Any]]) -> str:
    if any(not check["ok"] for check in static_checks):
        return "FAIL"
    statuses = [result["validation"]["status"] for result in results]
    if "FAIL" in statuses:
        return "FAIL"
    if "WARN" in statuses:
        return "WARN"
    return "PASS"


def _fmt(value: Any) -> str:
    if isinstance(value, float):
        if value >= 1000:
            return f"{value:,.0f}"
        return f"{value:.3f}"
    if isinstance(value, int):
        return f"{value:,}"
    return str(value)


def _summary(
    path: Path,
    config: Dict[str, Any],
    static_checks: List[Dict[str, Any]],
    results: List[Dict[str, Any]],
    plot_paths: List[str],
    overall: str,
) -> None:
    lines: List[str] = []
    lines.append(f"# Sitar Test Run {config['run_id']}")
    lines.append("")
    lines.append(f"- Overall status: **{overall}**")
    lines.append(f"- Timestamp: {config['timestamp']}")
    lines.append(f"- Model: `{config['model']}`")
    lines.append(f"- Simulator: `{config['simulator']}`")
    lines.append(f"- Git commit: `{config['git_commit']}`")
    lines.append(f"- Max simulation time: {config['max_time']}")
    lines.append(f"- Expected instructions per core: {config['expected_instructions']}")
    lines.append("")
    lines.append("## Results")
    lines.append("")
    lines.append(
        "| Scenario | Status | Cycles | IPC | IC hit | DC hit | Atomics | Coherence/1k | Errors | Warnings |"
    )
    lines.append("| --- | --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: |")
    for result in results:
        metrics = result["metrics"]
        lines.append(
            "| {scenario} | {status} | {cycles} | {ipc} | {ic} | {dc} | {atomic} | {coh} | {errors} | {warnings} |".format(
                scenario=metrics["scenario"],
                status=metrics["status"],
                cycles=_fmt(metrics["cycles"]),
                ipc=_fmt(metrics["ipc"]),
                ic=_fmt(metrics["icache_hit_rate"]),
                dc=_fmt(metrics["dcache_hit_rate"]),
                atomic=_fmt(metrics["ops_atomic"]),
                coh=_fmt(metrics["coherence_events_per_1k"]),
                errors=_fmt(metrics["errors"]),
                warnings=_fmt(metrics["warning_checks"]),
            )
        )
    lines.append("")
    lines.append("## Static Model Checks")
    lines.append("")
    lines.append("| Check | Result | Detail |")
    lines.append("| --- | --- | --- |")
    for check in static_checks:
        lines.append(
            "| {name} | {result} | {detail} |".format(
                name=check["name"],
                result="PASS" if check["ok"] else "FAIL",
                detail=check.get("detail", ""),
            )
        )
    lines.append("")
    failures = [
        (result, check)
        for result in results
        for check in result["validation"].get("failed", [])
    ]
    warnings = [
        (result, check)
        for result in results
        for check in result["validation"].get("warnings", [])
    ]
    lines.append("## Failures")
    lines.append("")
    if failures:
        for result, check in failures:
            lines.append(
                f"- `{result['scenario']['scenario_id']}`: `{check['name']}` {check.get('detail', '')}"
            )
    else:
        lines.append("None.")
    lines.append("")
    lines.append("## Warnings")
    lines.append("")
    if warnings:
        for result, check in warnings:
            lines.append(
                f"- `{result['scenario']['scenario_id']}`: `{check['name']}` {check.get('detail', '')}"
            )
    else:
        lines.append("None.")
    lines.append("")
    lines.append("## Plots")
    lines.append("")
    if plot_paths:
        for plot_path in plot_paths:
            relative = Path(plot_path).relative_to(path.parent)
            lines.append(f"- [{relative}]({relative})")
    else:
        lines.append("Plots disabled or unavailable.")
    lines.append("")
    lines.append("## Scenario Configuration")
    lines.append("")
    lines.append("| Scenario | Tags | Environment |")
    lines.append("| --- | --- | --- |")
    for scenario in config["scenarios"]:
        lines.append(
            "| {id} | {tags} | `{env}` |".format(
                id=scenario["id"],
                tags=", ".join(scenario["tags"]),
                env=env_text(scenario["env"]),
            )
        )
    lines.append("")
    lines.append("## Reproduction")
    lines.append("")
    lines.append("```bash")
    lines.append(
        f"python3 test/run_tests.py --reports reports --max-time {config['max_time']}"
    )
    lines.append("```")
    lines.append("")
    lines.append(f"Raw logs: `{config['run_id']}/raw/`  ")
    lines.append(f"Machine-readable results: `{config['run_id']}/results.json`, `{config['run_id']}/results.csv`")
    lines.append("")
    path.write_text("\n".join(lines))


def _update_latest(reports_dir: Path, run_dir: Path) -> None:
    latest = reports_dir / "latest"
    try:
        if latest.is_symlink() or latest.is_file():
            latest.unlink()
        elif latest.exists():
            print(f"warning: {latest} exists and is not a symlink", file=sys.stderr)
            return
        latest.symlink_to(run_dir.name, target_is_directory=True)
    except OSError as exc:
        print(f"warning: could not update latest symlink: {exc}", file=sys.stderr)


def _parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="Run the Sitar cache-coherence test matrix")
    parser.add_argument("--model", type=Path, default=ROOT / "model.sitar")
    parser.add_argument("--sim", type=Path, default=ROOT / "sitar_sim")
    parser.add_argument("--sitar", type=Path, default=DEFAULT_SITAR)
    parser.add_argument("--reports", type=Path, default=ROOT / "reports")
    parser.add_argument("--max-time", type=int, default=100000)
    parser.add_argument("--timeout", type=float, default=240.0)
    parser.add_argument("--build-timeout", type=float, default=600.0)
    parser.add_argument("--only", default="")
    parser.add_argument("--list", action="store_true")
    parser.add_argument("--build", action="store_true")
    parser.add_argument("--no-plots", action="store_true")
    return parser.parse_args()


def main() -> int:
    args = _parse_args()
    args.model = _resolve(args.model)
    args.sim = _resolve(args.sim)
    args.sitar = _resolve(args.sitar)
    args.reports = _resolve(args.reports)

    if args.list:
        for scenario in SCENARIOS:
            print(f"{scenario.scenario_id}: {scenario.description}")
            print(f"  {env_text(scenario.resolved_env())}")
        return 0

    selected: List[Scenario]
    if args.only:
        requested = [item.strip() for item in args.only.split(",") if item.strip()]
        try:
            selected = [get_scenario(item) for item in requested]
        except KeyError as exc:
            print(f"error: unknown scenario {exc}", file=sys.stderr)
            return 2
    else:
        selected = list(SCENARIOS)
    if not selected:
        print("error: no scenarios selected", file=sys.stderr)
        return 2

    if not args.model.exists():
        print(f"error: model not found at {args.model}", file=sys.stderr)
        return 2

    args.reports.mkdir(parents=True, exist_ok=True)
    run_dir = args.reports / _run_id(args.reports)
    (run_dir / "raw").mkdir(parents=True, exist_ok=False)
    (run_dir / "plots").mkdir(parents=True, exist_ok=False)

    if args.build:
        if not _build(args, run_dir):
            config = _config(args, run_dir, selected)
            _write_json(run_dir / "config.json", config)
            print(f"build failed; see {run_dir / 'build.log'}", file=sys.stderr)
            return 2

    if not args.sim.exists():
        print(f"error: simulator not found at {args.sim}", file=sys.stderr)
        return 2

    config = _config(args, run_dir, selected)
    _write_json(run_dir / "config.json", config)

    results: List[Dict[str, Any]] = []
    for index, scenario in enumerate(selected, start=1):
        log_path = run_dir / "raw" / f"{scenario.scenario_id}.log"
        env = os.environ.copy()
        env.update(scenario.resolved_env())
        command = [str(args.sim), str(args.max_time)]
        code, timed_out = _run_logged(command, ROOT, log_path, args.timeout, env=env)
        try:
            parsed = parse_log(log_path)
        except Exception as exc:
            parsed = {
                "path": str(log_path),
                "error_count": 1,
                "errors": [f"parse failure: {exc}"],
                "results": {},
                "cache": {"icache": {}, "dcache": {}},
                "bus": {"counts": {}, "total": 0},
                "ops": {},
                "wb": {},
                "both_dirty": 0,
            }
        validation = validate_run(
            parsed,
            expected_instructions=EXPECTED_INSTRUCTIONS,
            exit_code=code,
            timed_out=timed_out,
        )
        metrics = build_metrics(parsed, scenario, validation)
        results.append(
            {
                "scenario": {
                    "scenario_id": scenario.scenario_id,
                    "description": scenario.description,
                    "tags": list(scenario.tags),
                    "env": scenario.resolved_env(),
                },
                "exit_code": code,
                "timed_out": timed_out,
                "log": str(log_path),
                "parsed": parsed,
                "validation": validation,
                "metrics": metrics,
            }
        )
        print(
            f"[{index}/{len(selected)}] {scenario.scenario_id}: {validation['status']} "
            f"cycles={metrics['cycles']} ipc={metrics['ipc']:.3f}",
            flush=True,
        )

    static_checks = static_model_checks(args.model)
    overall = _overall_status(static_checks, results)

    plot_paths: List[str] = []
    if not args.no_plots:
        try:
            from plot_results import plot_all

            plot_paths = plot_all(
                [result["metrics"] for result in results],
                run_dir / "plots",
            )
        except Exception as exc:
            print(f"warning: plotting failed: {exc}", file=sys.stderr)

    rows = [result["metrics"] for result in results]
    _write_csv(run_dir / "results.csv", rows)
    _write_json(
        run_dir / "results.json",
        {
            "config": config,
            "static_checks": static_checks,
            "overall_status": overall,
            "runs": [
                {
                    "scenario": result["scenario"],
                    "exit_code": result["exit_code"],
                    "timed_out": result["timed_out"],
                    "log": result["log"],
                    "validation": result["validation"],
                    "metrics": result["metrics"],
                    "parsed": _clean_parsed(result["parsed"]),
                }
                for result in results
            ],
        },
    )
    _summary(
        run_dir / "summary.md",
        config,
        static_checks,
        results,
        plot_paths,
        overall,
    )
    _update_latest(args.reports, run_dir)

    print(f"overall: {overall}")
    print(f"report: {run_dir / 'summary.md'}")
    return 0 if overall != "FAIL" else 1


if __name__ == "__main__":
    raise SystemExit(main())
