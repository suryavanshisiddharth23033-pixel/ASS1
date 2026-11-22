# Use of AI Assistance and References

## Dual-Core MSI Cache-Coherence Simulator

**Artifact:** `model.sitar`  
**Model SHA-256:** `3b9cd995535da5ae837f219e84810f013164661ac2b94a82290fc86a2a463c0e`  
**Validated run:** `reports/run_20261122_152018`  
**Validation result:** 18/18 scenarios PASS, 0 failures, 0 warnings, 0 simulator errors

---

## 1. Purpose and Scope

This report is a compact disclosure of how an AI coding assistant was used during the
project and a list of the cache-design references consulted. It covers the five areas
requested: architecture design, the bus-level atomic load-store path
(`BUS_ATOMIC` / `ATOMICLOADSTORE`), snooping data from the write-back buffer,
debugging, and bug fixing. It is a companion to `BTP_Report.md`; it does not replace
that report and does not claim that AI output is authoritative.

The AI was used as a design and debugging assistant. It helped compare alternatives,
explain protocol behavior, propose code changes, and interpret simulator logs. Every
change was reviewed, built, and exercised by the test suite. The performance and
correctness numbers in this report come from the generated run directory, not from
AI-generated estimates. No results, logs, or reference links were fabricated.

---

## 2. Summary of AI Use

| Area | AI assistance provided | How it was checked |
| --- | --- | --- |
| Architecture design | Compared snooping MSI with directory coherence for a two-core design; checked split instruction/data caches, the serialized bus, six-way round-robin arbitration, and the rule that instruction caches have no write-back engine but still snoop their peer. | Static checks confirm no icache write-back instance and that all four caches have peer-snoop paths. All 18 scenarios pass. |
| `BUS_ATOMIC` / `ATOMICLOADSTORE` | Helped turn the atomic requirement into a bus transaction: drain a same-block write-back, snoop the peer, release the requester's copy, order dirty flushes, perform the memory RMW, and return the pre-atomic value. | Atomic validator reports zero old-value mismatches, including 1,265 atomics in `atomic_heavy` and 2,000 in `atomic_extreme`. |
| Snooping data from the WB buffer | Helped reason about the case where a dirty line is already invalid but its data is still in the one-entry write buffer; proposed supplying that data and cancelling any queued `BUS_WB`. | Write-back accounting is consistent; no simulator errors; all 18 scenarios pass. |
| Debugging | Helped trace response corruption across `CACHE_DELAY`, upgrade races, same-block write-back conflicts, and bus/snoop ordering. | Logs, invariant audit, saved-response path, and zero atomic/data mismatches. |
| Bug fixing | Helped unify duplicate write-through staging, restore the `wtNeeded` intent after upgrade/RDX recovery, correct the in-flight write-back cancellation handshake, and clean up the nested `BUS_ATOMIC` indentation. | Full build and 18/18 test suite pass with unchanged cycle counts. |
| Documentation and QA | Helped organize this disclosure, validate reference links, and check report structure. | Links checked on 22 November 2026; report reviewed manually. |

The assistant did not have independent access to hidden simulator state. Debugging
relied on the source, the generated logs, and controlled test runs. When an AI
suggestion conflicted with observed behavior, the observed behavior and the test suite
took precedence.

---

## 3. Architecture Design

The design is a two-core system with private split L1 caches, one instruction memory
(`IM`), one data memory (`DM`), and one serialized coherence bus. Each core has an
instruction cache and a data cache. Only the two data caches have write-back engines;
the instruction caches are read-only and never generate a dirty line. All four caches
are nevertheless snooped: data-cache traffic snoops the peer data cache, and an
instruction fetch snoops the peer instruction cache over `IM`. The coherence domain for
MSI is `{dcache0, dcache1, DM}`; the instruction caches are kept consistent with each
other but cannot supply dirty data or invalidate a requester.

AI assistance was useful in checking the consequences of those choices. The assistant
helped compare snooping MSI against a directory protocol and supported the simpler
snooping choice for two cores sharing a serialized bus. It also helped check that the
bus has six requesters (two icaches, two dcaches, two write-back engines), that
arbitration is six-way round-robin, and that adding peer instruction-cache snooping does
not require adding instruction-cache write-back engines. The final design therefore
keeps the requested asymmetry: instruction caches are coherent but read-only; data
caches are full MSI participants with write-back support.

Human verification was structural as well as dynamic. The static checks in
`test/validate.py` confirm that `icache0`, `icache1`, `dcache0`, and `dcache1` all have
`handle_snoop` call sites, that the bus has snoop pointers for both instruction caches,
and that no instruction-cache write-back engine instance exists. The full suite then
exercises the design across read-heavy, write-heavy, atomic-heavy, locality, memory-
latency, replacement, and blocking/non-blocking scenarios.

---

## 4. `BUS_ATOMIC` / `ATOMICLOADSTORE`

Atomics are not performed as ordinary store hits. A data-cache atomic is forwarded to
the bus as `BUS_ATOMIC`, using the exact word address. The bus derives the enclosing
block for snooping. The implemented sequence is:

1. The requester waits for any pending write-back of the same block to drain, so a late
   write-back cannot clobber the atomic result.
2. The bus snoops the peer dcache with `BUS_ATOMIC`; a modified peer supplies its dirty
   block and is invalidated.
3. The bus calls `release_for_atomic` on the requester. If the requester is modified,
   its dirty block is captured; in all cases its line becomes invalid.
4. If both copies were dirty, the peer's older copy is written to `DM` first and the
   requester's newer copy is written last, so the requester's value wins.
5. The bus sends `ATOMICLOADSTORE` to `DM`. Memory returns the pre-atomic value and
   writes the atomic test value `0x11223344`.
6. The bus returns the pre-atomic value to the requester. Both data caches are invalid
   afterward, so no stale copy survives.

AI assistance was used to reason about this ordering and to keep the defensive
both-dirty path explicit. The both-dirty case is counted and logged as
`BUS ATOMIC BOTH-DIRTY`; the current results contain zero such events, so the path is
defensive rather than frequently exercised. The important correctness property is that
the requester receives the value that existed before the atomic, not the newly written
value, and that both caches lose their copies.

The atomic validator checks three events for every atomic: the `BUS ATOMIC` grant, the
memory line containing the old value, and the processor's `ATOMIC OK` response. The
suite reports zero mismatches in every scenario. This validates the bus serialization,
both-cache invalidation, memory read-modify-write, and the saved-response path together.

---

## 5. Snooping Data from the Write-Back Buffer

A dirty line is invalidated in the cache array when it is parked in the one-entry write
buffer. That creates a subtle case: a snoop can arrive after the line is invalid but
before the write-back drains. If the snoop only searched the cache array, it would miss
the dirty data even though the block is still present in the buffer. The design treats
the write buffer as the source of truth in that window.

`handle_snoop()` checks `wbPending`, whether `wbBlockAddr` matches the requested block,
and whether `wbSize` matches the block size. If the buffer holds the block and the cache
array does not, the snoop supplies `wbData0..3` as dirty data. If the write-back request
is already outstanding, the cache clears `wb_bus_req_valid`, sets `wb_bus_err = 3`, and
asserts `wb_bus_resp_valid`; the write-back engine treats error 3 as a successful
cancellation, not a data error. If no request is outstanding, the buffer is released
immediately after supplying its data and `statWbDone` is incremented. If the buffer is
in the middle of the handshake, `wbPending` remains set until the engine consumes the
acknowledgement, so the buffer cannot be reused underneath the handshake.

AI assistance helped make this race explicit and helped check the cancellation
handshake. The key design point is that a snoop-supplied dirty block and a queued
`BUS_WB` are mutually exclusive outcomes: the data must be supplied once, and the
write-back must not also write the same block later. Write-back accounting in the
results is consistent with this behavior, and no scenario reports a write-back or
snoop error.

---

## 6. Debugging

The main debugging work was protocol-level rather than syntax-level. The assistant was
used to form hypotheses from the logs and then to suggest focused experiments. Four
examples illustrate the process.

**Response corruption across `CACHE_DELAY`.** A cache can be snooped while it is waiting
before responding to its processor. If the snoop overwrites the response registers, the
processor can receive the wrong data. The fix is the saved-response path: the cache
stores `responseErr` and `responseData0..3` in `savedResponse*`, waits `CACHE_DELAY`, and
sends the saved values. The validator reports zero data errors and zero atomic
old-value mismatches.

**Upgrade race.** A store hit on a shared line issues `BUS_UPGR`, but the peer can win
exclusive ownership while the upgrade is queued and invalidate the line. The fix is to
re-check after the grant. If the line is still valid, the store completes and the line
becomes modified. If the line is gone, the operation is converted into a `BUS_RDX` miss,
the block is installed as modified, and the store is applied. This is why the design
handles both the normal upgrade and the aborted-upgrade recovery path.

**Same-block write-back conflict.** An atomic or a miss must not proceed while a
write-back for the same block is still parked, or the late write-back could overwrite
newer data. The cache waits for the conflict to drain and counts the conflict stall.
The balanced run reports zero conflict stalls; buffer stalls are counted separately.

**Bus and snoop ordering.** The serialized bus is the single ordering point. Debugging
therefore focused on whether the snoop ran before the memory transaction, whether a
dirty peer supplied data instead of memory, and whether both caches were invalidated
after an atomic. The invariant audit checks after every non-write-back transaction that
no two data caches hold the same block in conflicting states. Across all 18 scenarios,
the count of invariant violations is zero.

AI helped compare these symptoms against the expected MSI transitions and propose
small, testable changes. The final judgment came from the simulator logs and the
validator, not from the assistant's explanation alone.

---

## 7. Bug Fixing

| Bug or risk | Fix applied | Evidence |
| --- | --- | --- |
| Write-through staging was duplicated and the `wtNeeded` intent could be lost after an upgrade or RDX recovery. | Unified staging into one `if(wtNeeded)` block; set `wtNeeded` on a normal write-through hit, after a successful `BUS_UPGR` re-check, and after `BUS_RDX` recovery; clear it after staging. | Build passes; full suite 18/18 PASS; write-through stores are staged once. |
| A snoop could overwrite the processor response during `CACHE_DELAY`. | Saved the response fields before the delay and sent the saved values afterward. | Zero data errors and zero atomic old-value mismatches. |
| A queued write-back could race with a snoop supplying the same dirty block. | Cancel the queued request with `wb_bus_err = 3`; keep `wbPending` set until the engine consumes the acknowledgement. | Write-back accounting is consistent; no errors. |
| The nested `else if(grantedCmd == BUS_ATOMIC)` branch had misleading indentation. | Re-indented the branch and its five closing `end if` statements. | Build passes; cycle counts unchanged. |
| Both data caches could, in principle, be dirty for the same block during an atomic. | Flush the peer's older copy first and the requester's newer copy last; count and log the both-dirty case. | Zero both-dirty events in current results; defensive path is explicit. |

The write-through fix is the clearest example of AI-assisted bug fixing. The original
path had more than one staging site and a fragile intent flag, so a store could be
staged twice or not at all after an upgrade/recovery sequence. The assistant helped
identify the duplicated control flow and proposed a single staging block driven by
`wtNeeded`. The change was then verified by rebuilding and rerunning the complete
matrix, not by assuming the refactor was correct.

---

## 8. Verification, Limits, and Responsible Use

The final validated run is `reports/run_20261122_152018`:

| Check | Result |
| --- | --- |
| Scenarios | 18 |
| Passed | 18 |
| Failed | 0 |
| Warnings | 0 |
| Simulator errors | 0 |
| Both-dirty events | 0 |
| Invariant violations | 0 |
| Atomic old-value mismatches | 0 |
| Instructions per core | 1,000 in every scenario |

The static checks also confirm that there is no instruction-cache write-back engine and
that all four caches have peer-snoop paths. The balanced baseline completes in 42,733
cycles with 923 bus transactions; the atomic stress cases complete in 98,637 cycles
(1,265 atomics) and 99,342 cycles (2,000 atomics).

The limits are important. The validation is dynamic and scenario-based, not a formal
proof. It does not exhaustively enumerate all interleavings, all address streams, or all
possible cache states. Deterministic mode improves reproducibility but narrows the
diversity of timing interleavings relative to global-RNG mode. The AI's suggestions are
therefore treated as hypotheses that must survive compilation, static checks, runtime
invariants, and the full scenario matrix.

Responsible use in this project meant: no fabricated measurements, no hidden changes to
the model, no AI-generated reference links, and no replacement of human review. The
references below were used for conceptual background; the implementation remains
specific to this assignment.

---

## 9. References

### Gate Smashers (YouTube)

1. Gate Smashers, *Computer Organization and Architecture (Complete Playlist)*.
   <https://www.youtube.com/playlist?list=PLxCzCOWd7aiHMonh3G6QNKq53C6oNXGrX>
2. Gate Smashers, *L-3.5: What is Cache Mapping | Cache Mapping Techniques*.
   <https://www.youtube.com/watch?v=m1dA7D6c3C0>
3. Gate Smashers, *L-3.6: Direct Mapping with Example in Hindi*.
   <https://www.youtube.com/watch?v=eObN3u3eAnU>
4. Gate Smashers, *L-3.8: Fully Associative Mapping with Examples in Hindi*.
   <https://www.youtube.com/watch?v=sLCJJdz0WAg>
5. Gate Smashers, *L-3.10: Set Associative Mapping with Examples in Hindi*.
   <https://www.youtube.com/watch?v=pFndaJARM4Q>
6. Gate Smashers, *System Design Playlist* (contains *Lec-16: Write Through vs Write
   Back*). <https://www.youtube.com/playlist?list=PLxCzCOWd7aiHanm1nA5_fMwbx5dCNFkDX>
7. Gate Smashers, *Computer Organization and Architecture Roadmap*.
   <https://www.gatesmashers.com/roadmaps/computer-organization-architecture>

### Cache-Design Tutorials and Lectures

8. MIT 6.004 Computation Structures, *L25: Cache Coherence*.
   <https://www.youtube.com/watch?v=83jOKVb_HTM>
9. MIT 6.004 Computation Structures, *21.2.5 Cache Coherence*.
   <https://www.youtube.com/watch?v=ISaYWm8T8n4>
10. CMU 15-418/15-618, *Lecture 10: Snooping-Based Cache Coherence* (playlist).
    <https://www.youtube.com/playlist?list=PLpIxOj-HnDsO4Atvrp86c-4La9Mq3kMQZ>
11. CMU 15-418/15-618, *Cache Coherence* slides.
    <https://www.cs.cmu.edu/~418/lectures/10_cachecoherence.pdf>
12. Onur Mutlu, *Cache Design* lecture.
    <https://www.youtube.com/watch?v=BGaLNhyr-2Q>
13. Onur Mutlu, *Memory Hierarchy* lecture.
    <https://www.youtube.com/watch?v=1zXDoDDkICQ>
14. Onur Mutlu, *Cache Coherence* lecture.
    <https://www.youtube.com/watch?v=jcWemSeLiGo>

### Background Texts

15. D. Patterson and J. Hennessy, *Computer Organization and Design: The
    Hardware/Software Interface*, 5th ed., Morgan Kaufmann, 2014.
16. D. J. Sorin, M. D. Hill, and D. A. Wood, *A Primer on Memory Consistency and Cache
    Coherence*, 2nd ed., Springer, 2020.

All web links were checked on 22 November 2026.
