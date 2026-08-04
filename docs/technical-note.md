# PhotonicSort: adaptive hybrid sorting (C11 + Python)

**PhotonicSort** is a classical adaptive sorting library. A single-pass disorder probe selects between structure-aware early exits and a stable residual sort. The production path is a **C11 library** (`libc` only). A pure-stdlib Python module is a reference implementation.

This is **not** optical hardware, **not** light-speed sorting, and **not** a P = NP claim. Worst case remains **O(n log n)**.

## Properties

| Property | Value |
|----------|-------|
| Correctness | Always correct; stable residual path |
| Worst case | O(n log n) |
| Best case | Near-linear on sorted / reverse / long-run input |
| C dependencies | libc only |
| Python dependencies | stdlib only |

## Algorithm

1. **Probe** — O(n) (or stratified sample): inversion ratio, max run, run count, direction changes, sortedness.
2. **Structure early-exit** — sorted → no-op; reverse → in-place reverse when eligible.
3. **Residual** — stable bottom-up mergesort (C) when no early exit applies.

## Benchmarks (C, n = 1e6, int64, -O3, best of 5)

| Pattern | PhotonicSort C | std::sort | Ratio |
|---------|---------------:|----------:|------:|
| Sorted | **0.019 ms** | 15.03 ms | ~800× |
| Reverse | **0.44 ms** | 9.34 ms | ~21× |
| Organpipe | **26.2 ms** | 89.0 ms | ~3.4× |
| Random | 105 ms | **78 ms** | std wins |

Correctness checked against `std::sort` every trial. Full methodology: [BENCHMARKS.md](https://github.com/HeywoodGeblomi/PhotonicSort/blob/main/BENCHMARKS.md).

## Architecture clarification

The C code is a **standalone C11 library**, not a Python C-extension. It does not release or interact with the Python GIL. Python and C are separate deliverables.

## Build

```bash
git clone https://github.com/HeywoodGeblomi/PhotonicSort.git
cd PhotonicSort/c && make && make test
```

Source: https://github.com/HeywoodGeblomi/PhotonicSort  
License: MIT
