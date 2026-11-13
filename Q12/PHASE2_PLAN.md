# Q12 — Phase 2 Implementation Plan: 2-Core MSI Snooping Cache Coherence Protocol

**Target Model**: `Q12/model.sitar` (evolving from `Q11/model.sitar`)  
**Roadmap Milestone**: Phase 2 — *Simple Coherence Protocol (2 Cores + Private L1 Caches + Shared Bus + Shared Memory)* (`BTP_ROADMAP.md`, Milestone 4)

---

## 1. Objective & Scope

In `Q11/model.sitar` (Phase 1), we completed a single-core cycle-accurate cache subsystem featuring configurable replacement policies (`RANDOM`, `LRU`, `FIFO`, `LFU`), configurable write policies (`WB`, `WT-ALLOC`, `WT-NO-ALLOC`), and a non-blocking background write engine (`WbEngine`).

In **Phase 2 (`Q12/model.sitar`)**, we scale the architecture to a **dual-core shared-memory multiprocessor** where two independent processors (`proc0`, `proc1`) access private L1 caches (`dcache0`, `dcache1`) connected to a single shared Data Memory (`DM : Memory<>`) through an arbitrated snooping bus (`CoherenceBus`) implementing the **MSI (`Modified`, `Shared`, `Invalid`)** coherence protocol.

### Key Deliverables for Phase 2
1. **2-Core Shared-Memory Topology**: Two processors with private split `ICache`/`DCache` and write engines sharing a single main memory through a snooping interconnect.
2. **MSI Snooping State Machine**: Full transition logic for processor requests (`LOAD`, `STORE`, `EVICT`, `ATOMIC`) and snooped bus transactions (`BusRd`, `BusRdX`, `BusUpgr`, `BusWB`, `BusAtomic`).
3. **Deadlock-Free Snoop & Write-Buffer Hazard Handling**: Atomic dual-ported tag snooping and write-buffer snoop forwarding/cancellation so non-blocking evictions (`WbEngine`) never cause stale memory reads across cores.
4. **Coordinated Multi-Core Execution & Coherence Telemetry**: Barrier-synchronized simulation termination and per-cache/bus counters for upgrades (`S -> M`), invalidations (`S/M -> I`), dirty interventions (`M -> S`, `M -> I`), and bus arbitration stall cycles.

---

## 2. Target System Architecture in SiTAR

```
                   +----------------------- DualCore Module -----------------------+
                   |                                                               |
                   |  +-------+     +---------+ (+-----------+ )                   |
                   |  | proc0 | <== | dcache0 | (|dcache0_wb | ) ==+               |
                   |  +-------+     +---------+ (+-----------+ )   |               |
                   |                     ^                         v               |
                   |                     | snoop            +--------------+       |      net (width 22)     +---------------+
                   |                     +----------------> |              | ======|=======================> |               |
                   |                                        | CoherenceBus |       |                         | DM : Memory<> |
                   |                     +----------------> |  (Arbiter +  | <=====|======================== |  (Shared 1KB) |
                   |                     | snoop            |Snoop Engine) |       |      net (width 17)     +---------------+
                   |                     v                  +--------------+       |
                   |  +-------+     +---------+ (+-----------+ )   ^               |
                   |  | proc1 | <== | dcache1 | (|dcache1_wb | ) ==+               |
                   |  +-------+     +---------+ (+-----------+ )                   |
                   |                                                               |
                   +---------------------------------------------------------------+
```

### Architectural Design Decisions for SiTAR

1. **Why Snoop Lookups Must Be Decoupled from the Cache Front-End Loop**:
   - In `Q11/model.sitar`, `Cache::behavior` is a sequential state machine that suspends inside `do ... while` loops when waiting for memory responses or `wait(CACHE_DELAY, 0)`.
   - If both `dcache0` and `dcache1` miss in the same cycle and request the bus simultaneously, one cache wins bus arbitration while the other blocks waiting for bus grant.
   - If the winning cache waited for the losing cache's `behavior` loop to wake up and answer a snoop request, the simulation would **deadlock**.
   - **Solution**: Following the exact pattern established in `Q10`/`Q11` (where `WbEngine` shares `Cache` state via pointers wired in `init $ ... $`), `CoherenceBus` holds direct C++ pointers to `dcache0` and `dcache1`. When `CoherenceBus` grants the bus to Core $i$, it invokes `dcache[1-i]->handle_snoop(...)` directly inside an atomic `$ ... $` block. This accurately models hardware's independent snoop tag port with zero deadlock risk.

2. **Instruction Memory (`IM`) vs. Data Memory (`DM`)**:
   - Both cores fetch instructions (`IFETCH`) from read-only instruction caches (`icache0`, `icache1`) backed by instruction memory (`IM0`, `IM1` or arbitrated `IM`). Because instruction fetches never write, no coherence invalidations occur on `ICache`.
   - All shared-memory reads, writes, atomics, and victim write-backs from `dcache0` and `dcache1` pass through `CoherenceBus` to the single shared `DM : Memory<>`.

---

## 3. Step-by-Step Implementation Approach

### Step 1: Replace `validBit` / `dirtyBit` with MSI `CoherenceState` & Bus Enums

In `procedure Cache` (`decl $ ... $`):

1. Define the MSI coherence states and bus command types:
   ```cpp
   enum CoherenceState {
       INVALID  = 0,   // I: Line is not valid
       SHARED   = 1,   // S: Line is valid, clean, potentially cached by peer
       MODIFIED = 2    // M: Line is valid, dirty, exclusively owned by this cache
   };

   enum BusCmd {
       BUS_NONE   = 0,
       BUS_RD     = 1, // Read miss (requests shared copy)
       BUS_RDX    = 2, // Write miss (requests exclusive copy, invalidates peer)
       BUS_UPGR   = 3, // Write hit on SHARED line (invalidates peer, no memory fetch)
       BUS_WB     = 4, // Dirty victim write-back or write-through store to memory
       BUS_ATOMIC = 5  // Atomic read-modify-write (flushes/invalidates peer first)
   };
   ```

2. Update `class CacheLine` (replacing raw `validBit` and `dirtyBit` booleans):
   ```cpp
   class CacheLine {
       public:
           CoherenceState state;
           uint32_t tagBit;
           long lastAccessCycle; // LRU
           long insertionCycle;  // FIFO
           long accessCount;     // LFU
           std::vector<uint8_t> data;

           CacheLine() {
               state = INVALID;
               tagBit = 0;
               lastAccessCycle = 0;
               insertionCycle = 0;
               accessCount = 0;
               data.resize(BLOCK_SIZE, 0);
           }

           bool isValid() const { return state != INVALID; }
           bool isDirty() const { return state == MODIFIED; }
   };
   ```

---

### Step 2: Specify and Implement the Complete MSI State Machine

The MSI protocol is governed by two coupled transition tables: **Processor-Side Transitions** (initiated by the local core) and **Bus-Snoop Transitions** (triggered on the peer cache when the bus broadcasts a command).

#### 2.1 Processor-Initiated Transitions (Local Cache $i$)

| Operation | Current State | Bus Action Issued | Data Source | Next State | Notes |
| :--- | :---: | :---: | :--- | :---: | :--- |
| **`LOAD` (PrRd)** | `INVALID` | `BusRd(blockAddr)` | Peer cache (if peer in `M`) or `DM` | **`SHARED`** | If victim way is `MODIFIED`, park victim in write buffer (`BusWB`) first. |
| **`LOAD` (PrRd)** | `SHARED` | *None* (Cache Hit) | Local cache | **`SHARED`** | Served in `CACHE_DELAY` cycles. |
| **`LOAD` (PrRd)** | `MODIFIED` | *None* (Cache Hit) | Local cache | **`MODIFIED`** | Served in `CACHE_DELAY` cycles. |
| **`STORE` (PrWr)** | `INVALID` | `BusRdX(blockAddr)` | Peer cache (if peer in `M`) or `DM` | **`MODIFIED`** | Peer invalidates (`S/M -> I`). Fetched block merged with store data. |
| **`STORE` (PrWr)** | `SHARED` | `BusUpgr(blockAddr)` | Local cache (no 16B fetch) | **`MODIFIED`** | **Coherence Upgrade**: Peer invalidates (`S -> I`); skips 40-cycle memory read. |
| **`STORE` (PrWr)** | `MODIFIED` | *None* (Silent Hit) | Local cache | **`MODIFIED`** | Already exclusive & dirty; updates data in place with zero bus traffic. |
| **Victim Eviction** | `SHARED` | *None* (Silent Drop) | — | **`INVALID`** | Clean line; evicted silently without bus traffic. |
| **Victim Eviction** | `MODIFIED` | `BusWB(victimAddr)` | Local victim block $\to$ `DM` | **`INVALID`** | Parked in `WbEngine` buffer and written back to shared `DM`. |

#### 2.2 Bus-Snooped Transitions (Peer Cache $1 - i$)

When Cache $i$ issues a bus transaction for `blockAddr`, the bus snoops Peer Cache $1-i$ (checking **both** its tag array `cache[setIdx][way]` and its non-blocking write buffer `wbPending`):

| Snooped Bus Command | Peer Line State | Peer State Transition | Peer Snoop Action | Counter Incremented on Peer |
| :--- | :---: | :---: | :--- | :--- |
| **`BusRd`** | `INVALID` | `INVALID -> INVALID` | No action (snoop miss). | — |
| **`BusRd`** | `SHARED` | `SHARED -> SHARED` | No action; both caches now hold block in `SHARED`. | — |
| **`BusRd`** | `MODIFIED` | **`MODIFIED -> SHARED`** | **Dirty Intervention**: Peer supplies its modified 16B block to the bus (and `CoherenceBus` writes it back to `DM` so main memory is up to date). | `statDirtyInterventions++`, `statDowngrades++` |
| **`BusRdX`** | `INVALID` | `INVALID -> INVALID` | No action (snoop miss). | — |
| **`BusRdX`** | `SHARED` | **`SHARED -> INVALID`** | **Invalidate**: Peer invalidates its shared copy. | `statInvalidations++` |
| **`BusRdX`** | `MODIFIED` | **`MODIFIED -> INVALID`** | **Dirty Intervention + Invalidate**: Peer supplies its modified 16B block to the bus so requester gets latest data before overwriting, and invalidates its line. | `statDirtyInterventions++`, `statInvalidations++` |
| **`BusUpgr`** | `INVALID` | `INVALID -> INVALID` | No action. | — |
| **`BusUpgr`** | `SHARED` | **`SHARED -> INVALID`** | **Invalidate**: Peer invalidates its shared copy so requester becomes exclusive owner (`MODIFIED`). | `statInvalidations++` |
| **`BusAtomic`** | `SHARED` / `MODIFIED` | **`S/M -> INVALID`** | If `MODIFIED`, flushes dirty data to `DM` first; invalidates cached line so atomic RMW operates on coherent memory. | `statInvalidations++` (+ `statDirtyInterventions++` if `M`) |

#### 2.3 Resolving the Non-Blocking Write-Buffer Snoop Hazard (`wbPending`)
Because `Q11/model.sitar` uses a non-blocking write-back buffer (`wbPending`), a subtle race can occur in a 2-core system:
- Suppose Core 1 evicts a `MODIFIED` block $B$ into `dcache1`'s write buffer (`dcache1.wbPending == true`, `dcache1.wbBlockAddr == B`). The line in `dcache1`'s tag array is now marked `INVALID`, **but `dcache1_wb` has not yet written $B$ to `DM`**.
- If Core 0 immediately misses on block $B$ (`BusRd` or `BusRdX`), checking only `dcache1`'s tag array would report `INVALID`, and Core 0 would read **stale data** from `DM`!
- **Fix in `Cache::handle_snoop(...)`**:
  1. Alongside checking `find_way(tag, setIdx)`, `handle_snoop` also checks:
     ```cpp
     bool hitWbBuf = wbPending &&
                     ((wbBlockAddr & ~((uint32_t)BLOCK_SIZE - 1)) == blockAddr) &&
                     (wbSize == block_size_code());
     ```
  2. If `hitWbBuf` is true:
     - Peer supplies `wbData0..3` directly as a dirty intervention (`suppliedDirtyData = true`).
     - Peer clears `wbPending = false` and increments `statWbDone++` because `CoherenceBus` either writes the block to `DM` (on `BusRd`) or hands exclusive ownership of the block to Core 0 (on `BusRdX`), eliminating the redundant background write-back!

---

### Step 3: Design and Implement `procedure CoherenceBus`

Create a dedicated `procedure CoherenceBus` inside the dual-core module that arbitrates between the two cores and drives the shared `dm_outp` (`width 22`) and `dm_inp` (`width 17`) ports to `DM : Memory<>`.

#### 3.1 Per-Core Bus Request Channel
Each `Cache` exposes a bus request/response structure (wired by pointer in `init $ ... $`):
- **Front-End Coherence Channel** (used by `Cache` for `BUS_RD`, `BUS_RDX`, `BUS_UPGR`, `BUS_ATOMIC`):
  - `bool bus_req_valid`, `bool bus_resp_valid`
  - `uint8_t bus_req_cmd` (`BusCmd`), `uint32_t bus_req_addr`, `uint8_t bus_req_size`
  - `uint32_t bus_req_d0..d3`
  - `uint8_t bus_resp_err`, `uint32_t bus_resp_d0..d3`
- **Write-Engine Channel** (used by `WbEngine` for `BUS_WB`):
  - `bool wb_bus_req_valid`, `bool wb_bus_resp_valid`
  - `uint32_t wb_bus_addr`, `uint8_t wb_bus_size`, `uint32_t wb_bus_d0..d3`
  - `uint8_t wb_bus_err`

#### 3.2 Fair Round-Robin Arbitration Policy
In every cycle, when `CoherenceBus` is idle:
1. Maintain `int rrTurn = 0;` (toggles `0 <-> 1` after each serviced transaction so neither core starves).
2. **Priority 1 — Front-End Demand Requests (`bus_req_valid`)**: Check `core[rrTurn]` first, then `core[1 - rrTurn]`.
3. **Priority 2 — Background Write-Buffer Drains (`wb_bus_req_valid`)**: Only when no front-end demand request is waiting (matching `feWaiting` priority from `Q11`), check `wb[rrTurn]`, then `wb[1 - rrTurn]`.
4. While a cache's `bus_req_valid` is true and waiting to be granted the bus, increment its `statBusWaitCycles++`.

#### 3.3 Bus Transaction Execution Flow in `CoherenceBus::behavior`
Once a requester (Core `id`, peer `peer = 1 - id`) is granted the bus:

1. **Case `BUS_UPGR` (Snoop-Only Upgrade `S -> M`)**:
   - Call `dcache[peer]->handle_snoop(BUS_UPGR, blockAddr, ...)`.
   - Wait `BUS_SNOOP_DELAY` (e.g., `wait(2, 0)` cycles for bus broadcast + tag invalidation).
   - Set `bus_resp_err = 0; bus_resp_valid = true;` — **no `Memory` request is sent**, saving the 40-cycle `MEMORY_DELAY`!

2. **Case `BUS_RD` (Read Miss)**:
   - Call `dcache[peer]->handle_snoop(BUS_RD, blockAddr, suppliedDirty, snoopD0..3)`.
   - **If `suppliedDirty == true` (`M -> S` intervention)**:
     - Peer had modified data and downgraded `M -> S`.
     - `CoherenceBus` packs a 16-byte `STORE` token with `snoopD0..3` and pushes it to `DM` (phase 1 push, phase 0 pull) so main memory is updated to match the `SHARED` state.
     - Returns `snoopD0..3` to the requesting cache.
   - **If `suppliedDirty == false`**:
     - `CoherenceBus` packs a 16-byte `LOAD` token for `blockAddr`, pushes to `DM` in `this_phase == 1`, waits for `DM`'s response in `this_phase == 0`, and returns the fetched block to the requesting cache.

3. **Case `BUS_RDX` (Write Miss)**:
   - Call `dcache[peer]->handle_snoop(BUS_RDX, blockAddr, suppliedDirty, snoopD0..3)`. Peer invalidates its copy (`S/M -> I`).
   - **If `suppliedDirty == true` (`M -> I` intervention)**:
     - Peer supplied the most recent 16B dirty block and invalidated its line.
     - Because the requesting core is immediately entering `MODIFIED` state and modifying the block, `CoherenceBus` does **not** need to read stale data from `DM`! Wait `BUS_SNOOP_DELAY` (`wait(2, 0)`) for cache-to-cache bus transfer and return `snoopD0..3` directly to the requester.
   - **If `suppliedDirty == false`**:
     - Fetch the 16-byte block from `DM` (`LOAD` token) and return it to the requester.

4. **Case `BUS_WB` (Victim Write-Back / Write-Through Store from `WbEngine`)**:
   - Pack a `STORE` token (`wb_bus_addr`, `STORE`, `wb_bus_size`, `wb_bus_d0..3`), push to `DM` in `this_phase == 1`, wait for response in `this_phase == 0`, and set `wb_bus_resp_valid = true`.
   - If the write is a sub-block Write-Through store (`writePolicy != WB_WRITE_ALLOCATE`), also snoop-invalidate the peer cache (`BUS_UPGR`) so the peer never retains stale `SHARED` data.

5. **Case `BUS_ATOMIC` (`ATOMICLOADSTORE`)**:
   - Snoop peer cache with `BUS_ATOMIC`. If peer held the block in `MODIFIED`, write the dirty block to `DM` first and invalidate peer. Also invalidate the requesting cache's own copy if present.
   - Issue `ATOMICLOADSTORE` token to `DM` and return the pre-atomic word to the requester.

---

### Step 4: Update `Cache::behavior` for Coherent Hits, Upgrades, and Misses

Update the `IFETCH / LOAD / STORE` logic in `Cache::behavior` (`Q11/model.sitar` lines 1479–2075):

1. **Cache Lookup (`read_cache`)**:
   - Returns `HIT` if `cache[setIdx][way].state != INVALID` and `tagBit == tag`. Also records the hitting line's state `hitState = cache[setIdx][way].state`.

2. **On `cacheStatus == HIT`**:
   - **`LOAD` / `IFETCH` Hit (`SHARED` or `MODIFIED`)**:
     - Increment `statTotal++`, `statHits++`. Serve data locally with zero bus traffic.
   - **`STORE` Hit in `MODIFIED` state**:
     - Increment `statTotal++`, `statHits++`.
     - Call `set_store_data()` and `write_data_to_cache(requestAddr, requestSize)`. Line remains `MODIFIED`. Zero bus traffic.
   - **`STORE` Hit in `SHARED` state (Coherence Upgrade `S -> M`)**:
     - Increment `statTotal++`, `statHits++`, `statUpgrades++`.
     - Issue `BUS_UPGR` for `blockAddr` on `CoherenceBus` and wait for `bus_resp_valid`.
     - Note: While waiting for `BUS_UPGR` bus grant, what if the peer core beat us to the bus with its own `BusRdX` or `BusUpgr` on the same `blockAddr` and invalidated our `SHARED` line?!
     - **Race-Condition Safeguard (Snoop-While-Waiting for Upgrade)**:
       - After `BUS_UPGR` is granted, re-check `find_way(tag, setIdx)`.
       - If our line is still `SHARED`, merge the store bytes and set `state = MODIFIED`.
       - If a peer snoop invalidated our line while we were waiting in the bus queue (`find_way < 0`), seamlessly convert the request into a `BUS_RDX` fetch so we never write into an invalidated line!

3. **On `cacheStatus == MISS` (`INVALID`)**:
   - Increment `statTotal++`, `statMisses++`.
   - Stall on local write-buffer conflict if `wbPending` holds the target block.
   - Select victim way via `select_victim_way(wb_setIdx)`.
   - If victim line is in `MODIFIED` state (`cache[wb_setIdx][wb_victimWay].state == MODIFIED`), park the 16-byte dirty block in the local write buffer (`wbPending = true`, `statWritebacks++`) and set victim `state = INVALID`. (If victim is `SHARED`, evict silently without write-back.)
   - **If `requestType == LOAD` or `IFETCH`**:
     - Issue `BUS_RD` for `blockAddr` via `CoherenceBus`.
     - Install returned 16-byte block into `wb_victimWay` with `state = SHARED`.
     - Extract requested word/doubleword/quadword into `responseData0..3`.
   - **If `requestType == STORE` (Write-Allocate)**:
     - Increment `statReadExclusives++`.
     - Issue `BUS_RDX` for `blockAddr` via `CoherenceBus`.
     - Install returned 16-byte block into `wb_victimWay`, merge store data via `set_store_data()` + `write_data_to_cache()`, and set `state = MODIFIED`.

---

### Step 5: Dual-Core `Processor` Enhancements & Coordinated Stop

1. **Core Identification (`CORE_ID`)**:
   - Add `parameter int CORE_ID = 0` to `Processor` and `Cache`.
   - In `Cache::set_store_data()`, generate core-identifiable store words:
     - Core 0 writes `0xAAAA0000 | (statTotal & 0xFFFF)` (or `0xAAAA1111`).
     - Core 1 writes `0xBBBB0000 | (statTotal & 0xFFFF)` (or `0xBBBB2222`).
     - This makes cross-core data sharing immediately verifiable in the simulation trace (e.g., Core 1 executing `LOAD` and printing `0xAAAA...` written by Core 0).

2. **Address Generation & Shared Memory Contention**:
   - Both `proc0` and `proc1` generate addresses in the shared `ADDR_SPACE = 1024` (64 blocks of 16 bytes).
   - Provide an optional environment variable `SHARED_REGION_MASK` / hot-spot probability so we can tune contention from low sharing (full 1024B space) to high sharing (e.g., 128B shared window = 8 cache blocks) to heavily exercise `BusRd`, `BusRdX`, and `BusUpgr`.

3. **Coordinated Dual-Core Termination (Fixing Unilateral `stop simulation`)**:
   - In `Q11/model.sitar`, a single `Processor` calls `stop simulation;` as soon as it finishes `TOTAL_INSTRUCTIONS`.
   - In `Q12/model.sitar`, with two cores running at slightly different miss rates:
     - Each `Processor` has a pointer `bool *my_done;` and `bool *peer_done;`.
     - When `proc0` finishes its 1000 instructions, it sends the statistics request (`0xFFFFFFFF`) to `icache0` and `dcache0`, sets `*my_done = true;`, and checks `if(*peer_done)`:
       - If the peer has **not** finished yet, `proc0` executes `stop behavior;` so `proc1`, `CoherenceBus`, and `Memory` continue running until `proc1` finishes.
       - The second processor to finish prints the global `COHERENCE BUS SUMMARY` and executes `stop simulation;`.

---

### Step 6: Coherence Statistics & Verification Plan

#### 6.1 Extended Per-Cache & Bus Telemetry
Extend the `0xFFFFFFFF` statistics dump in `Cache` and `CoherenceBus` to report:

| Metric | Scope | Description |
| :--- | :---: | :--- |
| `requests`, `hits`, `misses`, `p_hit` | Per Cache | Standard hit/miss counters from Phase 1. |
| `bus_rd` | Per Cache | Number of `BusRd` transactions issued (read misses). |
| `bus_rdx` (`read_exclusives`) | Per Cache | Number of `BusRdX` transactions issued (write misses). |
| `bus_upgr` (`upgrades`) | Per Cache | Number of `SHARED -> MODIFIED` upgrades on `STORE` hits (`BusUpgr`). |
| `invalidations_rx` | Per Cache | Number of times a valid line (`S` or `M`) was invalidated by a peer snoop. |
| `dirty_interventions` | Per Cache | Number of times a `MODIFIED` line was snooped by peer (`M -> S` on `BusRd` or `M -> I` on `BusRdX`). |
| `bus_wait_cycles` | Per Cache | Cycles spent waiting for `CoherenceBus` arbitration grant. |
| `state_M`, `state_S`, `state_I` | Per Cache | End-of-simulation snapshot of cache ways in `MODIFIED`, `SHARED`, and `INVALID`. |

#### 6.2 Four-Stage Verification Matrix

1. **Test 1 — Read-Only Sharing (`P_LOAD=70 P_STORE=0`)**:
   - *Expected behavior*: Both `dcache0` and `dcache1` issue `BusRd` on cold misses; lines settle into `SHARED` in both caches simultaneously; `bus_rdx = 0`, `bus_upgr = 0`, `invalidations_rx = 0`, `dirty_interventions = 0`, `state_M = 0`.
2. **Test 2 — Balanced Read/Write Sharing (`P_LOAD=40 P_STORE=30`, default)**:
   - *Expected behavior*: Active mix of `BusRd`, `BusRdX`, and `BusUpgr`. Verify that `STORE` hits on `SHARED` lines trigger `BUS_UPGR` (2-cycle snoop invalidation instead of 40-cycle memory fetch) and invalidate the peer's copy.
3. **Test 3 — High-Contention Ping-Pong Stress (`P_LOAD=35 P_STORE=55 TEMPORAL_LOCALITY=0.85`)**:
   - *Expected behavior*: Frequent `M -> S` (`BusRd` dirty intervention) and `M -> I` (`BusRdX` dirty intervention) transitions; non-zero `bus_wait_cycles` showing bus contention between Core 0 and Core 1; test with both `NON_BLOCKING=1` and `NON_BLOCKING=0`.
4. **Test 4 — Coherence Invariant Audit**:
   - Add an invariant assertion inside `CoherenceBus` after every transaction completes:
     - **Single-Writer / Multiple-Reader Invariant**: For any block address $B$, if `dcache0` holds $B$ in `MODIFIED`, `dcache1` must hold $B$ in `INVALID` (and vice versa). If both hold $B$ simultaneously, both must be in `SHARED`.
     - Verify 0 assertion failures, 0 `ERROR` logs, and full completion of 1000 instructions on **both** `proc0` and `proc1`.

---

## 4. Implementation Checklist for `Q12/model.sitar`

- [ ] Copy `Q11/model.sitar` to `Q12/model.sitar` as the baseline.
- [ ] Update `CacheLine` in `procedure Cache` with `CoherenceState { INVALID, SHARED, MODIFIED }`.
- [ ] Add `handle_snoop(...)` method to `procedure Cache` handling `BUS_RD`, `BUS_RDX`, `BUS_UPGR`, `BUS_ATOMIC` across both `cache[set][way]` and `wbPending`.
- [ ] Implement `procedure CoherenceBus` with round-robin arbitration, snoop invocation, dirty-intervention write-back, and phase-accurate `DM` port handshaking.
- [ ] Update `Cache::behavior` to issue `BUS_RD`, `BUS_RDX`, `BUS_UPGR`, and `BUS_ATOMIC`, including the upgrade race-condition re-check.
- [ ] Update `WbEngine` to route victim write-backs and write-through stores through its `CoherenceBus` write channel.
- [ ] Instantiate `proc0`/`proc1`, `icache0`/`icache1`, `dcache0`/`dcache1`, `dcache0_wb`/`dcache1_wb`, and `CoherenceBus` inside the dual-core module with coordinated `procDone` barrier termination.
- [ ] Run the 4-stage verification suite and record results in `Q12/PHASE2_REPORT.md`.
