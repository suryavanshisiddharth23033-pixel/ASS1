from __future__ import annotations

import re
from pathlib import Path
from typing import Any, Dict, List


BUS_NAMES = {
    1: "rd",
    2: "rdx",
    3: "upgr",
    4: "wb",
    5: "atomic",
}

MAXTIME_RE = re.compile(r"Maximum simulation time = (?P<max>\d+) cycles")
STOP_RE = re.compile(r"Simulation stopped at time \((?P<cycle>\d+),(?P<phase>\d+)\)")
RESULT_RE = re.compile(
    r"\[C(?P<core>\d+)\] RESULT instructions=(?P<instructions>\d+) cycles=(?P<cycles>\d+)"
)
DONE_RE = re.compile(
    r"\[C(?P<core>\d+)\] Both cores done\. Stopping simulation at cycle (?P<cycle>\d+)"
)
CACHE_CONFIG_RE = re.compile(
    r"TOP\.DC\.(?P<kind>icache|dcache)(?P<core>\d+):\[C\d+\] [ID]CACHE STATISTICS "
    r"\(config: sets=(?P<sets>\d+) ways=(?P<ways>\d+) blockSize=(?P<block>\d+) "
    r"nonBlocking=(?P<nb>\d+) repl=(?P<repl>\d+) write=(?P<write>\d+)\)"
)
ICACHE_STATS_RE = re.compile(
    r"TOP\.DC\.icache(?P<core>\d+):requests=(?P<requests>\d+) hits=(?P<hits>\d+) "
    r"misses=(?P<misses>\d+) writebacks=(?P<writebacks>\d+) "
    r"writethroughs=(?P<writethroughs>\d+) writebacks_done=(?P<writebacks_done>\d+)"
)
ICACHE_STALL_RE = re.compile(
    r"TOP\.DC\.icache(?P<core>\d+):hits_under_wb=(?P<hits_under_wb>\d+) "
    r"conflict_stall_cycles=(?P<conflict>\d+) buffer_stall_cycles=(?P<buffer>\d+) "
    r"mem_wait_cycles=(?P<mem>\d+) bus_wait_cycles=(?P<bus>\d+) "
    r"wb_wait_total=(?P<wb_total>\d+) wb_wait_max=(?P<wb_max>\d+)"
)
DCACHE_STATS_RE = re.compile(
    r"TOP\.DC\.dcache(?P<core>\d+):requests=(?P<requests>\d+) hits=(?P<hits>\d+) "
    r"misses=(?P<misses>\d+) writebacks=(?P<writebacks>\d+) "
    r"writethroughs=(?P<writethroughs>\d+) writebacks_done=(?P<writebacks_done>\d+)"
)
DCACHE_STALL_RE = re.compile(
    r"TOP\.DC\.dcache(?P<core>\d+):hits_under_wb=(?P<hits_under_wb>\d+) "
    r"conflict_stall_cycles=(?P<conflict>\d+) buffer_stall_cycles=(?P<buffer>\d+) "
    r"mem_wait_cycles=(?P<mem>\d+) bus_wait_cycles=(?P<bus>\d+) "
    r"wb_wait_total=(?P<wb_total>\d+) wb_wait_max=(?P<wb_max>\d+)"
)
DCACHE_BUS_RE = re.compile(
    r"TOP\.DC\.dcache(?P<core>\d+):bus_rd=(?P<rd>\d+) bus_rdx=(?P<rdx>\d+) "
    r"bus_upgr=(?P<upgr>\d+) invalidations_rx=(?P<inv>\d+) "
    r"dirty_interventions=(?P<dirty>\d+) downgrades=(?P<down>\d+)"
)
FINAL_STATE_RE = re.compile(
    r"TOP\.DC\.dcache(?P<core>\d+):final_state: M=(?P<m>\d+) S=(?P<s>\d+) I=(?P<i>\d+)"
)
PHIT_RE = re.compile(r"TOP\.DC\.(?P<kind>icache|dcache)(?P<core>\d+):p_hit=(?P<p>[0-9.]+)")
BUS_GRANT_RE = re.compile(
    r"BUS GRANT (?:core=(?P<core>\d+)|wb=(?P<wb>\d+)) cmd=(?P<cmd>\d+) "
    r"addr=(?P<addr>\d+) snoopHit=(?P<snoop>\d+) "
    r"dirtyIntervention=(?P<dirty>\d+) cycle=(?P<cycle>\d+)"
)
BUS_ATOMIC_RE = re.compile(
    r"BUS ATOMIC core=(?P<core>\d+) word=(?P<word>\d+) block=(?P<block>\d+) "
    r"reqDirty=(?P<req_dirty>\d+) peerDirty=(?P<peer_dirty>\d+)"
)
MEM_ATOMIC_RE = re.compile(
    r"MEM ATOMIC addr=(?P<addr>\d+) size=(?P<size>\d+) old0=(?P<old0>\d+)"
)
ATOMIC_REQ_RE = re.compile(
    r"\[C(?P<core>\d+)\] ATOMIC addr=(?P<addr>\d+) size=(?P<size>\d+)"
)
ATOMIC_OK_RE = re.compile(r"\[C(?P<core>\d+)\] ATOMIC OK data=(?P<data>[0-9 ]*)")
IFETCH_REQ_RE = re.compile(r"\[C(?P<core>\d+)\] IFETCH addr=(?P<addr>\d+)")
IFETCH_OK_RE = re.compile(r"\[C(?P<core>\d+)\] IFETCH OK data=(?P<data>[0-9 ]*)")
LOAD_REQ_RE = re.compile(r"\[C(?P<core>\d+)\] LOAD addr=(?P<addr>\d+)")
LOAD_OK_RE = re.compile(r"\[C(?P<core>\d+)\] LOAD OK")
STORE_REQ_RE = re.compile(r"\[C(?P<core>\d+)\] STORE addr=(?P<addr>\d+)")
STORE_OK_RE = re.compile(r"\[C(?P<core>\d+)\] STORE OK")
WB_START_RE = re.compile(
    r"TOP\.DC\.dcache(?P<core>\d+)_wb:BUS WB START addr=(?P<addr>\d+) "
    r"size=(?P<size>\d+) cycle=(?P<cycle>\d+)"
)
WB_DONE_RE = re.compile(
    r"TOP\.DC\.bus:BUS WB DONE wb=(?P<wb>\d+) addr=(?P<addr>\d+) cycle=(?P<cycle>\d+)"
)
WB_CANCEL_RE = re.compile(r"BUS WB CANCELLED BY SNOOP")
BOTH_DIRTY_RE = re.compile(r"BUS ATOMIC BOTH-DIRTY")
ERROR_RE = re.compile(r"\bERROR\b|INVARIANT VIOLATION|OUT OF BOUNDS|FATAL|ASSERT")


def _cache_stats() -> Dict[str, Any]:
    return {
        "config": {},
        "requests": 0,
        "hits": 0,
        "misses": 0,
        "writebacks": 0,
        "writethroughs": 0,
        "writebacks_done": 0,
        "hits_under_wb": 0,
        "conflict_stall_cycles": 0,
        "buffer_stall_cycles": 0,
        "mem_wait_cycles": 0,
        "bus_wait_cycles": 0,
        "wb_wait_total": 0,
        "wb_wait_max": 0,
        "bus_rd": 0,
        "bus_rdx": 0,
        "bus_upgr": 0,
        "invalidations_rx": 0,
        "dirty_interventions": 0,
        "downgrades": 0,
        "final_state": {},
        "p_hit": None,
    }


def _ops() -> Dict[str, Any]:
    return {
        "total": 0,
        "ok": 0,
        "by_core": {0: 0, 1: 0},
        "ok_by_core": {0: 0, 1: 0},
    }


def _ints(text: str) -> List[int]:
    return [int(value) for value in re.findall(r"\d+", text)]


def parse_log(path: str | Path) -> Dict[str, Any]:
    parsed: Dict[str, Any] = {
        "path": str(path),
        "max_time": None,
        "stop_cycle": None,
        "stop_phase": None,
        "done_cycle": None,
        "results": {},
        "cache": {
            "icache": {0: _cache_stats(), 1: _cache_stats()},
            "dcache": {0: _cache_stats(), 1: _cache_stats()},
        },
        "bus": {
            "counts": {name: 0 for name in BUS_NAMES.values()},
            "by_core": {
                0: {name: 0 for name in BUS_NAMES.values()},
                1: {name: 0 for name in BUS_NAMES.values()},
            },
            "total": 0,
            "wb_grants": 0,
            "snoop_hits": 0,
            "dirty_interventions": 0,
            "atomic_events": 0,
            "atomic_req_dirty": 0,
            "atomic_peer_dirty": 0,
        },
        "ops": {
            "ifetch": _ops(),
            "load": _ops(),
            "store": _ops(),
            "atomic": _ops(),
        },
        "wb": {"start": 0, "done": 0, "cancelled": 0, "start_by_core": {0: 0, 1: 0}},
        "ifetch": {
            "mismatches": [],
            "ok_without_req": 0,
            "pending_by_core": {0: [], 1: []},
        },
        "atomic": {
            "mismatches": [],
            "ok_without_req": 0,
            "missing_mem": 0,
            "mem_records": 0,
            "pending_by_core": {0: [], 1: []},
            "mem_pending": [],
        },
        "errors": [],
        "error_count": 0,
        "both_dirty": 0,
    }

    for raw_line in Path(path).read_text(errors="replace").splitlines():
        line = raw_line.strip()
        if not line:
            continue

        match = MAXTIME_RE.search(line)
        if match:
            parsed["max_time"] = int(match.group("max"))
            continue

        match = STOP_RE.search(line)
        if match:
            parsed["stop_cycle"] = int(match.group("cycle"))
            parsed["stop_phase"] = int(match.group("phase"))
            continue

        match = RESULT_RE.search(line)
        if match:
            core = int(match.group("core"))
            parsed["results"][core] = {
                "instructions": int(match.group("instructions")),
                "cycles": int(match.group("cycles")),
            }
            continue

        match = DONE_RE.search(line)
        if match:
            parsed["done_cycle"] = int(match.group("cycle"))
            continue

        match = CACHE_CONFIG_RE.search(line)
        if match:
            kind = match.group("kind")
            core = int(match.group("core"))
            parsed["cache"][kind][core]["config"] = {
                "sets": int(match.group("sets")),
                "ways": int(match.group("ways")),
                "block_size": int(match.group("block")),
                "non_blocking": int(match.group("nb")),
                "repl": int(match.group("repl")),
                "write": int(match.group("write")),
            }
            continue

        match = ICACHE_STATS_RE.search(line)
        if match:
            core = int(match.group("core"))
            cache = parsed["cache"]["icache"][core]
            cache["requests"] = int(match.group("requests"))
            cache["hits"] = int(match.group("hits"))
            cache["misses"] = int(match.group("misses"))
            cache["writebacks"] = int(match.group("writebacks"))
            cache["writethroughs"] = int(match.group("writethroughs"))
            cache["writebacks_done"] = int(match.group("writebacks_done"))
            continue

        match = DCACHE_STATS_RE.search(line)
        if match:
            core = int(match.group("core"))
            cache = parsed["cache"]["dcache"][core]
            cache["requests"] = int(match.group("requests"))
            cache["hits"] = int(match.group("hits"))
            cache["misses"] = int(match.group("misses"))
            cache["writebacks"] = int(match.group("writebacks"))
            cache["writethroughs"] = int(match.group("writethroughs"))
            cache["writebacks_done"] = int(match.group("writebacks_done"))
            continue

        match = ICACHE_STALL_RE.search(line)
        if match:
            core = int(match.group("core"))
            cache = parsed["cache"]["icache"][core]
            cache["hits_under_wb"] = int(match.group("hits_under_wb"))
            cache["conflict_stall_cycles"] = int(match.group("conflict"))
            cache["buffer_stall_cycles"] = int(match.group("buffer"))
            cache["mem_wait_cycles"] = int(match.group("mem"))
            cache["bus_wait_cycles"] = int(match.group("bus"))
            cache["wb_wait_total"] = int(match.group("wb_total"))
            cache["wb_wait_max"] = int(match.group("wb_max"))
            continue

        match = DCACHE_STALL_RE.search(line)
        if match:
            core = int(match.group("core"))
            cache = parsed["cache"]["dcache"][core]
            cache["hits_under_wb"] = int(match.group("hits_under_wb"))
            cache["conflict_stall_cycles"] = int(match.group("conflict"))
            cache["buffer_stall_cycles"] = int(match.group("buffer"))
            cache["mem_wait_cycles"] = int(match.group("mem"))
            cache["bus_wait_cycles"] = int(match.group("bus"))
            cache["wb_wait_total"] = int(match.group("wb_total"))
            cache["wb_wait_max"] = int(match.group("wb_max"))
            continue

        match = DCACHE_BUS_RE.search(line)
        if match:
            core = int(match.group("core"))
            cache = parsed["cache"]["dcache"][core]
            cache["bus_rd"] = int(match.group("rd"))
            cache["bus_rdx"] = int(match.group("rdx"))
            cache["bus_upgr"] = int(match.group("upgr"))
            cache["invalidations_rx"] = int(match.group("inv"))
            cache["dirty_interventions"] = int(match.group("dirty"))
            cache["downgrades"] = int(match.group("down"))
            continue

        match = FINAL_STATE_RE.search(line)
        if match:
            core = int(match.group("core"))
            parsed["cache"]["dcache"][core]["final_state"] = {
                "M": int(match.group("m")),
                "S": int(match.group("s")),
                "I": int(match.group("i")),
            }
            continue

        match = PHIT_RE.search(line)
        if match:
            kind = match.group("kind")
            core = int(match.group("core"))
            parsed["cache"][kind][core]["p_hit"] = float(match.group("p"))
            continue

        match = BUS_GRANT_RE.search(line)
        if match:
            cmd = int(match.group("cmd"))
            name = BUS_NAMES.get(cmd, f"cmd{cmd}")
            parsed["bus"]["counts"].setdefault(name, 0)
            parsed["bus"]["counts"][name] += 1
            parsed["bus"]["total"] += 1

            if match.group("core") is not None:
                core = int(match.group("core"))
                parsed["bus"]["by_core"][core].setdefault(name, 0)
                parsed["bus"]["by_core"][core][name] += 1
            else:
                parsed["bus"]["wb_grants"] += 1

            parsed["bus"]["snoop_hits"] += int(match.group("snoop"))
            parsed["bus"]["dirty_interventions"] += int(match.group("dirty"))
            continue

        match = BUS_ATOMIC_RE.search(line)
        if match:
            parsed["bus"]["atomic_events"] += 1
            parsed["bus"]["atomic_req_dirty"] += int(match.group("req_dirty"))
            parsed["bus"]["atomic_peer_dirty"] += int(match.group("peer_dirty"))
            continue

        if BOTH_DIRTY_RE.search(line):
            parsed["both_dirty"] += 1
            continue

        match = MEM_ATOMIC_RE.search(line)
        if match:
            parsed["atomic"]["mem_records"] += 1
            parsed["atomic"]["mem_pending"].append(
                {
                    "addr": int(match.group("addr")),
                    "size": int(match.group("size")),
                    "old0": int(match.group("old0")),
                    "consumed": False,
                }
            )
            continue

        match = ATOMIC_REQ_RE.search(line)
        if match:
            core = int(match.group("core"))
            addr = int(match.group("addr"))
            parsed["ops"]["atomic"]["total"] += 1
            parsed["ops"]["atomic"]["by_core"][core] += 1
            parsed["atomic"]["pending_by_core"][core].append(addr)
            continue

        match = ATOMIC_OK_RE.search(line)
        if match:
            core = int(match.group("core"))
            data = _ints(match.group("data"))
            parsed["ops"]["atomic"]["ok"] += 1
            parsed["ops"]["atomic"]["ok_by_core"][core] += 1
            pending = parsed["atomic"]["pending_by_core"][core]
            if not pending:
                parsed["atomic"]["ok_without_req"] += 1
                continue
            addr = pending.pop(0)
            record = None
            for candidate in parsed["atomic"]["mem_pending"]:
                if not candidate["consumed"] and candidate["addr"] == addr:
                    record = candidate
                    break
            if record is None:
                parsed["atomic"]["missing_mem"] += 1
                continue
            record["consumed"] = True
            old0 = record["old0"]
            expected = [
                (old0 >> 24) & 0xFF,
                (old0 >> 16) & 0xFF,
                (old0 >> 8) & 0xFF,
                old0 & 0xFF,
            ]
            if data != expected:
                parsed["atomic"]["mismatches"].append(
                    {
                        "core": core,
                        "addr": addr,
                        "expected": expected,
                        "actual": data,
                    }
                )
            continue

        match = IFETCH_REQ_RE.search(line)
        if match:
            core = int(match.group("core"))
            addr = int(match.group("addr"))
            parsed["ops"]["ifetch"]["total"] += 1
            parsed["ops"]["ifetch"]["by_core"][core] += 1
            parsed["ifetch"]["pending_by_core"][core].append(addr)
            continue

        match = IFETCH_OK_RE.search(line)
        if match:
            core = int(match.group("core"))
            data = _ints(match.group("data"))
            parsed["ops"]["ifetch"]["ok"] += 1
            parsed["ops"]["ifetch"]["ok_by_core"][core] += 1
            pending = parsed["ifetch"]["pending_by_core"][core]
            if not pending:
                parsed["ifetch"]["ok_without_req"] += 1
                continue
            addr = pending.pop(0)
            expected = [(addr + offset) & 0xFF for offset in range(4)]
            if data != expected:
                parsed["ifetch"]["mismatches"].append(
                    {
                        "core": core,
                        "addr": addr,
                        "expected": expected,
                        "actual": data,
                    }
                )
            continue

        match = LOAD_REQ_RE.search(line)
        if match:
            core = int(match.group("core"))
            parsed["ops"]["load"]["total"] += 1
            parsed["ops"]["load"]["by_core"][core] += 1
            continue

        match = LOAD_OK_RE.search(line)
        if match:
            core = int(match.group("core"))
            parsed["ops"]["load"]["ok"] += 1
            parsed["ops"]["load"]["ok_by_core"][core] += 1
            continue

        match = STORE_REQ_RE.search(line)
        if match:
            core = int(match.group("core"))
            parsed["ops"]["store"]["total"] += 1
            parsed["ops"]["store"]["by_core"][core] += 1
            continue

        match = STORE_OK_RE.search(line)
        if match:
            core = int(match.group("core"))
            parsed["ops"]["store"]["ok"] += 1
            parsed["ops"]["store"]["ok_by_core"][core] += 1
            continue

        match = WB_START_RE.search(line)
        if match:
            core = int(match.group("core"))
            parsed["wb"]["start"] += 1
            parsed["wb"]["start_by_core"][core] += 1
            continue

        match = WB_DONE_RE.search(line)
        if match:
            parsed["wb"]["done"] += 1
            continue

        if WB_CANCEL_RE.search(line):
            parsed["wb"]["cancelled"] += 1
            continue

        if ERROR_RE.search(line):
            parsed["error_count"] += 1
            if len(parsed["errors"]) < 50:
                parsed["errors"].append(line)

    return parsed
