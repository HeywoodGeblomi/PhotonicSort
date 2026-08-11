# PhotonicSort benchmarks

**Hard numbers beat vibes.** Judge the algorithm on measurements — not the account name.

> Classical adaptive hybrid. **Not** photonic hardware. **Not** "sort at the speed of light."  
> Worst case remains **O(n log n)**. No P = NP claim.  
> **Non-claims:** see [`NON_CLAIMS.md`](./NON_CLAIMS.md).

---

## Pure residual freeze (2026-08-10)

**Status: Frozen.** Best pure HE residual to date. Gates red. **Not field-level.**

**n = 1 000 000 · trials = 7 · best-of wall time · pure adaptive menu**

| Metric | Value |
|--------|------:|
| **geo pure / pdqsort** | **0.673×** |
| min pure / pdqsort | 0.385× |
| geo pure / std::sort | **0.144×** |
| HE pure / ska (full menu) | **1.105×** |
| HE pure / ska (isolated residual) | **~1.02–1.09×** |

| Pattern | pure / pdq | pure / ska |
|---------|-----------:|-----------:|
| uniform_i64 | 0.789× | **1.105×** |
| sorted / reverse | 0.46× / 0.43× | — |
| almost_0.001 / 0.01 | 0.48× / 0.44× | — |
| organpipe | 0.56× | 0.66× |
| sawtooth / few_unique | 0.39× / 0.39× | — |
| equal_heavy | **0.993×** | — |
| adversarial_pivot | **1.814×** (floor) | — |
| db_pk_sparse | **2.563×** (floor) | — |

**Residual:** classical MSD, fixed 8-bit, blocked scatter + prefetch, INS=192; majority_v2 + sparse-cluster routing.  
**Docs:** [`docs/phase3/`](./docs/phase3/) · [`NON_CLAIMS.md`](./NON_CLAIMS.md) · residual source [`residual/msd_radix_i64.hpp`](./residual/msd_radix_i64.hpp)

CSV: [`docs/phase3/FROZEN_BASELINE_FULL_SUITE.csv`](./docs/phase3/FROZEN_BASELINE_FULL_SUITE.csv)

---

## Phase 2 residual freeze (pure adaptive residual path)

**n = 1 000 000 · trials = 5 · geo-mean wall time · vs pdqsort**  
**Status: Complete and Frozen** (2026-08-10) · Release [v1.4.0-c](https://github.com/HeywoodGeblomi/PhotonicSort/releases/tag/v1.4.0-c)

| Metric | pure / pdqsort | Notes |
|--------|---------------:|-------|
| **Full-suite geo-mean** | **0.61×** | Ahead of pdqsort overall |
| almost_0.001 | **0.36×** | Identity-almost residual |
| almost_0.01 | **0.42×** | |
| Class 1 (sa_*) | **0.41–0.52×** | rsl_structured_correct |
| STRUCTURE (sorted/reverse) | **0.53–0.59×** | |
| uniform_i64 | **1.07×** | Near parity |
| HE residual vs ska (isolated) | **1.10×** | ≤ 1.15× target **MET** |
| equal_heavy | **1.43×** | Majority residual |
| adversarial_pivot | **~2.1×** | Residual floor vs pdqsort |
| db_pk_sparse | **~1.7×** | Residual floor |

**Suite:** Breakthrough Phase 0 expanded generators.  
**Honesty:** Not field-level. ska still wins pure HE. Residual floors acknowledged.  
**Docs:** [`docs/phase2/`](./docs/phase2/) · [`NON_CLAIMS.md`](./NON_CLAIMS.md)

---

## Vector-2 / Domination Suite v0.1 (hybrid residual configuration)

**Status: Criteria MET** (2026-08-09)

| Criterion | Threshold | Measured | Result |
|-----------|-----------|----------|--------|
| Geometric mean vs `std::sort` | ≥ 1.5× | **7.08×** | PASS |
| Geometric mean vs best specialized (pdqsort / ska_sort) | ≥ 1.5× | **1.94×** | PASS |
| Minimum speedup vs `std::sort` | ≥ 1.0× | **1.32×** | PASS |
| Major regressions vs specialized | 0 | **0** | PASS |

**Scope (mandatory):** On Domination Suite v0.1 the locked Vector-2 criteria are met. Structured / patterned / low-card wins remain pure-C Photonic. High-entropy residual uses library-strength pdqsort / ska_sort after selection from visible probe metrics. Result is scoped to this suite; broader evaluation may differ. No asymptotic claim. Full details: [docs/vector2/VECTOR2_CLAIM_v0.1.md](./docs/vector2/VECTOR2_CLAIM_v0.1.md).

The pure-C self-contained residual numbers (Plan A / v1.3.x) continue in the full repository history and prior sections of this document on main before this freeze update.

---

## Non-claims

See [`NON_CLAIMS.md`](./NON_CLAIMS.md). Phase 2 and pure residual freeze are **not field-level breakthroughs**. Independent reproduction required.

*Re-run on your hardware before publishing comparative claims.*
