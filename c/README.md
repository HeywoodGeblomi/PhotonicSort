# PhotonicSort — C11 core

Performance implementation of PhotonicSort: adaptive hybrid sort with a single-pass probe, structure-aware early exits, and a stable residual path.

- Single-pass O(n) probe (`restrict`, stratified sampling)
- O(n) exits on pure ascending / descending input
- Stable bottom-up mergesort residual (insertion sort for n ≤ 32)
- Fast `int64_t` path + generic `void *` + comparator path
- Dependencies: **libc only**

Version: **1.0.1-c** · License: MIT

Classical algorithm only. Does not claim photonic hardware, light-speed sorting, or P = NP. Naming metaphor: [RESEARCH.md](../RESEARCH.md).

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

Optional local peak (not for portable CI):

```bash
make clean all CFLAGS="-O3 -std=c11 -march=native -flto -Wall -Wextra"
```

## API

### `int64_t` fast path

```c
#include "photonic_sort.h"

int64_t a[] = {7, 2, 9, 1, 5};
photonic_sort_i64(a, 5);                 /* in-place adaptive */
photonic_sort_i64_force_collapse(a, 5);  /* force residual */

photonic_probe_t p;
photonic_probe_i64(a, 5, &p);            /* disorder profile */
```

### Generic path

```c
int cmp(const void *x, const void *y);   /* qsort-style */
photonic_sort(base, n, sizeof(*base), cmp);
```

## Layout

```
c/
├── photonic_sort.h
├── photonic_sort.c
├── Makefile
├── examples/demo.c
└── tests/test_photonic_sort.c
```

## Path codes

| Return | Meaning |
|--------|---------|
| `0` | trivial (n ≤ 1) |
| `1` | structure early path |
| `2` | residual sort |
| `-1` | allocation failure |

## Benchmarks and verification

- Root [BENCHMARKS.md](../BENCHMARKS.md) — C vs `std::sort`
- [RELEASE_NOTES_v1.0.1-c.md](./RELEASE_NOTES_v1.0.1-c.md) — SHA-256 index and API surface
