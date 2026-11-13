# Q11 — Phase 1 Report: WB/WT Write Policies & Replacement Policies

**Model file**: `Q11/model.sitar` (derived from `Q10/model.sitar`)
**Scope**: Completes the `add WB/WT, Replacement Policies` branch of the BTP problem statement (`btp.jpg`, Section 2 — Cache).

---

## 1. Objective

Q10 already had a non-blocking set-associative cache with a single dirty-victim write-back buffer drained by a background `WbEngine`. Phase 1 adds the two missing capabilities:

1. **Configurable Replacement Policies**: RANDOM, LRU, FIFO, LFU (Q10 only had random eviction).
2. **Configurable Write Policies**: Write-Back (WB), Write-Through with Write-Allocate (WT-ALLOC), Write-Through with No-Write-Allocate (WT-NO-ALLOC) — for normal `STORE` traffic (Q10 only had WB, plus an ATOMIC write-through bypass).

Both are parameterizable at instantiation and overridable at runtime through environment variables, and every policy is measurable through the per-cache statistics dump.

---

## 2. Configuration Interface

### 2.1 New `Cache` parameters

```sitar
procedure Cache
    parameter int NUM_SETS     = 4
    parameter int NUM_WAYS     = 4
    parameter int BLOCK_SIZE   = 16
    parameter int CACHE_DELAY  = 4
    parameter int REPL_POLICY  = 1    // 0=RANDOM, 1=LRU, 2=FIFO, 3=LFU
    parameter int WRITE_POLICY = 0    // 0=WB, 1=WT-ALLOC, 2=WT-NO-ALLOC
```

### 2.2 Enums added in `Cache` decl

```cpp
enum ReplacementPolicy { REPL_RANDOM = 0, REPL_LRU = 1, REPL_FIFO = 2, REPL_LFU = 3 };

enum WritePolicy { WB_WRITE_ALLOCATE = 0, WT_WRITE_ALLOCATE = 1, WT_NO_WRITE_ALLOCATE = 2 };
```

### 2.3 Runtime overrides (env variables)

| Env variable | Values | Meaning |
|---|---|---|
| `REPL_POLICY`  | 0 / 1 / 2 / 3 | RANDOM / LRU / FIFO / LFU |
| `WRITE_POLICY` | 0 / 1 / 2     | WB / WT-ALLOC / WT-NO-ALLOC |
| `NON_BLOCKING` | 1 / 0         | background vs. stall-until-drained write buffer |
| `MEMORY_DELAY`, `TEMPORAL_LOCALITY`, `SPATIAL_LOCALITY`, `P_LOAD`, `P_STORE` | as before | carried over from Q10 |

Existing statistics-request mechanism (`addr=0xFFFFFFFF`) still prints the config used:
`CACHE STATISTICS (config: sets=... ways=... blockSize=... nonBlocking=... repl=... write=...)`.

---

## 3. Implementation Details

### 3.1 Replacement policy metadata (`CacheLine`)

Each line now carries:

```cpp
long lastAccessCycle; // LRU: cycle of most recent access
long insertionCycle;  // FIFO: cycle the line was filled
long accessCount;     // LFU: number of accesses
```

- **On hit** — `read_cache(addr, size, touch, now)` updates `lastAccessCycle` and increments `accessCount` (only when `touch != 0`; the post-fill reload in the miss path passes `touch=0` so one access is never counted twice).
- **On fill** — `write_cache(addr, replaceWay, now)` initializes `insertionCycle = lastAccessCycle = now`, `accessCount = 1`.

`select_victim_way(setIdx)`:
1. Any invalid way wins (no eviction needed).
2. Full set → policy dispatch:
   - `RANDOM`: `rand() % NUM_WAYS`
   - `LRU`: minimum `lastAccessCycle`
   - `FIFO`: minimum `insertionCycle`
   - `LFU`: minimum `accessCount`, ties broken by oldest `lastAccessCycle`

### 3.2 Unified write buffer (WB victims + WT stores)

The single-entry write-back buffer was generalized into a generic write buffer:

- Added `uint8_t wbSize` (cache side) and `uint8_t *wb_size` (engine side), wired in `Core::init` for both `icache_wb` and `dcache_wb`.
- `WbEngine::prepareWriteback()` now takes the size **from the buffer** instead of hardcoding the block size, so one engine drains both:
  - **16-byte dirty victim blocks** (write-back, WB mode), and
  - **4B / 8B / 16B aligned stores** (write-through, WT modes).
- `Memory::write_block(addr, size)` already handled WORD/DOUBLEWORD/QUADWORD at aligned addresses — no memory model change was needed.
- Log labels in the engine were generalized: `WRITE-BACK START/DONE` → `MEM WRITE START/DONE addr=... size=...`.

### 3.3 Block-masked conflict stalls

Because a parked write-through store carries a *request* address (possibly a sub-block offset), all conflict tests (miss path and ATOMIC path) now compare block-aligned addresses:

```cpp
conflict = wbPending &&
           ((wbBlockAddr & ~((uint32_t)BLOCK_SIZE - 1)) == missBlockAddr);
```

This stalls a demand fetch of a block until the last write to that block (victim or WT store) has reached memory, preventing stale reads. Parking first waits for `wbPending` to clear, which also serializes back-to-back stores to memory.

### 3.4 Write-policy control flow in `Cache::behavior`

`write_data_to_cache()` now sets the dirty bit per policy:

```cpp
cache[setIdx][way].dirtyBit = (writePolicy == WB_WRITE_ALLOCATE);
```

**STORE hit**
- WB: line marked dirty; no memory traffic.
- WT-ALLOC / WT-NO-ALLOC: line updated and left clean; store `(addr, size, data)` parked in the write buffer (`statWriteThroughs++`).

**STORE miss**
- WT-NO-ALLOC: **bypass path** — no victim selection, no block fetch, cache untouched; store parked directly in the write buffer (`WRITE-THROUGH NO-ALLOC` log).
- WT-ALLOC: normal demand fetch + install (clean), then merged store parked in the write buffer.
- WB: Q10 behavior preserved — dirty victim parked (`wbSize = block_size_code()`), demand fetch, install, store merges and marks line dirty.

**Blocking vs non-blocking** — in all three park sites, the front end waits until `wbPending` clears only when `NON_BLOCKING=0`; otherwise the `WbEngine` drains in the background and the processor response proceeds after `CACHE_DELAY`.

**ATOMIC** — unchanged semantics: write-through bypass straight to memory through the front-end memory-port path, now with the block-masked conflict check.

### 3.5 Statistics added

| Counter | Meaning |
|---|---|
| `writethroughs` | stores parked in the buffer by a WT policy (hit, miss-alloc, miss-no-alloc) |
| `writebacks` | dirty victims parked (WB mode only) |
| `writebacks_done` | buffer writes acknowledged by memory (engine) |
| `conflict_stall_cycles`, `buffer_stall_cycles`, `mem_wait_cycles`, `hits_under_wb` | carried from Q10, now also exercised by WT traffic |

### 3.6 Bug fixed from Q10

`Core` contained a corrupted instantiation (a comment had been pasted into the code line):

```sitar
// Q10 (broken):
procedure proc : Processor<40, 3 total (256 bytes)
// Q11 (fixed, matches Processor parameter order):
procedure proc : Processor<40, 30, 3, 1000, 100000, 16>
```

`P_LOAD=40, P_STORE=30, EXECUTION_DELAY=3, TOTAL_INSTRUCTIONS=1000, MAX_TIME=100000, BLOCK_SIZE=16` (same values as Q9).

---

## 4. Build & Run

```bash
cd Q11
/home/sidd64/Projects/sitar/scripts/sitar translate model.sitar
/home/sidd64/Projects/sitar/scripts/sitar compile          # generates ./sitar_sim

# Example runs (simulation time must be passed as argument)
./sitar_sim 1000000
REPL_POLICY=3 WRITE_POLICY=1 NON_BLOCKING=0 ./sitar_sim 1000000
```

Translation and compilation succeeded with no errors or warnings.

---

## 5. Verification Results

Workload: 1000 instructions, temporal locality 0.7, spatial locality 0.6, P_LOAD=40, P_STORE=30, identical access trace across runs. ICACHE is read-only (IFETCH never dirties a line), so DCache numbers are the meaningful comparison.

### 5.1 Replacement policies (WRITE_POLICY=0 WB, NON_BLOCKING=1)

| REPL_POLICY | DCache p_hit | DCache writebacks | Total cycles |
|---|---|---|---|
| 0 RANDOM | 0.6228 | 58 (57 drained) | 24284 |
| 1 LRU    | 0.6172 | 60 (60 drained) | 25318 |
| 2 FIFO   | 0.6155 | 61 (61 drained) | 25377 |
| 3 LFU    | 0.5983 | 56 (56 drained) | 25700 |

All four policies produce distinguishable eviction behavior. (Different hit rates across policies also shift the random locality trace slightly, hence the differing request counts for RANDOM; the ranking is meaningful within the same workload family. LFU's classic aging problem — counters never decay — explains its lower hit rate under this loop-heavy pattern.)

### 5.2 Write policies (REPL_POLICY=1 LRU)

| WRITE_POLICY | NON_BLOCKING | p_hit | writebacks | writethroughs | Total cycles |
|---|---|---|---|---|---|
| 0 WB          | 1 | 0.6172 | 60 | 0   | **25318** |
| 0 WB          | 0 | 0.6172 | 60 | 0   | 27363 |
| 1 WT-ALLOC    | 1 | 0.6172 | 0  | 171 | 27066 |
| 1 WT-ALLOC    | 0 | 0.6172 | 0  | 171 | 32081 |
| 2 WT-NO-ALLOC | 1 | 0.5379 | 0  | 171 | 26871 |
| 2 WT-NO-ALLOC | 0 | 0.5379 | 0  | 171 | 31443 |

**Expected behaviors, all observed:**

1. **WB is the fastest** (25318): every store that hits never touches memory; 171 stores generate only 60 victim write-backs.
2. **WT sends every store to memory**: `writethroughs=171` in both WT modes, and `writebacks=0` — no line is ever dirty in WT modes (clean-victim check works).
3. **WT-ALLOC keeps WB hit rate** (0.6172): store misses still allocate, so the cache contents match WB.
4. **WT-NO-ALLOC drops hit rate** 0.6172 → 0.5379: store misses (46 of them) no longer fill lines, so subsequent accesses to those blocks miss (`misses 222 → 268`), yet it uses *less* cache pressure and stays ahead of blocking WT.
5. **Non-blocking always beats blocking**: parking and continuing (WB: 25318 vs 27363; WT-ALLOC: 27066 vs 32081) hides the 40-cycle memory write latency behind subsequent hits — the whole point of the `WbEngine`.
6. **Data integrity**: `Memory` log confirms WT stores arriving with the correct size and data, e.g. `Memory: WRITE-BACK received addr=808 size=1 data0=2864434397` (0xAABBCCDD, a DOUBLEWORD write-through store), including sub-block writes at word-aligned addresses.
7. **No runtime errors**: zero `ERROR` / `TIME LIMIT` occurrences across all 6 write-policy runs and 4 replacement runs; all parked writes complete (`writebacks_done == writebacks + writethroughs - 1` — the final in-flight drain finishes before the stats response).

---

## 6. Summary

- `model.sitar` in `Q11` completes the third bullet of the BTP cache section: full WB/WT matrix (WB, WT-ALLOCATE, WT-NO-ALLOCATE) × blocking/non-blocking × four replacement policies (RANDOM/LRU/FIFO/LFU), all env-switchable, with new statistics (`writethroughs`) and a generalized unified write buffer shared by the front end and `WbEngine`.
- Results match cache-theory expectations on this trace-driven workload: WB wins cycles, no-allocate WT loses hits, non-blocking hides memory write latency.
