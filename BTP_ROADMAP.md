# BTP Roadmap & Phase 1 Implementation Plan: SiTAR Cache Simulation

This document outlines the complete BTP roadmap mapped from the problem statement (`btp.jpg`, Section 2: Cache) and provides a step-by-step implementation plan for **Phase 1** in `Q10/model.sitar`.

---

## Part 1: Overall BTP Roadmap

### Current Progress Summary
| Milestone from `btp.jpg` | Status | Location | Details |
| :--- | :--- | :--- | :--- |
| **1. Sim addr only (`#hits / misses`, Parallel)** | Completed | `parallel_sim/model.sitar` | Address-trace cache simulator using OpenMP (`#pragma omp parallel for`) for parallel associative way tag-matching and hit/miss counting. |
| **2. Sim 1 full cache (`sitar`)** | Completed | `Q1-4` to `Q10/model.sitar` | Cycle-accurate SiTAR simulation with `Processor`, `Core`, split `ICache`/`DCache`, `WbEngine`, and `Memory` holding byte-accurate data arrays. |
| **3. Add WB/WT, Replacement Policies** | Partially Completed | `Q9/model.sitar`, `Q10/model.sitar` | Blocking Write-Back (`Q9`) and Non-Blocking Write-Back with `WbEngine` (`Q10`) are done. **Pending:** Configurable Write-Through (`WT`) policies and Replacement Policies (`LRU`, `FIFO`, `LFU`, `Random`). |
| **4. Simple Coherence Protocol (2 Cores + Shared Mem)** | Upcoming (Phase 2) | Next milestone | Dual-core private L1 caches connected to a single shared `Memory` via a snooping bus implementing MSI/MESI coherence. |
| **5. Tree Structure & N-Levels** | Upcoming (Phase 3) | Final milestone | Uniform port interfaces (`inport<22>`/`outport<17>`) allowing arbitrary $N$-level cache chaining ($L1 \to L2 \to \dots \to LN \to \text{Mem}$) and tree topologies. |

---

### Phase 1: Complete `WB / WT` & `Replacement Policies` in `model.sitar`
Enhance the single-core full cache model (`Q10/model.sitar`) so that both cache replacement policies and write policies are fully parameterized and measurable:
1. **Replacement Policies**: Support `RANDOM` (0), `LRU` (1), `FIFO` (2), and `LFU` (3).
2. **Write Policies**: Support `WRITE_BACK` (0, Write-Allocate), `WRITE_THROUGH_ALLOCATE` (1, Write-Through + Write-Allocate), and `WRITE_THROUGH_NO_ALLOCATE` (2, Write-Through + No-Write-Allocate).
3. **Unified Write Buffer (`WbEngine`)**: Reuse `WbEngine` to handle both 16-byte victim write-backs (in WB mode) and 4B/8B/16B write-through stores (in WT mode), supporting both blocking (`NON_BLOCKING=0`) and non-blocking (`NON_BLOCKING=1`) execution.

---

### Phase 2: `Simple Coherence Protocol` (2-Core Shared Memory Architecture)
Implement the middle diagram from `btp.jpg`: two processors, each connected to its own private cache, sharing a single main memory.

1. **Architecture in `.sitar`**:
   - Two `Processor` instances (`proc0`, `proc1`) and two private `Cache` instances (`dcache0`, `dcache1`).
   - A `CoherenceBus` (or shared bus controller procedure/module) arbitrating access to the single shared `DM : Memory<>`.
2. **MSI Snooping State Machine**:
   - Replace `validBit` and `dirtyBit` in `CacheLine` with `enum CoherenceState { INVALID = 0, SHARED = 1, MODIFIED = 2 }`.
   - **Bus Commands**:
     - `BusRd`: Triggered on a `LOAD` miss. If the peer cache holds the block in `MODIFIED`, it flushes the dirty block to memory/bus and downgrades `MODIFIED -> SHARED`. Requesting cache installs block in `SHARED`.
     - `BusRdX`: Triggered on a `STORE` miss. Peer cache invalidates its copy (`SHARED -> INVALID`, or flushes and transitions `MODIFIED -> INVALID`). Requesting cache installs block in `MODIFIED`.
     - `BusUpgr` (Invalidate): Triggered on a `STORE` hit to a `SHARED` line. Peer cache invalidates (`SHARED -> INVALID`); requesting cache upgrades `SHARED -> MODIFIED` without a memory data fetch.
     - `BusWB`: Triggered when evicting a `MODIFIED` victim block.
3. **Coherence Metrics**:
   - Track invalidations, read-exclusive misses, upgrades (`S -> M`), dirty snoop interventions (`M -> S` / `M -> I`), and bus arbitration stall cycles.

---

### Phase 3: `N-Levels` & `Tree Structure` Cache Hierarchy
Implement the bottom section of `btp.jpg`: multi-level caches and tree topologies.

1. **Standardized Port Interface for `N-Levels`**:
   - Convert `Cache`'s upward (processor-facing) interface from raw C++ pointers to SiTAR ports matching the downward (memory-facing) token protocol:
     - Request port: `inport req_inp : width 22` (`addr`, `type`, `size`, `d0..d3`)
     - Response port: `outport resp_outp : width 17` (`err`, `d0..d3`)
   - Because upward and downward ports use identical token widths (`22` and `17`), any `Cache` instance can be connected in series to form $N$ levels:
     $$\text{Core} \Longleftrightarrow L_1 \Longleftrightarrow L_2 \Longleftrightarrow \dots \Longleftrightarrow L_N \Longleftrightarrow \text{Memory}$$
2. **Tree Topology (`Tree Struc.`)**:
   - Create a parameterized `BusMux<K>` / `CoherentInterconnect<K>` module with $K$ upstream request/response port pairs and $1$ downstream port pair.
   - Connect multiple cores in a tree:
     - Leaves: 4 Cores with private $L_1$ caches (`L1_0..L1_3`).
     - Level 2: Two shared $L_2$ caches (`L2_0` serving `L1_0, L1_1`; `L2_1` serving `L1_2, L1_3`).
     - Root: Shared $L_3$ cache connected to `Memory`.

---

## Part 2: Detailed Implementation Plan for Phase 1 (`Q10/model.sitar`)

All changes for Phase 1 are confined strictly to `Q10/model.sitar`.

### Step 1: Add Policy Parameters & Enums to `procedure Cache`
In `procedure Cache` (around line 676):
1. Add two new parameters with defaults that preserve current behavior:
   ```sitar
   parameter int NUM_SETS = 4
   parameter int NUM_WAYS = 4
   parameter int BLOCK_SIZE = 16
   parameter int CACHE_DELAY = 4
   parameter int REPL_POLICY = 1    // 0: RANDOM, 1: LRU, 2: FIFO, 3: LFU
   parameter int WRITE_POLICY = 0   // 0: WRITE_BACK, 1: WT_ALLOCATE, 2: WT_NO_ALLOCATE
   ```
2. In `decl $ ... $` of `Cache`, define enums and runtime configuration variables:
   ```cpp
   enum ReplacementPolicy {
       REPL_RANDOM = 0,
       REPL_LRU    = 1,
       REPL_FIFO   = 2,
       REPL_LFU    = 3
   };

   enum WritePolicy {
       WB_WRITE_ALLOCATE    = 0,
       WT_WRITE_ALLOCATE    = 1,
       WT_NO_WRITE_ALLOCATE = 2
   };

   int replPolicy;
   int writePolicy;
   ```
3. In `init $ ... $` of `Cache`, allow runtime override via environment variables (matching `NON_BLOCKING`, `TEMPORAL_LOCALITY`, etc.):
   ```cpp
   replPolicy = REPL_POLICY;
   writePolicy = WRITE_POLICY;

   if(const char* e = std::getenv("REPL_POLICY"))
       replPolicy = std::atoi(e);

   if(const char* e = std::getenv("WRITE_POLICY"))
       writePolicy = std::atoi(e);
   ```

---

### Step 2: Extend `CacheLine` Metadata for Replacement Policies
In `class CacheLine` (lines 709–724 of `Q10/model.sitar`):
1. Add replacement tracking fields:
   ```cpp
   class CacheLine {
       public:
           bool validBit;
           bool dirtyBit;
           uint32_t tagBit;
           uint64_t lastAccessCycle; // Used by LRU
           uint64_t insertionCycle;  // Used by FIFO
           uint64_t accessCount;     // Used by LFU
           std::vector<uint8_t> data;

           CacheLine() {
               validBit = false;
               dirtyBit = false;
               tagBit = 0;
               lastAccessCycle = 0;
               insertionCycle = 0;
               accessCount = 0;
               data.resize(BLOCK_SIZE, 0);
           }
   };
   ```
2. Update metadata on **Cache Hit** (`read_cache` and `write_data_to_cache`):
   - Whenever a way hits in `read_cache(addr, size)`:
     ```cpp
     cache[setIdx][way].lastAccessCycle = current_time.cycle();
     cache[setIdx][way].accessCount++;
     ```
   - In `write_data_to_cache(addr, size)`:
     - Keep `lastAccessCycle = current_time.cycle()` updated (avoid double-incrementing `accessCount` if `read_cache` was already called on the same access, or update via a dedicated helper `touch_way(setIdx, way)` called once per hit).
3. Initialize metadata on **Cache Fill** (`write_cache(addr, replaceWay)`):
   ```cpp
   cache[setIdx][replaceWay].tagBit = tag;
   cache[setIdx][replaceWay].validBit = true;
   cache[setIdx][replaceWay].dirtyBit = false;
   cache[setIdx][replaceWay].insertionCycle = current_time.cycle();
   cache[setIdx][replaceWay].lastAccessCycle = current_time.cycle();
   cache[setIdx][replaceWay].accessCount = 1;
   ```

---

### Step 3: Implement `select_victim_way(uint32_t setIdx)`
Replace the random-only `select_victim_way` (lines 1001–1011) with policy-driven selection:
1. First scan ways `0 .. NUM_WAYS - 1` for an invalid line (`!cache[setIdx][way].validBit`); if found, return it immediately.
2. Otherwise, select the victim way based on `replPolicy`:
   - `REPL_RANDOM (0)`: `return rand() % NUM_WAYS;`
   - `REPL_LRU (1)`: return the way with minimum `lastAccessCycle`.
   - `REPL_FIFO (2)`: return the way with minimum `insertionCycle`.
   - `REPL_LFU (3)`: return the way with minimum `accessCount` (tie-breaking with smallest `lastAccessCycle`).

---

### Step 4: Generalize `WbEngine` to Support Both Write-Back (16B) and Write-Through (4B/8B/16B)
Notice that `Memory::write_block(addr, size)` (lines 2184–2206) **already** accepts `WORD` (4B), `DOUBLEWORD` (8B), or `QUADWORD` (16B) at any aligned address.
To let `WbEngine` drain both full-block victim evictions (WB) and sub-block/full-block write-through stores (WT):
1. Add `uint8_t wbSize;` to `Cache`'s write-back buffer declarations (around line 780) and `uint8_t *wb_size;` to `WbEngine` (around line 1766), wired in `Core::init`:
   ```cpp
   icache_wb.wb_size = &icache.wbSize;
   dcache_wb.wb_size = &dcache.wbSize;
   ```
2. In `WbEngine::prepareWriteback()`, use `*wb_size` directly instead of hardcoding `BLOCK_SIZE`:
   ```cpp
   uint32_t addr = *wb_block_addr; // aligned store address (block addr for WB, req addr for WT)
   uint8_t type = STORE;
   uint8_t size = *wb_size;
   ```
3. For conflict detection (`conflict` check on misses/atomics):
   - Compare block-aligned addresses: `(wbBlockAddr & ~((uint32_t)BLOCK_SIZE - 1)) == missBlockAddr`.
4. Add a helper in `Cache` to enqueue any write (victim block or write-through store) into the `WbEngine` buffer:
   - Wait while `wbPending == true` (incrementing `statBufferStall`).
   - Populate `wbBlockAddr`, `wbSize`, `wbData0..3`, and set `wbPending = true`.
   - If `nonBlocking == 0`, wait until `wbPending == false`.

---

### Step 5: Implement Write-Through (`WT`) vs. Write-Back (`WB`) Control Flow in `Cache::behavior`
Modify the `IFETCH / LOAD / STORE` branch (lines 1347–1688):

#### Case A: Cache `HIT` (lines 1357–1400)
- Increment `statTotal` and `statHits`.
- If `requestType == STORE`:
  1. Call `set_store_data()` and `write_data_to_cache(requestAddr, requestSize)`.
  2. **If `writePolicy == WB_WRITE_ALLOCATE`**:
     - `cache[setIdx][way].dirtyBit = true;` (current behavior, no memory request).
  3. **If `writePolicy == WT_WRITE_ALLOCATE` or `WT_NO_WRITE_ALLOCATE`**:
     - Keep `cache[setIdx][way].dirtyBit = false;`.
     - Increment a new counter `statWriteThroughs++`.
     - Park the store `(requestAddr, requestSize, responseData0..3)` in the write buffer (`wbPending = true`) so `WbEngine` writes it through to `Memory` (waiting for completion if `nonBlocking == 0`).

#### Case B: Cache `MISS` (lines 1401–1688)
- Increment `statTotal` and `statMisses`.
- Check for write-buffer block conflict (`(wbBlockAddr & ~(BLOCK_SIZE - 1)) == missBlockAddr`) and stall if needed.
- **Subcase B1: `requestType == STORE` and `writePolicy == WT_NO_WRITE_ALLOCATE`**:
  - Do **not** evict a victim and do **not** fetch the block from `Memory`.
  - Call `set_store_data()`.
  - Increment `statWriteThroughs++`.
  - Park the store `(requestAddr, requestSize, responseData0..3)` in the write buffer (wait until `wbPending` is free, set `wbPending = true`, and if `nonBlocking == 0` wait until drained).
- **Subcase B2: All `IFETCH`/`LOAD` misses, and `STORE` misses under `WB_WRITE_ALLOCATE` or `WT_WRITE_ALLOCATE`**:
  - Select victim way via `select_victim_way(wb_setIdx)`.
  - If victim is valid & dirty (only happens in `WB_WRITE_ALLOCATE`), park the 16-byte block in the write buffer with `wbSize = blockSizeCode`.
  - Acquire memory port (`feWaiting = true`), fetch the 16-byte block from `Memory`, and install via `write_cache(requestAddr, wb_victimWay)`.
  - If `requestType == STORE`:
    - Call `set_store_data()` and `write_data_to_cache(requestAddr, requestSize)`.
    - If `writePolicy == WB_WRITE_ALLOCATE`: set `dirtyBit = true`.
    - If `writePolicy == WT_WRITE_ALLOCATE`: keep `dirtyBit = false`, increment `statWriteThroughs++`, and park the store `(requestAddr, requestSize, responseData0..3)` in the write buffer.

---

### Step 6: Update Statistics Logging & Verification
1. Update the `CACHE STATISTICS` log block (lines 1163–1215) to print:
   - `replPolicy` (`RANDOM`/`LRU`/`FIFO`/`LFU`)
   - `writePolicy` (`WB`/`WT_ALLOC`/`WT_NO_ALLOC`)
   - `writethroughs=` (`statWriteThroughs`) alongside `writebacks=` and `writebacks_done=`.
2. **Verification Matrix**:
   Compile and run `model.sitar` across combinations of environment variables to verify correctness and compare cycle counts / hit rates:
   - `REPL_POLICY=0,1,2,3` (Random vs. LRU vs. FIFO vs. LFU hit rates)
   - `WRITE_POLICY=0,1,2` with `NON_BLOCKING=0,1` (WB vs. WT-Allocate vs. WT-No-Allocate cycle counts and memory traffic)
