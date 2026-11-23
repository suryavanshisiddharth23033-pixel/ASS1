# Dual Core Cache Simulation on SiTAR

Siddharth Suryavanshi · Roll No: 2303325

## 1. Introduction

This project builds a cycle-level model of a dual-core processor in SiTAR in which each core has private L1 caches and both cores share one memory system, and it keeps those caches coherent with an MSI snooping protocol. The model is configurable, measurable and checked automatically.

The reason coherence matters is simple. Every core keeps private caches because main memory is too slow to feed it, but a private copy of a shared variable can silently go stale. If Core 0 writes X = 9 while Core 1 still holds X = 5, Core 1's next load returns a wrong value without any crash or warning. Locks, counters and producer-consumer queues all rely on one core seeing another's write, so the protocol has to be right even in rare races.

The work had four goals:

- Model two in-order cores that issue instruction fetches, loads, stores and atomic read-modify-writes, with tunable temporal and spatial locality.
- Build set-associative L1 instruction and data caches with selectable replacement and write policies, and a write-back buffer.
- Implement a shared coherence bus with MSI states and the BusRd, BusRdX, BusUpgr, BusWB and BusAtomic transactions.
- Verify the design with an automated regression and built-in invariant checks, and use it to study the cost of coherence traffic.

## 2. System Architecture

The model has two cores, each with a processor, an L1 instruction cache, an L1 data cache and a write engine. All of them meet at one snooping coherence bus, which is the only master of the two memories (instruction memory and data memory). The top-level SiTAR module connects the DualCore block to the memories through single-slot nets.

| Component | Configuration |
| --- | --- |
| Cores | 2 in-order, blocking; 1000 instructions each; 3-cycle execution delay; every instruction begins with a fetch |
| L1 I-cache | 4 sets x 4 ways x 16 B = 256 B; LRU; 4-cycle access; read-only |
| L1 D-cache | 2 sets x 8 ways x 16 B = 256 B; LRU; write-back, write-allocate; 4-cycle access; MSI state per line |
| Write-back buffer | One entry per D-cache, drained by a separate write engine |
| Coherence bus | Single shared bus; round-robin over 6 requesters; 2-cycle snoop delay |
| Memory | Separate instruction and data memory; one request at a time; 40-cycle default latency |
| Address space | 1 KB, with 4, 8 and 16-byte aligned accesses |

The processor does not run a real instruction set. It generates synthetic traffic: a load and a store are drawn independently for every instruction. Load only gives a LOAD, store only gives a STORE, both together give a coherent ATOMIC read-modify-write, and neither gives a fetch only. Defaults are a 40% load and 30% store probability, with temporal locality 0.7 and spatial locality 0.6. All of these can be changed through environment variables, as can the replacement policy, write policy and memory latency.

The caches are deliberately tiny (256 B each) against a 1 KB address space, so that conflicts, evictions and sharing happen often enough to exercise the protocol in a short run.

Assumptions: exactly two cores, one shared bus, invalidate-based MSI with no Exclusive state, and synthetic rather than trace-driven traffic.

## 3. Cache and Coherence Design

Each D-cache line is in one of three states: Modified (the only valid copy, memory is stale), Shared (a clean copy, the peer may hold one too) or Invalid. The I-cache is read-only, so it never holds a Modified line.

| Event | Bus command | What happens |
| --- | --- | --- |
| Read hit (S or M) | none | Served locally in 4 cycles |
| Read miss | BusRd | A peer in M supplies the data and drops to S; otherwise memory supplies it. Line installs in S |
| Write miss | BusRdX | A peer in M hands over its data; a peer in S is invalidated. Line installs in M |
| Write hit in S | BusUpgr | Peer copy is invalidated, no data moves |
| Write hit in M | none | Silent write |
| Atomic RMW | BusAtomic | Dirty copies are flushed, both copies invalidated, the RMW runs at memory and the old value returns |
| Evict M line | BusWB | Parked in the write buffer and drained by the write engine |

A few design decisions carry most of the correctness:

- **Fair arbitration.** One rotating pointer covers D$0, D$1, I$0, I$1 and the two write engines, so no requester has fixed priority and none can be starved.
- **Snoop every transaction.** For each request other than a write-back, the bus calls handle\_snoop on the peer D-cache and the I-caches. I-cache snoops only confirm sharing and never supply dirty data.
- **Snoop the write buffer.** A dirty line waiting in the write buffer is still the newest copy. The snoop supplies it and cancels the queued memory write, so stale data never reaches memory later.
- **Ordered atomics.** BusAtomic flushes the peer's older dirty copy first and the requester's newer copy last, so the authoritative value wins. It then does the read-modify-write at memory.
- **Race recovery.** If a peer invalidates a Shared line while our upgrade is waiting for the bus, the request is retried as BusRdX so a Modified peer still hands over fresh data.
- **Write-back buffer.** A dirty victim moves into a one-entry buffer and a separate engine writes it back, so the core can continue past an eviction. This is controlled by a non-blocking flag.

## 4. Implementation Challenges

Most of the effort went into the corner cases, because a basic MSI protocol is easy to write and the races are not. These are the ones that shaped the final code.

1. **Upgrade versus invalidation race.** A Shared line can be invalidated by the peer while its own BusUpgr is still queued for the bus. The cache re-checks the line when the bus answers. If it is gone, the access is counted as a miss and the request is retried as BusRdX, so a Modified peer still intervenes.
2. **Atomics and stale copies.** An atomic must not be clobbered by a later write-back. The cache first waits for any parked write-back of the same block to drain. The bus then flushes dirty copies in a fixed order, invalidates both caches, and runs the read-modify-write at memory.
3. **Dirty data in the write buffer.** Once a victim is parked, it has left the cache arrays but is still the newest copy. The snoop therefore checks the buffer too. If the write engine had already queued the write on the bus, that request is cancelled with a special error code (3) and the engine acknowledges the cancellation before the buffer is reused.
4. **Snoops overwriting response data.** The bus calls snoop functions on a cache while that cache is waiting out its access delay, and these reuse the same response registers. The cache now saves its response before the delay and sends the saved copy.
5. **Fair sharing of the bus.** A fixed-priority bus could starve instruction fetches or write-backs, so arbitration rotates over all six requesters.
6. **Fair comparison of variants.** Bus timing changes how many random numbers each core consumes, so two runs would not do identical work. An optional deterministic per-core random generator (DET\_RNG) gives each core an address stream independent of arbitration.

Correctness is also checked inside the model. After every non-write-back transaction the bus audits the block: if one D-cache holds it Modified the other must hold it Invalid, and no I-cache may ever hold a Modified line. Any breach is logged as an invariant violation and counted.

## 5. Results and Verification

All 18 workload scenarios pass with zero errors and zero warnings, and all 7 static model checks pass. Each scenario runs 1000 instructions per core and covers loads, stores, atomics, locality, memory latency, replacement and write policy, and blocking versus non-blocking mode. The static checks confirm, among other things, that snoop hooks reach all four caches and that no I-cache has a write-back engine. The figures below come from run run\_20261122\_152018 of the test script (python3 test/run\_tests.py --reports reports --max-time 100000), which saves raw logs, results.json, results.csv and plots.

| Observation | Result |
| --- | --- |
| Read-only runs | 0 coherence events; shared lines stay in S |
| High versus low locality | 29,797 vs 95,655 cycles, about 3.2x faster |
| Memory latency 5 to 80 cycles | 14,356 to 79,235 cycles, about 5.5x slower |
| Coherence traffic vs latency | 50.5, 53.0 and 51.5 events per 1k at 5, 40 and 80 cycles |
| store\_extreme / store\_heavy | 100 and 78 coherence events per 1k |
| low\_locality | I-cache hit rate 0.427; 133.5 coherence events per 1k |
| atomic\_extreme | 2,000 atomics, slowest run at 99,342 cycles |
| Non-blocking vs blocking (balanced) | 42,733 vs 42,670 cycles |

These match what MSI predicts. Reads alone never cause invalidations, and stores drive coherence traffic. That traffic depends on the workload and not on memory timing. Atomics are fully serialised through the bus, which is why the atomic stress run is slowest. Locality is the biggest performance lever.

There are limits to note. Combined IPC is at most 0.164, because the cores are in-order and blocking, memory takes 40 cycles and one bus is shared. The two atomic stress runs finish at 98,637 and 99,342 cycles, close to the 100,000-cycle cap. Non-blocking mode gives almost no gain because a stalled core cannot overlap the write-back. Finally, validation uses synthetic traffic, two cores and invariant checks, not end-to-end data-value comparison against a reference.

## 6. Conclusion and Future Work

The result is a verified, parameterised dual-core cache model in SiTAR. It shows both that the MSI protocol is correct, including the awkward races around upgrades, atomics and the write buffer, and how much its traffic costs. Locality and memory latency dominate performance, and coherence traffic follows the workload.

Future work could include:

- MESI, where an Exclusive state removes upgrade traffic on private data.
- More cores and a directory protocol instead of one shared bus.
- A split-transaction bus to overlap snoops and memory access.
- Real program traces instead of synthetic traffic, and end-to-end data-value checking.
- Write-through policies under coherence, and false-sharing tests.

## 7. AI Use

AI assistance was used as a design and debugging aid, not as an authority. The assistant helped compare architecture alternatives, reason about BUS\_ATOMIC and write-back-buffer snooping, propose bug fixes, interpret logs, and draft documentation. Every change was reviewed, built, and run through the full suite. Performance and correctness numbers are taken from the generated run, not from AI estimates; no results or logs were fabricated.

| AI model / tool | Used for | Verification |
| --- | --- | --- |
| opencode CLI; session model deepseek-v4.1-flash-free | Architecture reasoning; BUS\_ATOMIC and write-back-buffer snoop design; debugging; bug fixing; report and slide drafting | Reviewed source; rebuilt; 18/18 suite pass; zero invariant and atomic errors |

## References

- Gate Smasher, YouTube playlist: Computer Organization and Architecture.
- Computer Architecture Basics: Cache Organization.
- Neso Academy, YouTube playlist, for understanding the MSI protocol.
- MIT 6.004 L25: Cache Coherence.
- SiTAR simulation framework documentation and course assignment handout.
