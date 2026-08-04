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

---

## Architecture

```
PhotonicSort/
├── c/                      # C11 performance core (primary)
│   ├── photonic_sort.h     # public API
│   ├── photonic_sort.c     # probe + early-exit + residual
│   ├── Makefile
│   ├── examples/demo.c
│   └── tests/
├── photonic_sort.py        # Python reference (stdlib)
├── tests/                  # Python unit tests
├── benchmarks/             # measured timings + environment
├── BENCHMARKS.md           # C vs std::sort, Python vs Timsort
├── RESEARCH.md             # naming metaphor + citations
└── VERIFY.md               # integrity / SHA-256 procedures
```

**Data path (both languages)**

1. **Probe** — one O(n) (or stratified-sample) pass: inversion ratio, max run, run count, direction changes, sortedness.
2. **Structure early-exit** — fully sorted → no-op; fully reverse → in-place reverse; other high-structure cases take a near-linear path when eligible.
3. **Residual sort** — stable bottom-up mergesort (C) or Timsort/`sorted` (Python) when the probe does not authorize an early exit.

Path codes (C): `0` trivial · `1` structure early path · `2` residual · `-1` allocation failure.

---

## C11 core (recommended)

```bash
cd c
make            # build demo + tests
make test
./demo
```

### Fast path (`int64_t`)

```c
#include "photonic_sort.h"

int64_t a[] = {7, 2, 9, 1, 5, 3, 8, 4, 6, 0};
photonic_sort_i64(a, 10);           /* in-place adaptive */

photonic_probe_t p;
photonic_probe_i64(a, 10, &p);      /* inspect disorder profile */
```

### Generic path

```c
int cmp(const void *x, const void *y);   /* qsort-style */
photonic_sort(base, n, sizeof(*elem), cmp);
```

| API | Role |
|-----|------|
| `photonic_probe_i64` / `photonic_probe_generic` | Disorder profile |
| `photonic_sort_i64` | In-place adaptive `int64_t` |
| `photonic_sort_i64_force_collapse` | Force residual path |
| `photonic_sort_i64_copy` | Out-of-place |
| `photonic_sort` | Generic `void *` + comparator |

Full notes: [`c/README.md`](./c/README.md) · release index: [`c/RELEASE_NOTES_v1.0.1-c.md`](./c/RELEASE_NOTES_v1.0.1-c.md).

---

## Benchmarks

Measured on Linux x86_64, `-O3`, best of 5. Correctness checked against `std::sort` each trial.

**C · n = 1,000,000 · `int64_t`**

| Pattern | PhotonicSort C | `std::sort` | Ratio |
|---------|---------------:|------------:|------:|
| Sorted | **0.019 ms** | 15.03 ms | ~800× |
| Reverse | **0.44 ms** | 9.34 ms | ~21× |
| Organpipe | **26.2 ms** | 89.0 ms | ~3.4× |
| Random | 105 ms | **78 ms** | `std::sort` faster |

Structure early-exits explain the sorted/reverse wins. On unstructured random input, a tuned introsort remains competitive — expected for adaptive designs.

Full tables, Python vs Timsort honesty panel, CSV, and host notes: **[BENCHMARKS.md](./BENCHMARKS.md)**.

---

## Python reference

```bash
python photonic_sort.py
python -m unittest discover -s tests -v
```

```python
from photonic_sort import photonic_sort, photonic_probe

data = [7, 2, 9, 1, 5, 3, 8, 4, 6, 0]
print(photonic_sort(data))
# [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]

print(photonic_probe(data))
```

CPython’s `list.sort` (Timsort) is native C and will typically beat pure-Python PhotonicSort on bulk paths. Prefer the **C11 library** for performance work.

---

## Integrity

| Document | Purpose |
|----------|---------|
| [VERIFY.md](./VERIFY.md) | Functional + digest checks |
| [SHA256_VERIFY_COMMANDS.md](./SHA256_VERIFY_COMMANDS.md) | Shell / PowerShell commands |
| [scripts/verify-sha256.sh](./scripts/verify-sha256.sh) | Automated verifier |
| [SHA256SUMS_v1.0.1-c.txt](./SHA256SUMS_v1.0.1-c.txt) | C release digests |

---

## Non-claims

1. PhotonicSort does **not** use photonic or optical processors.
2. It does **not** sort “at the speed of light” or bypass ordinary RAM/CPU limits.
3. It does **not** prove P = NP or solve NP-complete problems.
4. Worst-case complexity remains **O(n log n)**.
5. The name and research notes refer to a **design metaphor** drawn from published weak-value / negative group-delay photon experiments (arXiv:2409.03680), not to physical retrocausality in software. Details: [RESEARCH.md](./RESEARCH.md).

---

## Related projects

| Project | Role |
|---------|------|
| [GeblomiSort](https://github.com/HeywoodGeblomi/GeblomiSort) | C++20 production 1-D hybrid (probe / pdqsort / ska / Verge) |
| ImplosionSort (2-D+) | Multi-dimensional disposition research |

---

## License

MIT — see [LICENSE](./LICENSE).
