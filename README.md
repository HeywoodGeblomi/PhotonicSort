# PhotonicSort

**Adaptive hybrid sorting library** — C11 performance core + Python reference.

[![License: MIT](https://img.shields.io/badge/license-MIT-green.svg)](./LICENSE)
[![C11](https://img.shields.io/badge/C-11-blue.svg)](./c/)
[![Version](https://img.shields.io/badge/version-1.3.1--c-informational.svg)](./c/RELEASE_NOTES_v1.3.1-c.md)

PhotonicSort is a **classical** adaptive sort: a GyroRank-style pilot probe selects structure early-exits or a residual talent menu (counting, insertion/pdq, run-merge, radix).

| Property | Value |
|----------|-------|
| Correctness | Always correct |
| Worst case | O(n log n) |
| Best case | Near-linear on sorted / reverse / long runs |
| C dependencies | libc only |
| Python dependencies | stdlib only |

**Not** optical hardware. **Not** light-speed sorting or P = NP. See [Non-claims](#non-claims).

**Technical site:** https://heywoodgeblomi.github.io/PhotonicSort/  
**Build:** [BUILD.md](./BUILD.md) · **Benchmarks:** [BENCHMARKS.md](./BENCHMARKS.md)

---

## Architecture

```
probe → structure early-exit → residual talent menu
         (sorted / reverse)     LOW_CARD | LOW_DISORDER | PATTERNED | RANDOM
```

| Route | Residual |
|-------|----------|
| STRUCTURE | O(n) early-exit / reverse |
| LOW_CARD | Counting sort (few distinct keys) |
| LOW_DISORDER | Cache-local insertion (n≤4k) / pdq |
| PATTERNED | Capacity-checked run merge |
| RANDOM | LSD radix (int64) / introsort |

---

## Build

```bash
git clone https://github.com/HeywoodGeblomi/PhotonicSort.git
cd PhotonicSort/c
make && make test && ./demo
```

Full guide: [BUILD.md](./BUILD.md).

---

## C11 API

```c
#include "photonic_sort.h"
int64_t a[] = {7, 2, 9, 1, 5, 3, 8, 4, 6, 0};
photonic_sort_i64(a, 10);   /* path code: 0 trivial, 1 structure, 2 residual */
```

Version string: `photonic_sort_version()` → `"1.3.1-c"`.

---

## Benchmarks (v1.3.1-c vs `std::sort`, n=1e6, `-O3 -march=native`)

| Pattern | PhotonicSort | `std::sort` | Speedup |
|---------|-------------:|------------:|--------:|
| Sorted | **0.60 ms** | 13.4 ms | **22×** |
| Reverse | **1.0 ms** | 7.8 ms | **8×** |
| Organpipe | **3.0 ms** | 88 ms | **30×** |
| Random | **33 ms** | 80 ms | **2.5×** |
| Sawtooth | **1.7 ms** | 24 ms | **14×** |
| Few-unique | **1.9 ms** | 24 ms | **13×** |
| Almost-sorted | **12 ms** | 16 ms | **1.3×** |

Plan A (v1.3) closed the previous gaps on sawtooth / few-unique / almost-sorted.

---

## Non-claims

1. No photonic/optical processors.
2. Does not sort “at the speed of light.”
3. Does not prove P = NP.
4. Worst case remains **O(n log n)**.
5. Name is a design metaphor (arXiv:2409.03680).

---

## Contributors

- **Heywood Geblomi** — design, adaptive hybrid architecture, release engineering
- Grok (xAI) — implementation collaboration (C11 core, residual menu, benchmarks)

---

## License

MIT — [LICENSE](./LICENSE).
