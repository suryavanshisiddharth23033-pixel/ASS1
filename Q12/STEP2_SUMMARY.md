# Q12 — Step 2 Summary: MSI State Machine & Snoop Handler

**Objective**: Implement the complete MSI state machine with the `handle_snoop()` method that processes bus commands from peer caches, including the critical write-buffer hazard handling.

---

## Changes Made

### 1. Added CORE_ID Parameter

Added a `CORE_ID` parameter to the `Cache` procedure to identify which core owns this cache instance:

```sitar
parameter int CORE_ID = 0
```

This will be used in later steps to generate core-identifiable store data patterns (e.g., Core 0 writes `0xAAAA...`, Core 1 writes `0xBBBB...`).

---

### 2. Added Coherence Statistics Counters

Added 7 new statistics counters to track coherence protocol activity:

| Counter | Description |
|---------|-------------|
| `statBusRd` | Number of `BusRd` transactions issued (read misses) |
| `statBusRdx` | Number of `BusRdX` transactions issued (write misses) |
| `statBusUpgr` | Number of `BusUpgr` transactions issued (S→M upgrades) |
| `statInvalidationsRx` | Number of invalidations received from peer snoop |
| `statDirtyInterventions` | Number of dirty interventions (M→S or M→I) |
| `statDowngrades` | Number of M→S downgrades |
| `statBusWaitCycles` | Cycles spent waiting for bus grant |

These counters will be included in the cache statistics dump and are essential for verifying correct protocol behavior.

---

### 3. Added Bus Request/Response Channels

Added two sets of communication channels for the `CoherenceBus` to interact with the cache:

#### Front-End Coherence Channel
Used by `Cache::behavior` to issue `BUS_RD`, `BUS_RDX`, `BUS_UPGR`, and `BUS_ATOMIC` requests:

```cpp
bool bus_req_valid;
bool bus_resp_valid;
uint8_t bus_req_cmd;
uint32_t bus_req_addr;
uint8_t bus_req_size;
uint32_t bus_req_d0, bus_req_d1, bus_req_d2, bus_req_d3;
uint8_t bus_resp_err;
uint32_t bus_resp_d0, bus_resp_d1, bus_resp_d2, bus_resp_d3;
```

#### Write-Engine Coherence Channel
Used by `WbEngine` to issue `BUS_WB` requests for dirty victim write-backs and write-through stores:

```cpp
bool wb_bus_req_valid;
bool wb_bus_resp_valid;
uint32_t wb_bus_addr;
uint8_t wb_bus_size;
uint32_t wb_bus_d0, wb_bus_d1, wb_bus_d2, wb_bus_d3;
uint8_t wb_bus_err;
```

These channels will be wired to the `CoherenceBus` in Step 3 when we implement the bus arbiter.

---

### 4. Implemented `handle_snoop()` Method

This is the **core of the MSI protocol**. The method is called by `CoherenceBus` (from within an atomic `$ ... $` block) when the peer cache issues a bus transaction.

#### Signature

```cpp
bool handle_snoop(
    uint8_t cmd,           // Bus command (BUS_RD, BUS_RDX, BUS_UPGR, BUS_ATOMIC)
    uint32_t blockAddr,    // Block-aligned address being snooped
    bool &suppliedDirty,   // Output: true if this cache supplies dirty data
    uint32_t &dirtyD0,     // Output: dirty data words
    uint32_t &dirtyD1,
    uint32_t &dirtyD2,
    uint32_t &dirtyD3
)
```

#### Three Cases Handled

**Case 1: Block is in the write buffer (write-buffer hazard)**

When a `MODIFIED` line is evicted, it's parked in the write buffer (`wbPending == true`) and the cache line is immediately marked `INVALID`. If the peer immediately snoops this block, checking only the tag array would incorrectly report `INVALID`, and the peer would read stale data from memory.

**Solution**: `handle_snoop` checks both the tag array AND the write buffer. If the block is in the buffer:
- Supply the dirty data directly from `wbData0..3`
- Cancel the pending write-back (`wbPending = false`)
- Update statistics (`statDirtyInterventions++`, etc.)

This prevents stale memory reads and eliminates redundant write-backs.

**Case 2: Block is in the tag array**

Based on the bus command and current line state, perform the appropriate state transition:

| Command | Line State | Action | New State | Statistics |
|---------|-----------|--------|-----------|------------|
| `BUS_RD` | `SHARED` | No action | `SHARED` | — |
| `BUS_RD` | `MODIFIED` | Supply dirty data, downgrade | `SHARED` | `statDirtyInterventions++`, `statDowngrades++` |
| `BUS_RDX` | `SHARED` | Invalidate | `INVALID` | `statInvalidationsRx++` |
| `BUS_RDX` | `MODIFIED` | Supply dirty data, invalidate | `INVALID` | `statDirtyInterventions++`, `statInvalidationsRx++` |
| `BUS_UPGR` | `SHARED` | Invalidate (so requester can upgrade) | `INVALID` | `statInvalidationsRx++` |
| `BUS_ATOMIC` | `SHARED`/`MODIFIED` | Flush dirty data if `M`, then invalidate | `INVALID` | `statInvalidationsRx++` (+ `statDirtyInterventions++` if `M`) |

**Case 3: Block not found**

Return `false` with `suppliedDirty = false`.

---

### 5. Updated `read_cache()` to Return Coherence State

Modified the `read_cache()` function signature to also return the coherence state of the hit line:

```cpp
int read_cache(uint32_t addr, int size, int touch, long now, int &hitState)
```

This is needed in Step 4 when `Cache::behavior` needs to know whether a `STORE` hit is in `SHARED` state (requires `BusUpgr`) or `MODIFIED` state (no bus traffic needed).

Updated both call sites to pass a dummy `hitState` variable (the actual state-based logic will be implemented in Step 4).

---

### 6. Initialized All New Variables

Added initialization code in the `init $ ... $` block for:
- All 7 coherence statistics counters (set to 0)
- All bus request/response channel variables (set to 0/false)

---

## Verification

Compiled and ran the single-core simulation to verify Step 2 is a clean refactoring:

```bash
cd Q12
/home/sidd64/Projects/sitar/scripts/sitar translate model.sitar
/home/sidd64/Projects/sitar/scripts/sitar compile
./sitar_sim 100000
```

**Results:**
- Translation: ✅ Success
- Compilation: ✅ Success (no warnings or errors)
- Simulation: ✅ Runs correctly
- Output: ✅ Identical to Q11 (1000 instructions, 25318 cycles, 61.7% hit rate)

This confirms that Step 2 adds the coherence infrastructure **without changing single-core behavior**. The new methods and variables are dormant until Steps 3-5 activate them.

---

## What's Ready for Step 3

Step 2 has laid the groundwork for the `CoherenceBus`:

✅ `handle_snoop()` method ready to be called by the bus  
✅ Bus request/response channels ready to be wired  
✅ Coherence statistics counters ready to track activity  
✅ CORE_ID parameter ready for core identification  

**Next step (Step 3):** Implement the `CoherenceBus` procedure with round-robin arbitration, snoop invocation, and memory port handshaking.

---

## Key Design Decisions

### Why `handle_snoop()` Returns Data Through Output Parameters

The method is called from within an atomic `$ ... $` block in the `CoherenceBus` behavior loop. Returning data through reference parameters (`bool &suppliedDirty`, `uint32_t &dirtyD0`, etc.) allows the bus to immediately use the supplied data in the same atomic step, avoiding the need for additional synchronization.

### Why Check the Write Buffer First

The write-buffer hazard is a critical race condition. If we only checked the tag array, we'd miss blocks that were recently evicted but haven't been written to memory yet. By checking the buffer first, we ensure that:
1. The peer always gets the most recent data (no stale reads)
2. Redundant write-backs are eliminated (better performance)
3. The protocol correctly handles the non-blocking write-buffer semantics from Q11

### Why `handle_snoop()` is a Method (Not a Separate Procedure)

Making it a method of `Cache` (rather than a separate `SnoopHandler` procedure) allows direct access to the cache's internal state (`cache[setIdx][way]`, `wbPending`, etc.) without needing complex pointer wiring. The `CoherenceBus` holds a pointer to the `Cache` and calls this method atomically, which accurately models hardware's independent snoop tag port.
