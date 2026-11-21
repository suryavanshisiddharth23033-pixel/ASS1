# Sitar Test Run run_20261121_141027

- Overall status: **PASS**
- Timestamp: 2026-11-21T14:10:57
- Model: `/home/sidd64/Sitar_Assignment_1/model.sitar`
- Simulator: `/home/sidd64/Sitar_Assignment_1/sitar_sim`
- Git commit: `34795b3`
- Max simulation time: 100000
- Expected instructions per core: 1000

## Results

| Scenario | Status | Cycles | IPC | IC hit | DC hit | Atomics | Coherence/1k | Errors | Warnings |
| --- | --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: |
| balanced | PASS | 42,733 | 0.047 | 0.883 | 0.502 | 232 | 53.000 | 0 | 0 |
| global_rng | PASS | 44,515 | 0.045 | 0.877 | 0.508 | 261 | 53.500 | 0 | 0 |
| read_heavy | PASS | 35,113 | 0.057 | 0.877 | 0.675 | 134 | 19.000 | 0 | 0 |
| read_only | PASS | 26,607 | 0.075 | 0.867 | 0.819 | 0 | 0.000 | 0 | 0 |
| store_heavy | PASS | 44,065 | 0.045 | 0.861 | 0.692 | 159 | 78.000 | 0 | 0 |
| load_extreme | PASS | 29,831 | 0.067 | 0.862 | 0.855 | 0 | 0.000 | 0 | 0 |
| store_extreme | PASS | 33,019 | 0.061 | 0.862 | 0.845 | 0 | 100.000 | 0 | 0 |
| atomic_heavy | PASS | 98,637 | 0.020 | 0.850 | 0.097 | 1,265 | 46.500 | 0 | 0 |
| atomic_extreme | PASS | 99,342 | 0.020 | 0.847 | 0.000 | 2,000 | 0.000 | 0 | 0 |
| no_data_ops | PASS | 12,208 | 0.164 | 0.910 | 0.000 | 0 | 0.000 | 0 | 0 |
| balanced_blocking | PASS | 42,670 | 0.047 | 0.883 | 0.504 | 232 | 51.000 | 0 | 0 |
| pingpong_t3a | PASS | 47,720 | 0.042 | 0.940 | 0.474 | 368 | 70.000 | 0 | 0 |
| pingpong_blocking | PASS | 47,599 | 0.042 | 0.940 | 0.474 | 368 | 70.000 | 0 | 0 |
| high_locality | PASS | 29,797 | 0.067 | 0.986 | 0.626 | 224 | 0.000 | 0 | 0 |
| low_locality | PASS | 95,655 | 0.021 | 0.427 | 0.256 | 225 | 133.500 | 0 | 0 |
| fast_memory | PASS | 14,356 | 0.139 | 0.883 | 0.504 | 232 | 50.500 | 0 | 0 |
| slow_memory | PASS | 79,235 | 0.025 | 0.883 | 0.503 | 232 | 51.500 | 0 | 0 |
| random_repl | PASS | 43,425 | 0.046 | 0.874 | 0.504 | 232 | 49.500 | 0 | 0 |

## Static Model Checks

| Check | Result | Detail |
| --- | --- | --- |
| no icache write-back engine instances | PASS | found=None |
| dcache0 snoop call present | PASS | found=dcache0->handle_snoop |
| dcache1 snoop call present | PASS | found=dcache1->handle_snoop |
| icache0 snoop call present | PASS | found=icache0->handle_snoop |
| icache1 snoop call present | PASS | found=icache1->handle_snoop |
| bus icache0 snoop pointer present | PASS | found=bus.icache0 = |
| bus icache1 snoop pointer present | PASS | found=bus.icache1 = |

## Failures

None.

## Warnings

None.

## Plots

- [plots/cycles.png](plots/cycles.png)
- [plots/ipc.png](plots/ipc.png)
- [plots/hit_rates.png](plots/hit_rates.png)
- [plots/bus_mix.png](plots/bus_mix.png)
- [plots/coherence.png](plots/coherence.png)
- [plots/wb_latency.png](plots/wb_latency.png)
- [plots/stalls.png](plots/stalls.png)
- [plots/atomic_share.png](plots/atomic_share.png)

## Scenario Configuration

| Scenario | Tags | Environment |
| --- | --- | --- |
| balanced | baseline, performance | `P_LOAD=40 P_STORE=30 TEMPORAL_LOCALITY=0.7 SPATIAL_LOCALITY=0.6 MEMORY_DELAY=40 NON_BLOCKING=1 REPL_POLICY=1 WRITE_POLICY=0 DET_RNG=1` |
| global_rng | rng | `P_LOAD=40 P_STORE=30 TEMPORAL_LOCALITY=0.7 SPATIAL_LOCALITY=0.6 MEMORY_DELAY=40 NON_BLOCKING=1 REPL_POLICY=1 WRITE_POLICY=0 DET_RNG=0` |
| read_heavy | workload | `P_LOAD=70 P_STORE=10 TEMPORAL_LOCALITY=0.7 SPATIAL_LOCALITY=0.6 MEMORY_DELAY=40 NON_BLOCKING=1 REPL_POLICY=1 WRITE_POLICY=0 DET_RNG=1` |
| read_only | workload | `P_LOAD=70 P_STORE=0 TEMPORAL_LOCALITY=0.7 SPATIAL_LOCALITY=0.6 MEMORY_DELAY=40 NON_BLOCKING=1 REPL_POLICY=1 WRITE_POLICY=0 DET_RNG=1` |
| store_heavy | workload | `P_LOAD=10 P_STORE=80 TEMPORAL_LOCALITY=0.7 SPATIAL_LOCALITY=0.6 MEMORY_DELAY=40 NON_BLOCKING=1 REPL_POLICY=1 WRITE_POLICY=0 DET_RNG=1` |
| load_extreme | workload, stress | `P_LOAD=100 P_STORE=0 TEMPORAL_LOCALITY=0.7 SPATIAL_LOCALITY=0.6 MEMORY_DELAY=40 NON_BLOCKING=1 REPL_POLICY=1 WRITE_POLICY=0 DET_RNG=1` |
| store_extreme | workload, stress | `P_LOAD=0 P_STORE=100 TEMPORAL_LOCALITY=0.7 SPATIAL_LOCALITY=0.6 MEMORY_DELAY=40 NON_BLOCKING=1 REPL_POLICY=1 WRITE_POLICY=0 DET_RNG=1` |
| atomic_heavy | atomic, stress | `P_LOAD=80 P_STORE=80 TEMPORAL_LOCALITY=0.7 SPATIAL_LOCALITY=0.6 MEMORY_DELAY=40 NON_BLOCKING=1 REPL_POLICY=1 WRITE_POLICY=0 DET_RNG=1` |
| atomic_extreme | atomic, stress | `P_LOAD=100 P_STORE=100 TEMPORAL_LOCALITY=0.7 SPATIAL_LOCALITY=0.6 MEMORY_DELAY=40 NON_BLOCKING=1 REPL_POLICY=1 WRITE_POLICY=0 DET_RNG=1` |
| no_data_ops | icache | `P_LOAD=0 P_STORE=0 TEMPORAL_LOCALITY=0.7 SPATIAL_LOCALITY=0.6 MEMORY_DELAY=40 NON_BLOCKING=1 REPL_POLICY=1 WRITE_POLICY=0 DET_RNG=1` |
| balanced_blocking | blocking | `P_LOAD=40 P_STORE=30 TEMPORAL_LOCALITY=0.7 SPATIAL_LOCALITY=0.6 MEMORY_DELAY=40 NON_BLOCKING=0 REPL_POLICY=1 WRITE_POLICY=0 DET_RNG=1` |
| pingpong_t3a | coherence | `P_LOAD=35 P_STORE=55 TEMPORAL_LOCALITY=0.85 SPATIAL_LOCALITY=0.6 MEMORY_DELAY=40 NON_BLOCKING=1 REPL_POLICY=1 WRITE_POLICY=0 DET_RNG=1` |
| pingpong_blocking | coherence, blocking | `P_LOAD=35 P_STORE=55 TEMPORAL_LOCALITY=0.85 SPATIAL_LOCALITY=0.6 MEMORY_DELAY=40 NON_BLOCKING=0 REPL_POLICY=1 WRITE_POLICY=0 DET_RNG=1` |
| high_locality | locality | `P_LOAD=40 P_STORE=30 TEMPORAL_LOCALITY=0.9 SPATIAL_LOCALITY=0.9 MEMORY_DELAY=40 NON_BLOCKING=1 REPL_POLICY=1 WRITE_POLICY=0 DET_RNG=1` |
| low_locality | locality | `P_LOAD=40 P_STORE=30 TEMPORAL_LOCALITY=0.2 SPATIAL_LOCALITY=0.2 MEMORY_DELAY=40 NON_BLOCKING=1 REPL_POLICY=1 WRITE_POLICY=0 DET_RNG=1` |
| fast_memory | memory | `P_LOAD=40 P_STORE=30 TEMPORAL_LOCALITY=0.7 SPATIAL_LOCALITY=0.6 MEMORY_DELAY=5 NON_BLOCKING=1 REPL_POLICY=1 WRITE_POLICY=0 DET_RNG=1` |
| slow_memory | memory | `P_LOAD=40 P_STORE=30 TEMPORAL_LOCALITY=0.7 SPATIAL_LOCALITY=0.6 MEMORY_DELAY=80 NON_BLOCKING=1 REPL_POLICY=1 WRITE_POLICY=0 DET_RNG=1` |
| random_repl | cache-policy | `P_LOAD=40 P_STORE=30 TEMPORAL_LOCALITY=0.7 SPATIAL_LOCALITY=0.6 MEMORY_DELAY=40 NON_BLOCKING=1 REPL_POLICY=0 WRITE_POLICY=0 DET_RNG=1` |

## Reproduction

```bash
python3 test/run_tests.py --reports reports --max-time 100000
```

Raw logs: `run_20261121_141027/raw/`  
Machine-readable results: `run_20261121_141027/results.json`, `run_20261121_141027/results.csv`
