# PhotonicSort

**Adaptive hybrid sorting library** — C11 performance core + Python reference + Rust FFI bindings.

[![License: MIT](https://img.shields.io/badge/license-MIT-green.svg)](./LICENSE)
[![C11](https://img.shields.io/badge/C-11-blue.svg)](./c/)
[![Rust](https://img.shields.io/badge/Rust-FFI-orange.svg)](./rust/)
[![Version](https://img.shields.io/badge/version-1.3.2--c-informational.svg)](./c/RELEASE_NOTES_v1.3.2-c.md)
[![Vector-2](https://img.shields.io/badge/Vector--2-MET-brightgreen.svg)](./docs/vector2/VECTOR2_CLAIM_v0.1.md)
[![Phase-2](https://img.shields.io/badge/Phase%202-frozen-blue.svg)](./docs/phase2/PHASE2_CYCLE_COMPLETE.md)
[![Release](https://img.shields.io/badge/release-v1.4.0--c-informational.svg)](https://github.com/HeywoodGeblomi/PhotonicSort/releases/tag/v1.4.0-c)

**Primary practical sorter of THE BEASTIE BOYZ.** Classical adaptive hybrid: GyroRank-style pilot probe selects structure early-exits or a residual talent menu.

---

## Vector-2 Status — Domination Suite v0.1

**Criteria MET** (2026-08-09).

| Criterion | Threshold | Measured | Result |
|-----------|-----------|----------|--------|
| Geometric mean vs `std::sort` | ≥ 1.5× | **7.08×** | PASS |
| Geometric mean vs best specialized (pdqsort / ska_sort) | ≥ 1.5× | **1.94×** | PASS |
| Minimum speedup vs `std::sort` | ≥ 1.0× | **1.32×** | PASS |
| Major regressions vs specialized | 0 | **0** | PASS |

> **Scope (mandatory):** On Domination Suite v0.1, the locked Vector-2 criteria are met (geo vs std ≈ 7.1×, geo vs best specialized ≈ 1.9×, min ≥ 1.3×, zero major regressions). Structured / patterned / low-card wins remain pure-C Photonic. High-entropy residual uses library-strength pdqsort / ska_sort after correct selection from visible probe metrics. Result is scoped to this suite; broader evaluation may differ. No asymptotic claim.

**Full claim document:** [docs/vector2/VECTOR2_CLAIM_v0.1.md](./docs/vector2/VECTOR2_CLAIM_v0.1.md)

The pure-C residual menu is the **self-contained default** shipped in the C11 library (no external sort dependencies). The Vector-2 numbers above are the measured hybrid configuration: pure-C structured / patterned / low-card paths + probe-selected library residual on high-entropy inputs. This is valid engineering selection, not a claim that the pure-C residual alone beats library pdqsort/ska_sort on pure random.

---

## Phase 2 residual freeze (pure adaptive residual excellence)

**Status: Complete and Frozen** (2026-08-10). Systems milestone on the pure residual path. Release: [v1.4.0-c](https://github.com/HeywoodGeblomi/PhotonicSort/releases/tag/v1.4.0-c).

| Metric (n=1e6, pure / pdqsort) | Result |
|--------------------------------|-------:|
| Full-suite geo-mean | **0.61×** (ahead overall) |
| almost_* family | **0.36–0.42×** |
| Class 1 (structured almost) | **0.41–0.52×** |
| STRUCTURE | **0.53–0.59×** |
| uniform_i64 | **1.07×** |
| HE residual vs ska (isolated) | **1.10×** (≤1.15× target **MET**) |
| equal_heavy | **1.43×** |
| adversarial_pivot / db_pk_sparse | ~2.1× / ~1.7× residual floors |

Residual menu additions: Class 1 (`rsl_structured_correct`), buffer MSD (INS=192), identity-almost, majority residual.

**Not a field-level breakthrough.** See [`NON_CLAIMS.md`](./NON_CLAIMS.md).  
Docs: [`docs/phase2/`](./docs/phase2/) · [`c/RELEASE_NOTES_v1.4.0-c.md`](./c/RELEASE_NOTES_v1.4.0-c.md)

---

## Architecture

```
probe → structure early-exit → residual talent menu
         (sorted / reverse)     LOW_CARD | LOW_DISORDER | PATTERNED | RANDOM | Class 1
```

| Route | Residual (shipped pure-C default) |
|-------|-----------------------------------|
| STRUCTURE | O(n) early-exit / reverse (**verified**) |
| LOW_CARD | Counting sort (few distinct keys) |
| LOW_DISORDER | Cache-local insertion / internal pdq-class / identity-almost |
| PATTERNED | Capacity-checked run merge |
| Class 1 | `rsl_structured_correct` (backbone + islands) |
| RANDOM | LSD / buffer MSD (int64) / introsort |

### Sort modes (v1.3.2-c)

| Mode | Intent |
|------|--------|
| `NORMAL` | Production baseline (default) |
| `AGGRESSIVE` | Widened early-exit thresholds |
| `FORCE_HOLE` | Max hole-in-one attempt (opt-in) |

STRUCTURE O(n) verification remains mandatory on every mode.

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
photonic_sort_i64(a, 10);   /* NORMAL (default) */
photonic_sort_i64_ex(a, 10, PHOTONIC_MODE_AGGRESSIVE);
photonic_sort_i64_ex(a, 10, PHOTONIC_MODE_FORCE_HOLE);
```

Version string: `photonic_sort_version()` → `"1.3.2-c"`.

---

## Rust

Safe FFI bindings over the C11 core live in [`rust/`](./rust/):

```bash
cd rust && cargo test -p photonic-sort
```

| Crate | Role |
|-------|------|
| [`photonic-sort`](./rust/photonic-sort) | Safe API (`sort_i64`, `probe_i64`, `PathCode`) |
| [`photonic-sort-sys`](./rust/photonic-sort-sys) | Low-level `extern "C"` + vendored C build |

See [rust/README.md](./rust/README.md) and [rust/PUBLISH.md](./rust/PUBLISH.md).

---

## Benchmarks

### A. Pure-C PhotonicSort (self-contained residual menu)

v1.3.1-c / v1.3.2-c vs `std::sort`, n=1e6, `-O3`:

| Pattern | PhotonicSort | `std::sort` | Speedup |
|---------|-------------:|------------:|--------:|
| Sorted | **0.37 ms** | 10.62 ms | **~29×** |
| Reverse | **0.78 ms** | 7.15 ms | **~9×** |
| Organpipe | **2.47 ms** | 81.3 ms | **~33×** |
| Sawtooth | **1.59 ms** | 23.2 ms | **~15×** |
| Few-unique | **1.52 ms** | 19.7 ms | **~13×** |
| Almost-sorted | **2.97 ms** | 10.84 ms | **~3.6×** |
| Random | **46.7 ms** | 77.7 ms | **~1.7×** |

Geometric mean of the 7 ratios ≈ **10×**. Structure early-exit is mandatory O(n) verified. Full details + mode panel: [BENCHMARKS.md](./BENCHMARKS.md).

### B. Domination Suite v0.1 — Hybrid residual configuration (Vector-2 numbers)

- Structured / patterned / low-card: pure-C Photonic
- High-entropy (random / adversarial): probe selection → library pdqsort / ska_sort

| Metric | Value |
|--------|-------|
| Geo mean vs `std::sort` | **7.08×** |
| Geo mean vs best specialized | **1.94×** |
| Min vs `std::sort` | **1.32×** |
| Major regressions vs specialized | **0** |

Full protocol, pattern list, and non-claims: [docs/vector2/VECTOR2_CLAIM_v0.1.md](./docs/vector2/VECTOR2_CLAIM_v0.1.md).

### C. Phase 2 pure residual freeze (vs pdqsort)

See [BENCHMARKS.md](./BENCHMARKS.md) Phase 2 section and [`docs/phase2/`](./docs/phase2/). Full-suite geo-mean **0.61×** vs pdqsort at n=1e6. **Not a field-level breakthrough.**

---

## Non-claims

**Full statement:** [`NON_CLAIMS.md`](./NON_CLAIMS.md).

1. No photonic/optical processors.
2. Does not sort “at the speed of light.”
3. Does not prove P = NP.
4. Worst case remains **O(n log n)**.
5. Name is a design metaphor (arXiv:2409.03680).
6. ForceHole does **not** claim hole-in-one on every input.
7. The pure-C residual menu does not claim to beat library pdqsort / ska_sort on pure high-entropy data; the Vector-2 result uses library residual after selection on those patterns.
8. Phase 2 residual freeze is **not a field-level breakthrough** (ska remains stronger on pure HE; residual floors remain on some patterns).

---

## Contributors

- **Heywood Geblomi** — design, adaptive hybrid architecture, release engineering
- Grok (xAI) + THE BEASTIE BOYZ (Harper, Lucas, Benjamin) — implementation collaboration (C11 core, residual menu, Domination Suite, Vector-2 claim, Phase 2 residual freeze)

---

## License

MIT — [LICENSE](./LICENSE).
