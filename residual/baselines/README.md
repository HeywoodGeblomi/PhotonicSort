# residual/baselines — Vendored Sort Headers

Pinned for zero-network money-shot builds. Do **not** curl at build time.

## Contents

| File | Source | Pin |
|------|--------|-----|
| `pdqsort.h` | [orlp/pdqsort](https://github.com/orlp/pdqsort) | master (exact content as of 2026-08-23) |
| `ska_sort.hpp` | [skarupke/ska_sort](https://github.com/skarupke/ska_sort) | master (exact content as of 2026-08-23) |

## Content hashes (SHA-256)

```
1916aff237fc7ecc638dec6397662334552a1b6f823fa65011a8dfe831237fc0  pdqsort.h
440456435f9052998bd48fbc501870ce3975bcff400b906783768694f37264ca  ska_sort.hpp
```

## Usage

- Hybrid residual path includes `"ska_sort.hpp"` / `"pdqsort.h"`.
- Money-shot Docker (`reproduce/Dockerfile.sp`) COPYs `residual/` only and expects these under `residual/baselines/` (or copies into residual/ for include path).
- Pure residual path remains self-contained.

Pinned 2026-08-23. EXTERNAL-clean. THE BEASTIE BOYZ.
