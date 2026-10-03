# SiTAR — Working Reference

A dense handoff for writing and debugging SiTAR models. Everything here is verified
against the kernel source in `Projects/sitar/core/` and against actual simulation runs.

---

## 1. What SiTAR is

A **cycle-based discrete-event simulator** for architectural models. You write a `.sitar`
file describing modules; SiTAR translates it to C++, compiles it against its simulation
kernel, and produces an executable.

Three steps, deliberately separate so you can inspect each:

```bash
sitar translate model.sitar    # .sitar  ->  C++ in ./Output/
sitar compile                 # C++     ->  ./sitar_sim
./sitar_sim 2000              # run, argument = max cycles
```

**Both commands are relative to your current working directory.** `scripts/sitar` reads its
config from `os.getcwd()` and the compile step reads `./Output`. Always run them from the
directory holding your `.sitar` file.

Requires `sitar` on `PATH` and `scons` installed. Verify with `sitar -h`.

---

## 2. Skeleton — start here

A complete, working model. Copy this and edit.

```sitar
module Top
    submodule P : Producer<>
    submodule C : Consumer<>

    net n_data : capacity 1 width 4

    P.outp => n_data
    C.inp  <= n_data

    behavior
        wait(5, 0);
        stop simulation;
    end behavior
end module

module Producer
    parameter int W = 4

    outport outp : width W

    include $#include <vector>$
    decl $
    token<W> t;
    int v;
    bool ok;
    $

    behavior
        do
            wait until (this_phase == 1);      // PUSH in phase 1
            $v = 42; sitar::pack(t, v); ok = outp.push(t);$;
            if (not ok) then
                wait
            end if;
            wait(1, 0);
        while (1) end do;
    end behavior
end module

module Consumer
    parameter int W = 4

    inport inp : width W

    decl $
    token<W> t;
    int v;
    bool hasToken;
    $

    behavior
        do
            wait until (this_phase == 0);      // PULL in phase 0
            $hasToken = inp.peek(t);$;
            if (hasToken) then
                $inp.pull(t); sitar::unpack(t, v);
                log << endl << "got " << v;$;
            else
                wait                           // nothing yet: yield, don't spin
            end if;
        while (1) end do;
    end behavior
end module
```

Expected output of `./sitar_sim 6`:

```
(1,0)TOP.C      :got 42
(2,0)TOP.C      :got 42
(3,0)TOP.C      :got 42
(4,0)TOP.C      :got 42
Simulation stopped at time (5,0)
```

One receive per cycle, always at **phase 0**. That is the shape to copy.

!!! gotcha "Never put a `pull` after a `wait`"
    A `wait` inside an `if` only suspends *that branch*. When its timer expires the branch
    ends and execution **falls through to the next statement** — so a `pull` placed after the
    `if` runs even when no token arrived. `pull` returns `false` and leaves the token
    untouched, so if you ignore the return value you silently unpack **stale data** and log
    phantom receives. Either guard the `pull` inside the true branch as above, or use the
    `do ... while (not hasToken) end do;` form, which only exits the loop on success.

Note there is no module called `Top` *and* no `end module` for `Top` semantics you need to
worry about — one `Top` per file, instantiated by the kernel as `TOP`.

---

## 3. The time model — the core mental model

Time is a `(cycle, phase)` pair. Every cycle has exactly two phases:

```
(0,0) -> (0,1) -> (1,0) -> (1,1) -> (2,0) -> (2,1) -> ...
```

Internally a single integer: **each cycle = 2 units, each phase = 1 unit.**
`cycle() = t >> 1`, `phase() = t & 1`.

Consequences that will bite you:

| Expression | Resumes at |
|---|---|
| `wait(2,0)` from `(3,1)` | `(5,1)` — phases carry into the cycle count |
| `wait(3,1)` from `(2,1)` | `(6,0)`, not `(5,2)` — there is no phase 2 |

### The Two-Phase Rule

> **push only in phase 1. pull only in phase 0.**

SiTAR does **not** enforce this at runtime. Violating it silently produces
order-dependent, non-deterministic results. Follow it and module ordering stops mattering.

### Round-trip latency budget (measure this before you blame your model)

A request/response exchange over two nets costs **2 extra cycles on top of the responder's
internal service time**:

```
push request  --1c-->  responder receives  --SERVICE-->  responder replies  --1c-->  you observe
```

Verified: with `DELAY_HIT=4`, a `waitEnd - waitStart` measured at the requester comes out
as **6**, not 4.

So if you measure latency end-to-end and compare it to the responder's configured delay,
subtract 2. If instead you want the responder's own access time, measure inside the
responder, or subtract the constant with a comment naming where the 2 comes from.

### Execution order within a phase

The docs say it is "not guaranteed". In the serial kernel it is concrete and worth knowing:

`core/sitar_module.cpp:51` iterates `_submodules`, a `std::map` keyed by instance name — so
**submodules run alphabetically**, parent before child. If a receiver's name sorts before a
sender's (`C` before `P`), the receiver cannot see a token pushed in the *same* phase, and
needs one extra cycle. This is invisible in the `.sitar` source; check the instance names.

---

## 4. Language reference

### Module body — what may appear, in any order

| Construct | Syntax |
|---|---|
| parameter | `parameter int N = 4` — types `int`, `char`, `bool`; default required |
| submodule | `submodule a, b : Worker` |
| submodule array | `submodule_array n[8] : Worker` |
| submodule (templated) | `submodule a : Foo<10, 'x', 1>` — positional, trailing may be omitted |
| net | `net n : capacity 2 width 4` — **width is in BYTES**; omit for a pure signal |
| net array | `net_array n[8] : capacity 1 width 4` |
| inport / outport | `inport i : width 4` |
| connection | `sender.outp => n` and `receiver.inp <= n` (interchangeable) |
| procedure instance | `procedure p : Proc` |
| header inject | `include $#include <vector>$` |
| member declaration | `decl $int x; std::vector<int> v;$` |
| constructor init | `init $x = 0;$` |
| behavior | `behavior ... end behavior` |

Parameters become C++ template parameters, so they must be compile-time constants. `float`/
`double` are **not** valid parameter types — declare a `float` in `decl` and have the
parent set it in `init`.

### Code blocks — four positions

| Form | Goes into |
|---|---|
| `$...$` inside `behavior` | the module's `run` function (instantaneous statement) |
| `include $...$` | file header, before the class |
| `decl $...$` | the class body (members) |
| `init $...$` | the class constructor |

`include` and `decl` at module scope take **no** trailing semicolon; inside `behavior` they
do. `init $...$;` inside `behavior` still lands in the constructor — it does **not** run
mid-simulation.

### Statements

```sitar
wait;                       // advance one phase  ( == wait(0,1) )
wait(4, 0);                 // advance 4 cycles
wait until (this_phase == 1);
wait until (this_cycle >= 10);
wait until (current_time >= time(5,0));

if (cond) then  ... [else ...] end if;

do  ... while (cond) end do;

[  stmt; || stmt; || stmt; ];          // parallel block

for i in 0 to (N-1)  <connection statements>  end for

run my_proc;                            // invoke a procedure

$ arbitrary_c++;$                        // atomic C++ statement

stop simulation;                         // halt everything at end of phase
stop behavior;                           // halt this module only
```

`stop simulation` only halts at the end of the current phase — statements after it in the
same phase still run.

### Condition expressions

Parsed by SiTAR's own grammar, a strict subset of C++:

- comparison `== != < > <= >=`
- arithmetic `+ - * / %` and unary `-`
- logical: **`and`, `or`, `not`** — *not* `&&`, `||`, `!`
- atoms: integer literals, identifiers, `this_cycle`, `this_phase`, calls, parens
- anything else: wrap in `$...$`

```sitar
if (x > 0 and y < 10) then ... end if;
if ($ptr != nullptr$) then ... end if;
```

### Nets and tokens

Nets are passive FIFO buffers with **no latency parameter**. `width` is bytes.

```sitar
net n : capacity 4            // token<>     pure signal
net n : capacity 4 width 4    // token<4>    one int
net n : capacity 4 width 8    // token<8>    two ints, or one double
```

Widths must agree across net, both ports, and the token type. Mismatches are compile errors.

| Call | Returns | Notes |
|---|---|---|
| `outp.push(t)` | `true` if space | non-blocking, best-effort |
| `inp.pull(t)` | `true` if a token was there | pops the oldest |
| `inp.peek(t)` | `true` if non-empty | **does not consume** |
| `inp.numTokens()` | count | |
| `outp.full()` | `true` if at capacity | |

### pack / unpack

Serializes typed values into the token's byte payload.

```sitar
$int a; float b; sitar::pack(t, a, b);$      // token<8>
$sitar::unpack(t, a, b);$
```

**The `static_assert` requires `sizeof(args...) == W` exactly.** Not "at most" — exactly. Get
it wrong and you get a compile error, not corruption. Zero-width tokens need no pack.

Tokens carry `ID` (`uint64_t`) and `type` (`uint8_t`) fields alongside the payload. Use
`ID` as a push timestamp to model delays larger than one cycle:

```sitar
$outp.push(t); t.ID = this_cycle;$;
...
$if (inp.peek(t) and this_cycle >= (t.ID + delay)) inp.pull(t);$
```

---

## 5. Built-ins available inside any `$...$` block

| Name | Type | Meaning |
|---|---|---|
| `current_time` | `sitar::time` | streams as `(cycle,phase)` |
| `this_cycle` | `uint64_t` | current cycle |
| `this_phase` | `bool` | current phase, 0 or 1 |
| `log` | `sitar::logger` | auto-prefixed ostream |
| `stop simulation` / `stop behavior` | statement | — |
| `instanceId()` | `string` | local name, e.g. `"a"` |
| `hierarchicalId()` | `string` | full path, e.g. `"TOP.sys.a"` |
| `parent()` | `module*` | structural parent |
| `getInfo()` | `string` | recursive structure dump |

`log << endl` emits a newline **and** the `(cycle,phase)TOP.path:` prefix. Without `endl`,
output continues on the same line with no new prefix. Use that to group related output.

`sitar::time` also has `cycle()`, `phase()`, `toUint64()`, `toString()`, and full
comparison/arithmetic operators.

---

## 6. Gotchas — the ones that cost real time

**Token width is bytes, not bits.** `width 6` carries a 6-**byte** payload, not 6 bits.

**pack/unpack is an exact-size `static_assert`.** Mismatch = compile error.

**push in phase 1, pull in phase 0.** Not enforced. Violating it gives results that depend
on submodule name ordering, which looks like a random bug.

**A full round trip costs +2 cycles.** See §3. Budget for it before concluding your delay
parameter is wrong.

**Submodules run alphabetically** in the serial kernel. Rename `C` to `Z` and your timing
can change. This is a real, reproducible effect, not a race.

**`SITAR_ITERATION_LIMIT` (1000) counts loop iterations per phase.** An infinite
`while (1)` with no `wait` inside, or a `wait until` whose condition never becomes true,
trips it and kills the simulation with `ERROR:Iteration limit exceeded`. Every loop needs a
path that either advances time or exits.

**A `wait` inside an `if` only suspends that branch — code after the `if` still runs.** A
`pull` written after an `if (not hasToken) then wait end if;` executes on *every* pass.
`pull` returns `false` on an empty net and leaves the token untouched, so ignoring the return
value unpacks **stale payload** and logs phantom receives. Guard the `pull` inside the true
branch, or use the `do ... while (not hasToken) end do;` form, which only exits on success.
This one produced a convincing but entirely fictional trace during development.

**`sitar translate` exits 0 on failure.** A missing input file prints `Translation Failed`
and returns **success**. So a shell script that keeps going will happily rebuild and run the
**stale** binary, and you'll read old results thinking your edit did nothing. Put `set -e` in
any wrapper script, or check for `Output/` being rewritten.

**Bash: no spaces around `=` in assignments.** `FILE = $1` parses as a *command* named
`FILE` with arguments `=` and `$1` → `FILE: command not found`, and `$FILE` stays empty.
Write `FILE=$1`.

**`init` is not a behavioral statement.** `init $...$;` inside `behavior` lands in the
constructor and runs before simulation. Use a plain code block for mid-simulation resets.

---

## 7. Debugging workflow

1. **Read the log prefix.** `(3,1)TOP.sys.producer:` tells you the exact cycle, phase, and
   instance for every line. You almost never need to add instrumentation.
2. **Trace the token by hand.** Pick one `push`, note its `(cycle,phase)`. The `pull` will
   be in a *later* phase. Check the gap against your delay parameter plus the +2.
3. **Read the generated C++** in `Output/<Module>.h`. Each source statement becomes a
   `case` in a `switch` over a `_pointer[]` array, with `wait` compiled to
   `_timer[i] = current_time + sitar::time(c, p);`. This makes the phase arithmetic explicit
   and is the fastest way to settle timing questions.
4. **Sanity-check with extreme parameters.** Set the hit ratio to 100 and to 0; the
   measured value should land on the two endpoints. If it doesn't, the measurement — not the
   model — is wrong.

---

## 8. Further reading

Full documentation with runnable examples lives in `Projects/sitar/docs/docs_source/`:

| Page | Covers |
|---|---|
| `2_basic_concepts/timing_and_execution_model.md` | phases, waits, the two-phase rule |
| `2_basic_concepts/io_and_tokens.md` | nets, tokens, pack/unpack |
| `2_basic_concepts/module_behavior.md` | convergence and execution state |
| `2_basic_concepts/execution_model.md` | the simulation kernel step by step |
| `3_language_and_examples/*.md` | one page per statement type |
| `4_examples/processor_model.md` | a full processor + memory model |
| `cli_reference.md` | every `translate` / `compile` flag |

Ready-to-run models covering every feature: `Projects/sitar/examples/`.