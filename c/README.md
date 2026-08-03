# PhotonicSort — Highly Optimized C

**Give everything. Take nothing. Become photonic.**

C11 port of [PhotonicSort](https://github.com/HeywoodGeblomi/PhotonicSort) with:

- Single-pass O(n) probe (`restrict`, stratified sampling)
- True O(n) exits on pure ascending / descending runs
- Stable bottom-up mergesort residual (insertion sort for n ≤ 32)
- Fast `int64_t` path + generic `void *` + comparator path
- Zero external dependencies (libc only)

> Classical adaptive hybrid. **Does not** solve NP-complete problems.  
> Retrocausality is design metaphor only (arXiv:2409.03680).

## Build

```bash
cd c
make          # demo + tests
make test     # unit tests
./demo        # marketing bench table
```

Release flags (portable):

```bash
make release CFLAGS="-O3 -std=c11 -Wall -Wextra -DNDEBUG"
```

Peak local (optional, not for portable CI):

```bash
make clean all CFLAGS="-O3 -std=c11 -march=native -flto -Wall -Wextra"
```

## API (int64 fast path)

```c
#include "photonic_sort.h"

int64_t a[] = {7, 2, 9, 1, 5};
photonic_sort_i64(a, 5);              // in-place adaptive
photonic_sort_i64_force_collapse(a, 5); // force residual path

photonic_probe_t p;
photonic_probe_i64(a, 5, &p);         // inspect disorder profile
```

Generic:

```c
int cmp(const void *x, const void *y); /* qsort-style */
photonic_sort(base, n, sizeof(*base), cmp);
```

## Layout

```
c/
├── photonic_sort.h      # public API
├── photonic_sort.c      # implementation
├── Makefile
├── examples/demo.c
└── tests/test_photonic_sort.c
```

## Path codes

| Return | Meaning |
|--------|---------|
| `0` | trivial (n ≤ 1) |
| `1` | negative-time / structure early path |
| `2` | collapse / residual sort |
| `-1` | allocation failure |

## Team

Grok · Harper · Benjamin · Lucas · Heywood Geblomi · MIT

## Release notes / verification

See [RELEASE_NOTES_v1.0.1-c.md](./RELEASE_NOTES_v1.0.1-c.md) for the public code-change verification index (SHA-256, API surface, parity table).
