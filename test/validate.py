from __future__ import annotations

import re
from pathlib import Path
from typing import Any, Dict, List


def _check(
    checks: List[Dict[str, Any]],
    name: str,
    ok: bool,
    detail: str = "",
    level: str = "fail",
) -> None:
    checks.append({"name": name, "ok": bool(ok), "detail": detail, "level": level})


def _brief(items: List[Any], limit: int = 3) -> str:
    if not items:
        return ""
    return "; ".join(str(item) for item in items[:limit])


def validate_run(
    parsed: Dict[str, Any],
    expected_instructions: int = 1000,
    exit_code: int = 0,
    timed_out: bool = False,
) -> Dict[str, Any]:
    checks: List[Dict[str, Any]] = []

    _check(checks, "run_exit", exit_code == 0 and not timed_out, f"exit={exit_code} timeout={timed_out}")
    _check(
        checks,
        "no_errors",
        parsed.get("error_count", 0) == 0,
        _brief(parsed.get("errors", [])),
    )
    _check(
        checks,
        "both_dirty",
        parsed.get("both_dirty", 0) == 0,
        f"count={parsed.get('both_dirty', 0)}",
        level="warn",
    )

    results = parsed.get("results", {})
    for core in (0, 1):
        result = results.get(core)
        _check(
            checks,
            f"result_core{core}",
            result is not None
            and result.get("instructions") == expected_instructions
            and result.get("cycles", 0) > 0,
            str(result),
        )

    total_instructions = sum(
        result.get("instructions", 0) for result in results.values()
    )
    _check(
        checks,
        "instructions_total",
        total_instructions == 2 * expected_instructions,
        f"total={total_instructions} expected={2 * expected_instructions}",
    )
    _check(
        checks,
        "both_done",
        parsed.get("done_cycle") is not None,
        f"done_cycle={parsed.get('done_cycle')}",
    )
    _check(
        checks,
        "completed_before_max",
        parsed.get("done_cycle") is not None
        and parsed.get("max_time") is not None
        and parsed["done_cycle"] <= parsed["max_time"],
        f"done_cycle={parsed.get('done_cycle')} max_time={parsed.get('max_time')}",
    )

    ops = parsed.get("ops", {})
    ifetch = ops.get("ifetch", {})
    atomic = ops.get("atomic", {})
    load = ops.get("load", {})
    store = ops.get("store", {})

    data_ops = (
        load.get("total", 0) + store.get("total", 0) + atomic.get("total", 0)
    )
    _check(
        checks,
        "data_ops_bound",
        data_ops <= 2 * expected_instructions,
        f"data_ops={data_ops} instructions={2 * expected_instructions}",
    )

    _check(
        checks,
        "ifetch_count",
        ifetch.get("total", 0) == 2 * expected_instructions
        and ifetch.get("by_core", {}).get(0, 0) == expected_instructions
        and ifetch.get("by_core", {}).get(1, 0) == expected_instructions,
        f"total={ifetch.get('total')} by_core={ifetch.get('by_core')}",
    )
    _check(
        checks,
        "ifetch_ok",
        ifetch.get("ok", 0) == ifetch.get("total", 0)
        and parsed.get("ifetch", {}).get("ok_without_req", 0) == 0,
        f"ok={ifetch.get('ok')} total={ifetch.get('total')} "
        f"orphan={parsed.get('ifetch', {}).get('ok_without_req')}",
    )
    ifetch_mismatches = parsed.get("ifetch", {}).get("mismatches", [])
    _check(
        checks,
        "ifetch_data",
        not ifetch_mismatches,
        _brief(ifetch_mismatches),
    )

    _check(
        checks,
        "atomic_ok",
        atomic.get("ok", 0) == atomic.get("total", 0)
        and parsed.get("atomic", {}).get("ok_without_req", 0) == 0,
        f"ok={atomic.get('ok')} total={atomic.get('total')} "
        f"orphan={parsed.get('atomic', {}).get('ok_without_req')}",
    )
    _check(
        checks,
        "atomic_mem",
        parsed.get("atomic", {}).get("mem_records", 0) == atomic.get("total", 0)
        and parsed.get("atomic", {}).get("missing_mem", 0) == 0,
        f"mem={parsed.get('atomic', {}).get('mem_records')} "
        f"req={atomic.get('total')} missing={parsed.get('atomic', {}).get('missing_mem')}",
    )
    atomic_mismatches = parsed.get("atomic", {}).get("mismatches", [])
    _check(
        checks,
        "atomic_data",
        not atomic_mismatches,
        _brief(atomic_mismatches),
    )

    bus = parsed.get("bus", {})
    bus_atomic = bus.get("counts", {}).get("atomic", 0)
    _check(
        checks,
        "atomic_bus",
        bus_atomic == atomic.get("total", 0)
        and bus.get("atomic_events", 0) == atomic.get("total", 0),
        f"grants={bus_atomic} events={bus.get('atomic_events')} req={atomic.get('total')}",
    )

    _check(
        checks,
        "load_ok",
        load.get("ok", 0) == load.get("total", 0),
        f"ok={load.get('ok')} total={load.get('total')}",
    )
    _check(
        checks,
        "store_ok",
        store.get("ok", 0) == store.get("total", 0),
        f"ok={store.get('ok')} total={store.get('total')}",
    )

    icache = parsed.get("cache", {}).get("icache", {})
    dcache = parsed.get("cache", {}).get("dcache", {})
    for core in (0, 1):
        icache_core = icache.get(core, {})
        _check(
            checks,
            f"icache_core{core}",
            icache_core.get("requests", 0) == expected_instructions
            and icache_core.get("hits", 0) + icache_core.get("misses", 0)
            == icache_core.get("requests", 0),
            f"req={icache_core.get('requests')} hits={icache_core.get('hits')} "
            f"misses={icache_core.get('misses')}",
        )
        dcache_core = dcache.get(core, {})
        _check(
            checks,
            f"dcache_core{core}",
            dcache_core.get("hits", 0) + dcache_core.get("misses", 0)
            == dcache_core.get("requests", 0),
            f"req={dcache_core.get('requests')} hits={dcache_core.get('hits')} "
            f"misses={dcache_core.get('misses')}",
        )

    wb = parsed.get("wb", {})
    wb_start = wb.get("start", 0)
    wb_done = wb.get("done", 0)
    wb_cancelled = wb.get("cancelled", 0)
    _check(
        checks,
        "wb_accounting",
        wb_done + wb_cancelled == wb_start,
        f"start={wb_start} done={wb_done} cancelled={wb_cancelled}",
        level="warn",
    )

    failed = [check for check in checks if not check["ok"] and check["level"] == "fail"]
    warned = [check for check in checks if not check["ok"] and check["level"] == "warn"]
    status = "FAIL" if failed else "WARN" if warned else "PASS"

    return {
        "status": status,
        "checks": checks,
        "failed": failed,
        "warnings": warned,
    }


def static_model_checks(model_path: str | Path) -> List[Dict[str, Any]]:
    text = Path(model_path).read_text(errors="replace")
    checks: List[Dict[str, Any]] = []

    forbidden = (
        (r"icache[01]_wb", "no icache write-back engine instances"),
    )
    required = (
        (r"dcache0\s*->\s*handle_snoop", "dcache0 snoop call present"),
        (r"dcache1\s*->\s*handle_snoop", "dcache1 snoop call present"),
        (r"icache0\s*->\s*handle_snoop", "icache0 snoop call present"),
        (r"icache1\s*->\s*handle_snoop", "icache1 snoop call present"),
        (r"bus\.icache0\s*=", "bus icache0 snoop pointer present"),
        (r"bus\.icache1\s*=", "bus icache1 snoop pointer present"),
    )

    for pattern, name in forbidden:
        found = re.search(pattern, text)
        _check(checks, name, found is None, f"found={found.group(0) if found else None}")

    for pattern, name in required:
        found = re.search(pattern, text)
        _check(checks, name, found is not None, f"found={found.group(0) if found else None}")

    return checks
