# PhotonicSort — C11 core

Adaptive hybrid sort: single-pass probe → structure early-exit → residual talent menu.

- Single-pass O(n) probe (`restrict`, stratified sampling)
- O(n) exits on pure ascending / descending input (**STRUCTURE verify mandatory**)
- Residual menu: LOW_CARD counting · LOW_DISORDER insertion/pdq · run-merge · radix/pdq
- Fast `int64_t` path + generic `void *` + comparator path
- Dependencies: **libc only**

Version: **1.3.2-c** · License: MIT

Classical algorithm only. Does not claim photonic hardware, light-speed sorting, or P = NP.

## Sort modes (v1.3.2-c)

| Mode | Intent |
|------|--------|
| `PHOTONIC_MODE_NORMAL` | Production baseline (default, safety-first) |
| `PHOTONIC_MODE_AGGRESSIVE` | Widened LOW_DISORDER / LOW_CARD thresholds |
| `PHOTONIC_MODE_FORCE_HOLE` | Max hole-in-one attempt (structure → low-card → residual) |

Default is **NORMAL**. ForceHole is opt-in only. STRUCTURE O(n) verification is mandatory on every mode.

```c
photonic_sort_i64(a, n);                                    /* NORMAL */
photonic_sort_i64_ex(a, n, PHOTONIC_MODE_AGGRESSIVE);
photonic_sort_i64_ex(a, n, PHOTONIC_MODE_FORCE_HOLE);
photonic_sort_set_mode(PHOTONIC_MODE_AGGRESSIVE);           /* process-wide */
```

## Build

```bash
cd c
make          # demo + tests
make test
./demo
```

Release flags (portable):

```bash
make release CFLAGS="-O3 -std=c11 -Wall -Wextra -DNDEBUG"
```

## API (int64 fast path)

```c
#include "photonic_sort.h"

int64_t a[] = {7, 2, 9, 1, 5};
photonic_sort_i64(a, 5);                 /* in-place adaptive (current mode) */
photonic_sort_i64_ex(a, 5, PHOTONIC_MODE_FORCE_HOLE);
photonic_sort_i64_force_collapse(a, 5);  /* force residual mergesort */

photonic_probe_t p;
photonic_probe_i64(a, 5, &p);            /* disorder profile */
photonic_probe_i64_ex(a, 5, &p, PHOTONIC_MODE_AGGRESSIVE);
```

## Path codes

| Return | Meaning |
|--------|---------|
| `0` | trivial (n ≤ 1) |
| `1` | structure early-exit (verified) |
| `2` | residual sort |
| `-1` | allocation failure |

## Layout

```
c/
├── photonic_sort.h
├── photonic_sort.c          # single-file body (29 596 B)
├── Makefile
├── SHA256SUMS
├── RELEASE_NOTES_v1.3.2-c.md
├── examples/demo.c
└── tests/test_photonic_sort.c
```

## Benchmarks and verification

- Root [BENCHMARKS.md](../BENCHMARKS.md)
- [RELEASE_NOTES_v1.3.2-c.md](./RELEASE_NOTES_v1.3.2-c.md)
- Local sensitivity: **0/57 fails** (n=200k)
