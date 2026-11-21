# Sitar Test Run run_20261121_140957

- Overall status: **FAIL**
- Timestamp: 2026-11-21T14:09:57
- Model: `/home/sidd64/Sitar_Assignment_1/model.sitar`
- Simulator: `/home/sidd64/Sitar_Assignment_1/sitar_sim`
- Git commit: `34795b3`
- Max simulation time: 100000
- Expected instructions per core: 1000

## Results

| Scenario | Status | Cycles | IPC | IC hit | DC hit | Atomics | Coherence/1k | Errors | Warnings |
| --- | --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: |
| balanced | PASS | 42,733 | 0.047 | 0.883 | 0.502 | 232 | 53.000 | 0 | 0 |

## Static Model Checks

| Check | Result | Detail |
| --- | --- | --- |
| no icache snoop calls | FAIL | found=icache1->handle_snoop |
| no bus icache ports | FAIL | found=bus.icache0 |
| dcache0 snoop call present | PASS | found=dcache0->handle_snoop |
| dcache1 snoop call present | PASS | found=dcache1->handle_snoop |

## Failures

None.

## Warnings

None.

## Plots

Plots disabled or unavailable.

## Scenario Configuration

| Scenario | Tags | Environment |
| --- | --- | --- |
| balanced | baseline, performance | `P_LOAD=40 P_STORE=30 TEMPORAL_LOCALITY=0.7 SPATIAL_LOCALITY=0.6 MEMORY_DELAY=40 NON_BLOCKING=1 REPL_POLICY=1 WRITE_POLICY=0 DET_RNG=1` |

## Reproduction

```bash
python3 test/run_tests.py --reports reports --max-time 100000
```

Raw logs: `run_20261121_140957/raw/`  
Machine-readable results: `run_20261121_140957/results.json`, `run_20261121_140957/results.csv`
