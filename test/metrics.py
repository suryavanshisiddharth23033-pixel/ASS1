from __future__ import annotations

from typing import Any, Dict


CSV_FIELDS = (
    "scenario",
    "description",
    "status",
    "tags",
    "cycles",
    "cycles_core0",
    "cycles_core1",
    "instructions_core0",
    "instructions_core1",
    "instructions_total",
    "ipc",
    "ipc_core0",
    "ipc_core1",
    "ops_ifetch",
    "ops_load",
    "ops_store",
    "ops_atomic",
    "ops_alu",
    "icache_requests",
    "icache_hits",
    "icache_misses",
    "icache_hit_rate",
    "icache0_hit_rate",
    "icache1_hit_rate",
    "dcache_requests",
    "dcache_hits",
    "dcache_misses",
    "dcache_hit_rate",
    "dcache0_hit_rate",
    "dcache1_hit_rate",
    "bus_total",
    "bus_rd",
    "bus_rdx",
    "bus_upgr",
    "bus_wb",
    "bus_atomic",
    "bus_rd_pct",
    "bus_rdx_pct",
    "bus_upgr_pct",
    "bus_wb_pct",
    "bus_atomic_pct",
    "snoop_hits",
    "bus_dirty_interventions",
    "invalidations",
    "dirty_interventions",
    "downgrades",
    "coherence_events",
    "coherence_events_per_1k",
    "wb_starts",
    "wb_done",
    "wb_cancelled",
    "wb_avg",
    "wb_max",
    "conflict_stall_cycles",
    "buffer_stall_cycles",
    "mem_wait_cycles",
    "bus_wait_cycles",
    "wb_wait_total",
    "atomic_share_pct",
    "errors",
    "both_dirty",
    "failed_checks",
    "warning_checks",
)


def _rate(hits: int, requests: int) -> float:
    if requests <= 0:
        return 0.0
    return hits / requests


def _pct(value: int, total: int) -> float:
    if total <= 0:
        return 0.0
    return 100.0 * value / total


def _safe_div(numerator: float, denominator: float) -> float:
    if denominator <= 0:
        return 0.0
    return numerator / denominator


def _round(value: float) -> float:
    return round(float(value), 6)


def build_metrics(parsed: Dict[str, Any], scenario: Any, validation: Dict[str, Any]) -> Dict[str, Any]:
    results = parsed.get("results", {})
    cycles_core0 = results.get(0, {}).get("cycles", 0)
    cycles_core1 = results.get(1, {}).get("cycles", 0)
    instructions_core0 = results.get(0, {}).get("instructions", 0)
    instructions_core1 = results.get(1, {}).get("instructions", 0)
    instructions_total = instructions_core0 + instructions_core1
    cycles = parsed.get("stop_cycle") or max(cycles_core0, cycles_core1, 0)

    ops = parsed.get("ops", {})
    ifetch = ops.get("ifetch", {})
    load = ops.get("load", {})
    store = ops.get("store", {})
    atomic = ops.get("atomic", {})

    icache = parsed.get("cache", {}).get("icache", {})
    dcache = parsed.get("cache", {}).get("dcache", {})
    icache_requests = sum(icache.get(core, {}).get("requests", 0) for core in (0, 1))
    icache_hits = sum(icache.get(core, {}).get("hits", 0) for core in (0, 1))
    icache_misses = sum(icache.get(core, {}).get("misses", 0) for core in (0, 1))
    dcache_requests = sum(dcache.get(core, {}).get("requests", 0) for core in (0, 1))
    dcache_hits = sum(dcache.get(core, {}).get("hits", 0) for core in (0, 1))
    dcache_misses = sum(dcache.get(core, {}).get("misses", 0) for core in (0, 1))

    bus = parsed.get("bus", {})
    counts = bus.get("counts", {})
    bus_total = bus.get("total", 0)
    bus_rd = counts.get("rd", 0)
    bus_rdx = counts.get("rdx", 0)
    bus_upgr = counts.get("upgr", 0)
    bus_wb = counts.get("wb", 0)
    bus_atomic = counts.get("atomic", 0)

    invalidations = sum(dcache.get(core, {}).get("invalidations_rx", 0) for core in (0, 1))
    dirty_interventions = sum(
        dcache.get(core, {}).get("dirty_interventions", 0) for core in (0, 1)
    )
    downgrades = sum(dcache.get(core, {}).get("downgrades", 0) for core in (0, 1))
    coherence_events = invalidations + dirty_interventions + downgrades

    wb_starts = parsed.get("wb", {}).get("start", 0)
    wb_done = parsed.get("wb", {}).get("done", 0)
    wb_cancelled = parsed.get("wb", {}).get("cancelled", 0)
    wb_wait_total = sum(dcache.get(core, {}).get("wb_wait_total", 0) for core in (0, 1))
    writebacks_done = sum(
        dcache.get(core, {}).get("writebacks_done", 0) for core in (0, 1)
    )
    wb_avg = _safe_div(wb_wait_total, writebacks_done)
    wb_max = max((dcache.get(core, {}).get("wb_wait_max", 0) for core in (0, 1)), default=0)

    conflict_stall_cycles = sum(
        dcache.get(core, {}).get("conflict_stall_cycles", 0)
        for core in (0, 1)
    )
    buffer_stall_cycles = sum(
        dcache.get(core, {}).get("buffer_stall_cycles", 0) for core in (0, 1)
    )
    mem_wait_cycles = sum(
        dcache.get(core, {}).get("mem_wait_cycles", 0) for core in (0, 1)
    )
    bus_wait_cycles = sum(
        dcache.get(core, {}).get("bus_wait_cycles", 0) for core in (0, 1)
    )

    status = validation.get("status", "FAIL")
    failed_checks = validation.get("failed", [])
    warning_checks = validation.get("warnings", [])

    metrics: Dict[str, Any] = {
        "scenario": scenario.scenario_id,
        "description": scenario.description,
        "status": status,
        "tags": ";".join(scenario.tags),
        "env": scenario.resolved_env(),
        "cycles": cycles,
        "cycles_core0": cycles_core0,
        "cycles_core1": cycles_core1,
        "instructions_core0": instructions_core0,
        "instructions_core1": instructions_core1,
        "instructions_total": instructions_total,
        "ipc": _round(_safe_div(instructions_total, cycles)),
        "ipc_core0": _round(_safe_div(instructions_core0, cycles_core0)),
        "ipc_core1": _round(_safe_div(instructions_core1, cycles_core1)),
        "ops_ifetch": ifetch.get("total", 0),
        "ops_load": load.get("total", 0),
        "ops_store": store.get("total", 0),
        "ops_atomic": atomic.get("total", 0),
        "ops_alu": max(
            0,
            instructions_total
            - load.get("total", 0)
            - store.get("total", 0)
            - atomic.get("total", 0),
        ),
        "icache_requests": icache_requests,
        "icache_hits": icache_hits,
        "icache_misses": icache_misses,
        "icache_hit_rate": _round(_rate(icache_hits, icache_requests)),
        "icache0_hit_rate": _round(
            _rate(
                icache.get(0, {}).get("hits", 0),
                icache.get(0, {}).get("requests", 0),
            )
        ),
        "icache1_hit_rate": _round(
            _rate(
                icache.get(1, {}).get("hits", 0),
                icache.get(1, {}).get("requests", 0),
            )
        ),
        "dcache_requests": dcache_requests,
        "dcache_hits": dcache_hits,
        "dcache_misses": dcache_misses,
        "dcache_hit_rate": _round(_rate(dcache_hits, dcache_requests)),
        "dcache0_hit_rate": _round(
            _rate(
                dcache.get(0, {}).get("hits", 0),
                dcache.get(0, {}).get("requests", 0),
            )
        ),
        "dcache1_hit_rate": _round(
            _rate(
                dcache.get(1, {}).get("hits", 0),
                dcache.get(1, {}).get("requests", 0),
            )
        ),
        "bus_total": bus_total,
        "bus_rd": bus_rd,
        "bus_rdx": bus_rdx,
        "bus_upgr": bus_upgr,
        "bus_wb": bus_wb,
        "bus_atomic": bus_atomic,
        "bus_rd_pct": _round(_pct(bus_rd, bus_total)),
        "bus_rdx_pct": _round(_pct(bus_rdx, bus_total)),
        "bus_upgr_pct": _round(_pct(bus_upgr, bus_total)),
        "bus_wb_pct": _round(_pct(bus_wb, bus_total)),
        "bus_atomic_pct": _round(_pct(bus_atomic, bus_total)),
        "snoop_hits": bus.get("snoop_hits", 0),
        "bus_dirty_interventions": bus.get("dirty_interventions", 0),
        "invalidations": invalidations,
        "dirty_interventions": dirty_interventions,
        "downgrades": downgrades,
        "coherence_events": coherence_events,
        "coherence_events_per_1k": _round(
            _safe_div(coherence_events * 1000.0, instructions_total)
        ),
        "wb_starts": wb_starts,
        "wb_done": wb_done,
        "wb_cancelled": wb_cancelled,
        "wb_avg": _round(wb_avg),
        "wb_max": wb_max,
        "conflict_stall_cycles": conflict_stall_cycles,
        "buffer_stall_cycles": buffer_stall_cycles,
        "mem_wait_cycles": mem_wait_cycles,
        "bus_wait_cycles": bus_wait_cycles,
        "wb_wait_total": wb_wait_total,
        "atomic_share_pct": _round(
            _pct(atomic.get("total", 0), instructions_total)
        ),
        "errors": parsed.get("error_count", 0),
        "both_dirty": parsed.get("both_dirty", 0),
        "failed_checks": len(failed_checks),
        "warning_checks": len(warning_checks),
    }
    return metrics


def csv_row(metrics: Dict[str, Any]) -> Dict[str, Any]:
    return {field: metrics.get(field, "") for field in CSV_FIELDS}
