# PhotonicSort — C11 pure residual path

**Give everything. Take nothing. Become photonic.**

C11 adaptive hybrid with a pure residual menu (libc only).  
Breakthrough Phase 2 residual freeze applied.

> Classical adaptive hybrid. **Does not** solve NP-complete problems.  
> See repository root [`NON_CLAIMS.md`](../NON_CLAIMS.md) before citing numbers.

---

## Residual menu (frozen)

| Route | Code | Residual |
|-------|-----:|----------|
| STRUCTURE | 0 | O(n) verify + early exit |
| PATTERNED | 1 | Run-merge / patterned |
| RANDOM / HE | 2 | Buffer MSD radix (INS = 192) |
| LOW_CARD | 3 | Counting sort (range-guarded) |
| LOW_DISORDER | 4 | Insertion / pdq-class residual |
| STRUCTURED_ALMOST (Class 1) | 5 | `rsl_structured_correct` |

**Phase 2 residual excellence (int64 path):**

- High-entropy: buffer MSD, base threshold **192**
- Class 1: island residual (O(1) island list ≤ 64)
- Structured / almost paths remain pure-C (no external library calls)

---

## Headline numbers (n = 1e6, pure adaptive residual path)

| Metric | pure / pdqsort |
|--------|---------------:|
| Full-suite geo-mean | **0.61×** |
| almost_* family | **0.36–0.42×** |
| Class 1 | **0.41–0.52×** |
| STRUCTURE | **0.53–0.59×** |
| uniform_i64 | **1.07×** |
| HE residual vs ska (isolated) | **1.10×** (target MET) |

**Not a field-level breakthrough** — see `NON_CLAIMS.md`.

Full tables: [`BENCHMARKS.md`](../BENCHMARKS.md) · [`docs/phase2/`](../docs/phase2/)

---

## Build

```bash
cd c
make && make test && ./demo
```

## API (int64 fast path)

```c
#include "photonic_sort.h"
int64_t a[] = {7, 2, 9, 1, 5};
photonic_sort_i64(a, 5);
```

## Version

C residual path: **v1.3.2-c** (code version string).  
Phase 2 residual freeze is a systems milestone; packaging tag: **v1.4.0-c**.

## Team

THE BEASTIE BOYZ · Grok · Harper · Benjamin · Lucas · Heywood Geblomi · MIT
