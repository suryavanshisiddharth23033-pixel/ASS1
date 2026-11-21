from __future__ import annotations

from pathlib import Path
from typing import Any, Dict, List


STATUS_COLORS = {
    "PASS": "#4c78a8",
    "WARN": "#f2a900",
    "FAIL": "#d1495b",
}


def _plot_setup():
    import matplotlib

    matplotlib.use("Agg")
    import matplotlib.pyplot as plt

    plt.rcParams.update(
        {
            "font.size": 9,
            "axes.titlesize": 11,
            "axes.labelsize": 9,
            "axes.grid": True,
            "grid.alpha": 0.25,
            "figure.facecolor": "white",
            "axes.facecolor": "white",
        }
    )
    return plt


def _save(plt, fig, path: Path) -> str:
    fig.tight_layout()
    fig.savefig(path, dpi=150, bbox_inches="tight")
    plt.close(fig)
    return str(path)


def _labels(ax, ids: List[str]) -> None:
    ax.set_xticks(range(len(ids)))
    ax.set_xticklabels(ids, rotation=45, ha="right")


def _status_bar(plt, ids: List[str], values: List[float], title: str, ylabel: str, path: Path, statuses: List[str]) -> str:
    fig, ax = plt.subplots(figsize=(13, 5.2))
    colors = [STATUS_COLORS.get(status, "#7f7f7f") for status in statuses]
    ax.bar(range(len(ids)), values, color=colors)
    ax.set_title(title)
    ax.set_ylabel(ylabel)
    _labels(ax, ids)
    fig.tight_layout()
    fig.savefig(path, dpi=150, bbox_inches="tight")
    plt.close(fig)
    return str(path)


def _stacked_bar(plt, ids: List[str], series: List[tuple[str, List[float], str]], title: str, ylabel: str, path: Path) -> str:
    import numpy as np

    fig, ax = plt.subplots(figsize=(13, 5.2))
    x = np.arange(len(ids))
    bottom = np.zeros(len(ids), dtype=float)
    for name, values, color in series:
        values_array = np.asarray(values, dtype=float)
        ax.bar(x, values_array, bottom=bottom, label=name, color=color)
        bottom += values_array
    ax.set_title(title)
    ax.set_ylabel(ylabel)
    ax.legend(ncol=len(series), loc="upper left", fontsize=8)
    _labels(ax, ids)
    fig.tight_layout()
    fig.savefig(path, dpi=150, bbox_inches="tight")
    plt.close(fig)
    return str(path)


def plot_all(metrics: List[Dict[str, Any]], out_dir: Path) -> List[str]:
    if not metrics:
        return []

    plt = _plot_setup()
    out_dir.mkdir(parents=True, exist_ok=True)
    ids = [item["scenario"] for item in metrics]
    statuses = [item.get("status", "FAIL") for item in metrics]
    paths: List[str] = []

    paths.append(
        _status_bar(
            plt,
            ids,
            [item.get("cycles", 0) for item in metrics],
            "Simulation Cycles",
            "cycles",
            out_dir / "cycles.png",
            statuses,
        )
    )
    paths.append(
        _status_bar(
            plt,
            ids,
            [item.get("ipc", 0) for item in metrics],
            "Aggregate IPC",
            "instructions / cycle",
            out_dir / "ipc.png",
            statuses,
        )
    )

    import numpy as np

    fig, ax = plt.subplots(figsize=(14, 5.6))
    width = 0.2
    x = np.arange(len(ids))
    series = (
        ("icache0", [item.get("icache0_hit_rate", 0) for item in metrics], "#4c78a8"),
        ("icache1", [item.get("icache1_hit_rate", 0) for item in metrics], "#72b7b2"),
        ("dcache0", [item.get("dcache0_hit_rate", 0) for item in metrics], "#f58518"),
        ("dcache1", [item.get("dcache1_hit_rate", 0) for item in metrics], "#e45756"),
    )
    for index, (name, values, color) in enumerate(series):
        ax.bar(x + (index - 1.5) * width, values, width, label=name, color=color)
    ax.set_ylim(0, 1.05)
    ax.set_ylabel("hit rate")
    ax.set_title("Cache Hit Rates")
    ax.legend(ncol=4, loc="upper left", fontsize=8)
    _labels(ax, ids)
    fig.tight_layout()
    fig.savefig(out_dir / "hit_rates.png", dpi=150, bbox_inches="tight")
    plt.close(fig)
    paths.append(str(out_dir / "hit_rates.png"))

    paths.append(
        _stacked_bar(
            plt,
            ids,
            [
                ("read", [item.get("bus_rd", 0) for item in metrics], "#4c78a8"),
                ("readx", [item.get("bus_rdx", 0) for item in metrics], "#f58518"),
                ("upgrade", [item.get("bus_upgr", 0) for item in metrics], "#72b7b2"),
                ("writeback", [item.get("bus_wb", 0) for item in metrics], "#b279a2"),
                ("atomic", [item.get("bus_atomic", 0) for item in metrics], "#e45756"),
            ],
            "Bus Command Mix",
            "grants",
            out_dir / "bus_mix.png",
        )
    )

    paths.append(
        _stacked_bar(
            plt,
            ids,
            [
                ("invalidations", [item.get("invalidations", 0) for item in metrics], "#4c78a8"),
                ("dirty interventions", [item.get("dirty_interventions", 0) for item in metrics], "#f58518"),
                ("downgrades", [item.get("downgrades", 0) for item in metrics], "#72b7b2"),
            ],
            "Coherence Events",
            "events",
            out_dir / "coherence.png",
        )
    )

    fig, ax = plt.subplots(figsize=(13, 5.2))
    x = np.arange(len(ids))
    ax.bar(x - 0.2, [item.get("wb_avg", 0) for item in metrics], 0.4, label="average", color="#4c78a8")
    ax.bar(x + 0.2, [item.get("wb_max", 0) for item in metrics], 0.4, label="maximum", color="#e45756")
    ax.set_title("Writeback Wait Latency")
    ax.set_ylabel("cycles")
    ax.legend(fontsize=8)
    _labels(ax, ids)
    fig.tight_layout()
    fig.savefig(out_dir / "wb_latency.png", dpi=150, bbox_inches="tight")
    plt.close(fig)
    paths.append(str(out_dir / "wb_latency.png"))

    paths.append(
        _stacked_bar(
            plt,
            ids,
            [
                ("conflict", [item.get("conflict_stall_cycles", 0) for item in metrics], "#4c78a8"),
                ("buffer", [item.get("buffer_stall_cycles", 0) for item in metrics], "#f58518"),
                ("memory wait", [item.get("mem_wait_cycles", 0) for item in metrics], "#72b7b2"),
                ("bus wait", [item.get("bus_wait_cycles", 0) for item in metrics], "#b279a2"),
            ],
            "DCache Stall Cycles",
            "cycles",
            out_dir / "stalls.png",
        )
    )

    paths.append(
        _status_bar(
            plt,
            ids,
            [item.get("atomic_share_pct", 0) for item in metrics],
            "Atomic Instruction Share",
            "percent of instructions",
            out_dir / "atomic_share.png",
            statuses,
        )
    )

    return paths
