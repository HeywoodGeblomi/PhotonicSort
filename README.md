# PhotonicSort

**Adaptive hybrid sorting library** — C11 performance core + Python reference implementation.

[![License: MIT](https://img.shields.io/badge/license-MIT-green.svg)](./LICENSE)
[![C11](https://img.shields.io/badge/C-11-blue.svg)](./c/)
[![Python](https://img.shields.io/badge/Python-3.10%2B-blue.svg)](./photonic_sort.py)
[![Version](https://img.shields.io/badge/version-1.0.1-informational.svg)](./BENCHMARKS.md)

PhotonicSort is a **classical** adaptive sorting algorithm: a single-pass disorder probe selects between structure-aware early exits and a stable residual sort. The production path is a **C11 library** (`c/`); Python provides a pure-stdlib reference and research harness.

| Property | Value |
|----------|-------|
| Correctness | Always correct; stable residual path |
| Worst case | O(n log n) |
| Best case | Near-linear on sorted / reverse / long-run input |
| C dependencies | libc only |
| Python dependencies | stdlib only |

**Not** optical hardware. **Not** a claim of light-speed sorting or P = NP. See [Non-claims](#non-claims).

**Technical site:** https://heywoodgeblomi.github.io/PhotonicSort/  
**Build:** [BUILD.md](./BUILD.md) · **Benchmarks:** [BENCHMARKS.md](./BENCHMARKS.md) · **Show HN package:** [SHOW_HN.md](./SHOW_HN.md)

---

## Architecture

```
PhotonicSort/
├── c/                      # C11 performance core (primary)
├── photonic_sort.py        # Python reference (stdlib)
├── docs/                   # GitHub Pages site (index.html)
├── BUILD.md
├── BENCHMARKS.md
├── RESEARCH.md
└── VERIFY.md
```

**Data path:** probe → structure early-exit → residual sort (stable mergesort in C).

---

## Build (quick start)

```bash
git clone https://github.com/HeywoodGeblomi/PhotonicSort.git
cd PhotonicSort/c
make && make test && ./demo
make release    # portable -O3 -DNDEBUG -flto
make static     # fully static for scratch/distroless
```

Docker: `docker build -t photonicsort . && docker run --rm photonicsort`  
Full guide: [BUILD.md](./BUILD.md).

---

## C11 API

```c
#include "photonic_sort.h"
int64_t a[] = {7, 2, 9, 1, 5, 3, 8, 4, 6, 0};
photonic_sort_i64(a, 10);
```

Details: [`c/README.md`](./c/README.md).

---

## Benchmarks

**C · n = 1,000,000 · `int64_t`** (best of 5, `-O3`)

| Pattern | PhotonicSort C | `std::sort` | Ratio |
|---------|---------------:|------------:|------:|
| Sorted | **0.019 ms** | 15.03 ms | ~800× |
| Reverse | **0.44 ms** | 9.34 ms | ~21× |
| Organpipe | **26.2 ms** | 89.0 ms | ~3.4× |
| Random | 105 ms | **78 ms** | `std::sort` faster |

Full tables: [BENCHMARKS.md](./BENCHMARKS.md).

---

## Non-claims

1. No photonic/optical processors.
2. Does not sort “at the speed of light.”
3. Does not prove P = NP.
4. Worst case remains **O(n log n)**.
5. Name is a design metaphor (arXiv:2409.03680) — [RESEARCH.md](./RESEARCH.md).

## License

MIT — [LICENSE](./LICENSE).
