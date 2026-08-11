# PhotonicSort

**Adaptive hybrid sorting library** — C11 performance core + Python reference + Rust FFI bindings.

[![License: MIT](https://img.shields.io/badge/license-MIT-green.svg)](./LICENSE)
[![C11](https://img.shields.io/badge/C-11-blue.svg)](./c/)
[![Rust](https://img.shields.io/badge/Rust-FFI-orange.svg)](./rust/)
[![Version](https://img.shields.io/badge/version-1.3.2--c-informational.svg)](./c/RELEASE_NOTES_v1.3.2-c.md)
[![Vector-2](https://img.shields.io/badge/Vector--2-MET-brightgreen.svg)](./docs/vector2/VECTOR2_CLAIM_v0.1.md)
[![Phase-2](https://img.shields.io/badge/Phase%202-frozen-blue.svg)](./docs/phase2/PHASE2_CYCLE_COMPLETE.md)
[![Pure residual](https://img.shields.io/badge/pure%20residual-frozen-blue.svg)](./docs/phase3/)
[![Release](https://img.shields.io/badge/release-v1.4.0--c-informational.svg)](https://github.com/HeywoodGeblomi/PhotonicSort/releases/tag/v1.4.0-c)

**Primary practical sorter of THE BEASTIE BOYZ.** Classical adaptive hybrid: GyroRank-style pilot probe selects structure early-exits or a residual talent menu.

---

## Pure residual freeze (2026-08-10)

**Status: Frozen.** Best pure HE residual measured on this host (~**1.02–1.09×** isolated vs ska; **1.105×** full-menu). Full-suite geo pure/pdqsort **0.673×**.

| Floor | pure / pdqsort |
|-------|---------------:|
| equal_heavy | **0.993×** |
| adversarial_pivot | **1.814×** |
| db_pk_sparse | **2.563×** |

Residual: classical MSD + blocked scatter + prefetch; majority_v2 + sparse-cluster. STRUCTURE / Class 1 held.  
**Not a field-level breakthrough.** Pure residual work stopped. See [`NON_CLAIMS.md`](./NON_CLAIMS.md) and [`docs/phase3/`](./docs/phase3/).

---

## Vector-2 Status — Domination Suite v0.1

**Criteria MET** (2026-08-09). See [docs/vector2/VECTOR2_CLAIM_v0.1.md](./docs/vector2/VECTOR2_CLAIM_v0.1.md).

---

## Phase 2 residual freeze

**Status: Complete and Frozen** (2026-08-10). Release: [v1.4.0-c](https://github.com/HeywoodGeblomi/PhotonicSort/releases/tag/v1.4.0-c). Full-suite geo-mean **0.61×** vs pdqsort. **Not a field-level breakthrough.** Docs: [`docs/phase2/`](./docs/phase2/).

---

## Architecture

```
probe → structure early-exit → residual talent menu
         (sorted / reverse)     LOW_CARD | LOW_DISORDER | PATTERNED | RANDOM | Class 1
```

| Route | Residual (pure path) |
|-------|----------------------|
| STRUCTURE | O(n) early-exit / reverse (**verified**) |
| LOW_CARD | Counting sort |
| LOW_DISORDER | Cache-local insertion / identity-almost / majority_v2 |
| PATTERNED | Capacity-checked run merge |
| Class 1 | `rsl_structured_correct` |
| RANDOM / HE | Classical MSD + blocked scatter + prefetch (`residual/msd_radix_i64.hpp`) |

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

See [BENCHMARKS.md](./BENCHMARKS.md) for pure residual freeze, Phase 2, Vector-2, and pure-C Plan A numbers.

---

## Non-claims

**Full statement:** [`NON_CLAIMS.md`](./NON_CLAIMS.md).

Not a field-level breakthrough. Best pure HE residual to date; residual floors documented; gates red.

---

## Contributors

- **Heywood Geblomi** — design, adaptive hybrid architecture, release engineering
- Grok (xAI) + THE BEASTIE BOYZ (Harper, Lucas, Benjamin) — implementation collaboration

---

## License

MIT — [LICENSE](./LICENSE).
