# Sitar Test Suite

This directory drives `model.sitar` through a repeatable workload matrix, validates
correctness, and writes performance metrics and plots into `reports/`.

## Requirements

- Python 3.10+
- `matplotlib`
- `numpy`

No pandas dependency is required.

## Quick Start

From the repository root:

```bash
python3 test/run_tests.py
```

Build the simulator first when the model changed:

```bash
python3 test/run_tests.py --build
```

Run only selected scenarios:

```bash
python3 test/run_tests.py --only balanced,read_only,atomic_heavy
```

List scenarios and their effective environment:

```bash
python3 test/run_tests.py --list
```

Disable plots:

```bash
python3 test/run_tests.py --no-plots
```

## Runner Options

- `--model PATH`: model source, default `model.sitar`
- `--sim PATH`: simulator binary, default `sitar_sim`
- `--sitar PATH`: Sitar tool, default `/home/sidd64/Projects/sitar/scripts/sitar`
- `--reports PATH`: results root, default `reports`
- `--max-time N`: simulator argument, default `100000`
- `--timeout SECONDS`: wall-clock limit per run, default `240`
- `--build-timeout SECONDS`: wall-clock limit per build command, default `600`
- `--only IDS`: comma-separated scenario ids
- `--list`: print scenarios and exit
- `--build`: run `sitar translate` and `sitar compile` before testing
- `--no-plots`: skip matplotlib output

Exit codes:

- `0`: all runs PASS or WARN
- `1`: at least one FAIL, or a static model check failed
- `2`: configuration, build, or simulator error

## Files

- `scenarios.py`: scenario matrix and effective environment defaults
- `parse_log.py`: log parser for results, cache counters, bus activity, IFETCH,
  atomic old values, writebacks, and error lines
- `validate.py`: per-run correctness checks and static icache/dcache decoupling checks
- `metrics.py`: derived performance metrics and CSV schema
- `plot_results.py`: PNG plots
- `run_tests.py`: CLI, build orchestration, report generation, and `latest` symlink

## Scenarios

| Scenario | Workload / configuration |
| --- | --- |
| `balanced` | 40% load, 30% store |
| `global_rng` | balanced with the legacy global RNG |
| `read_heavy` | 70% load, 10% store |
| `read_only` | 70% load, 0% store |
| `store_heavy` | 10% load, 80% store |
| `load_extreme` | 100% load |
| `store_extreme` | 100% store |
| `atomic_heavy` | 80% load, 80% store (mostly atomics) |
| `atomic_extreme` | 100% load, 100% store (all atomics) |
| `no_data_ops` | no load/store/atomic traffic |
| `balanced_blocking` | balanced with blocking caches |
| `pingpong_t3a` | store-heavy, high temporal locality |
| `pingpong_blocking` | ping-pong with blocking caches |
| `high_locality` | temporal 0.9, spatial 0.9 |
| `low_locality` | temporal 0.2, spatial 0.2 |
| `fast_memory` | `MEMORY_DELAY=5` |
| `slow_memory` | `MEMORY_DELAY=80` |
| `random_repl` | random replacement policy |

All scenarios default to `DET_RNG=1` except `global_rng`, so address streams are
reproducible across runs and are independent of bus arbitration.

## Validation

Each run is checked for:

- successful simulator exit and completion of 1000 instructions on both cores
- no `ERROR`, `INVARIANT VIOLATION`, `OUT OF BOUNDS`, or similar failure lines
- zero `BUS ATOMIC BOTH-DIRTY` events (reported as WARN if present)
- IFETCH request/response counts and data bytes against the IM initialization
- atomic request/response counts and old-value bytes against `MEM ATOMIC old0`
- load/store completion counts
- cache request partition (`hits + misses == requests`)
- bus atomic grants and atomic events matching atomic instructions
- writeback start/done/cancel accounting
- static source checks: no icache write-back engine instances, and peer snoop
  calls plus bus snoop pointers present for both dcache and icache pairs

`BOTH-DIRTY` is treated as a warning because the P2 ordering path is designed to
handle it; any data or invariant failure is still a hard FAIL.

## Constraint

The processor stops at its own `MAX_TIME=100000`. `MEMORY_DELAY=100` does not
complete 1000 instructions before that limit, so `slow_memory` uses delay 80.
`low_locality` uses 0.2/0.2 locality because 0.1 does not complete in time.
`atomic_extreme` completes at roughly 99k cycles and is intentionally kept near
the limit as a stress case.

## Outputs

Every run creates `reports/run_YYYYmmdd_HHMMSS/` containing:

- `raw/*.log`: simulator stdout for each scenario
- `plots/*.png`: cycles, IPC, hit rates, bus mix, coherence events, writeback
  latency, stalls, and atomic share
- `results.csv`: one row per scenario
- `results.json`: full config, validation, metrics, and parsed summaries
- `summary.md`: human-readable table, failures, warnings, and reproduction command
- `config.json`: model/simulator hashes, git commit, Python and plotting versions
- `build.log`: present when `--build` is used

`reports/latest` is updated to point at the newest run.
