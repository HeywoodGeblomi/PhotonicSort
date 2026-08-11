# PhotonicSort

**Adaptive hybrid sorting library** — C11 performance core + Python reference + Rust FFI bindings.

[![License: MIT](https://img.shields.io/badge/license-MIT-green.svg)](./LICENSE)
[![C11](https://img.shields.io/badge/C-11-blue.svg)](./c/)
[![Rust](https://img.shields.io/badge/Rust-FFI-orange.svg)](./rust/)
[![Vector-2](https://img.shields.io/badge/Vector--2-MET-brightgreen.svg)](./docs/vector2/VECTOR2_CLAIM_v0.1.md)
[![Harvest](https://img.shields.io/badge/pure%20residual%20harvest-GATES%20MET-brightgreen.svg)](./docs/harvest/)
[![Release](https://img.shields.io/badge/release-v1.4.0--c-informational.svg)](https://github.com/HeywoodGeblomi/PhotonicSort/releases/tag/v1.4.0-c)

**Primary practical sorter of THE BEASTIE BOYZ.** Classical adaptive hybrid: probe selects structure early-exits or a pure residual talent menu.

---

## One Single Harvest (2026-08-11) — pure residual gates MET

Pure residual menu stands alone — **no library dispatch** to pdqsort or ska_sort on any residual path. EXTERNAL-clean.

| Gate | Target | Result |
|------|--------|-------:|
| db_pk_sparse | ≤ 0.95× pdq | **0.895×** |
| adversarial_pivot | ≤ 0.95× pdq | **0.856×** |
| HE isolated | ≤ 0.92× ska | **0.916×** |
| Full-suite geo | ≤ 0.55× pdq | **0.538×** |
| Zero major regressions | held | **0** |

| Metric | Prior freeze | Harvest |
|--------|-------------:|--------:|
| geo pure/pdq | 0.673× | **0.538×** |
| db_pk_sparse | 2.56× | **0.895×** |
| adversarial_pivot | 1.81× | **0.856×** |

**Menu:** STRUCTURE → Majority (Dutch-flag) → Low-card counting → Sparse MSD → Identity-almost → HE MSD

Sources: [`residual/pure_residual_menu.hpp`](./residual/pure_residual_menu.hpp) · Results: [`docs/harvest/`](./docs/harvest/) · Non-claims: [`NON_CLAIMS.md`](./NON_CLAIMS.md)

**Not a broader field-level claim beyond this suite.**

---

## Vector-2 Status — Domination Suite v0.1

**Criteria MET** (2026-08-09). See [docs/vector2/VECTOR2_CLAIM_v0.1.md](./docs/vector2/VECTOR2_CLAIM_v0.1.md).

---

## Phase 2 residual freeze

**Status: Complete and Frozen** (2026-08-10). Release: [v1.4.0-c](https://github.com/HeywoodGeblomi/PhotonicSort/releases/tag/v1.4.0-c). Docs: [`docs/phase2/`](./docs/phase2/).

---

## Architecture

```
probe → structure early-exit → pure residual talent menu
         (sorted / reverse)     Majority | Low-card | Sparse | Identity-almost | HE MSD
```

| Route | Residual (pure path) |
|-------|----------------------|
| STRUCTURE | O(n) early-exit / reverse (**verified**) |
| Majority | Dutch-flag around sampled mode (`residual_adversarial_i64.hpp`) |
| Low-card | Counting sort |
| Sparse | Pure MSD INS=64 BLOCK=256 (`residual_sparse_i64.hpp`) |
| Identity-almost | Misplaced-index recovery |
| HE / RANDOM | Classical MSD INS=96 BLOCK=512 (`residual_he_msd_i64.hpp`) |

---

## Build

```bash
git clone https://github.com/HeywoodGeblomi/PhotonicSort.git
cd PhotonicSort/c
make && make test && ./demo
```

Full guide: [BUILD.md](./BUILD.md).

---

## Benchmarks

See [BENCHMARKS.md](./BENCHMARKS.md) and [docs/harvest/P4_FULL_SUITE_RESULT.md](./docs/harvest/P4_FULL_SUITE_RESULT.md).

---

## Non-claims

**Full statement:** [`NON_CLAIMS.md`](./NON_CLAIMS.md).

Not a field-level breakthrough beyond the published suite. Harvest closed the prior pure residual floors; independent reproduction still required for stronger language.

---

## Contributors

- **Heywood Geblomi** — design, adaptive hybrid architecture, release engineering
- Grok (xAI) + THE BEASTIE BOYZ (Harper, Lucas, Benjamin) — implementation collaboration

---

## License

MIT — [LICENSE](./LICENSE).
