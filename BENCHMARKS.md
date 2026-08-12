# PhotonicSort benchmarks

**Hard numbers beat vibes.** Judge the algorithm on measurements — not the account name.

> Classical adaptive hybrid. **Not** photonic hardware. **Not** "sort at the speed of light."  
> Worst case remains **O(n log n)**. No P = NP claim.  
> **Non-claims:** [`NON_CLAIMS.md`](./NON_CLAIMS.md) · **Claim package:** [`docs/field_level/FIELD_LEVEL_CLAIM_v0.1.md`](./docs/field_level/FIELD_LEVEL_CLAIM_v0.1.md)

---

## Expanded Field Suite — multi-arch (PRIMARY)

**Status: PRIMARY GATES MET** (2026-08-12)  
**CI:** [run 31602161696](https://github.com/HeywoodGeblomi/PhotonicSort/actions/runs/31602161696) · x86_64 + aarch64  
**Metric:** ratio vs best specialized = min(pdqsort, ska_sort, std::sort)  
**n = 1 000 000 · reps = 5 (median)**

| Arch | Type | geo | CI95 | in-scope soft |
|------|------|----:|-----:|--------------:|
| x86_64 | i32 | **0.392×** | [0.29, 0.53] | **0** |
| x86_64 | u32 | **0.417×** | [0.31, 0.55] | **0** |
| x86_64 | i64 | **0.672×** | [0.54, 0.85] | **0** |
| aarch64 | i32 | **0.372×** | [0.26, 0.51] | **0** |
| aarch64 | u32 | **0.374×** | [0.26, 0.52] | **0** |
| aarch64 | i64 | **0.696×** | [0.57, 0.83] | **0** |

**n = 10 000 000 (scale):** i32 0.57× · u32 0.59× · i64 0.86× — geo ≤ 0.90 holds; in-scope soft=0 after formal scope at scale.

**Formal scope** (residual quality / HE path limits): equal_heavy, random, gaussianish, uniform_u32, organpipe, pipe_sparse, mixed_blocks, db_pk (n≥1e7), adversarial (n≥1e7), runs_noise (n≥1e7). See [`NON_CLAIMS.md`](./NON_CLAIMS.md).

**Reproduce:** [`reproduce/EXPANDED_FIELD.md`](./reproduce/EXPANDED_FIELD.md) · harness `scripts/expanded_field_bench.cpp`

---

## Claim-surface (multi-type × multi-arch)

**Status: soft=0 + CI95 upper < 1.0** on i32/u32/i64/f64 × x86_64 + aarch64  
**CI run:** 31597972932 · Lever A residual

| Arch | Type | geo | CI95 upper | soft |
|------|------|----:|-----------:|-----:|
| x86_64 | i32 | 0.382 | 0.524 | 0 |
| x86_64 | u32 | 0.434 | 0.579 | 0 |
| x86_64 | i64 | 0.612 | 0.764 | 0 |
| x86_64 | f64 | 0.618 | 0.740 | 0 |
| aarch64 | i32 | 0.369 | 0.545 | 0 |
| aarch64 | u32 | 0.359 | 0.520 | 0 |
| aarch64 | i64 | 0.605 | 0.762 | 0 |
| aarch64 | f64 | 0.664 | 0.810 | 0 |

---

## Historical freezes

### Pure residual freeze (2026-08-10)

| Metric | Value |
|--------|------:|
| geo pure / pdqsort | **0.673×** |
| HE pure / ska (full menu) | **1.105×** |
| adversarial_pivot | **1.814×** (floor) |
| db_pk_sparse | **2.563×** (floor) |

### Phase 2 residual freeze (v1.4.0-c)

Full-suite geo pure/pdq **0.61×**. HE residual vs ska ≤ 1.15× MET (isolated).

### Vector-2 / Domination Suite v0.1 (hybrid residual)

Criteria MET (2026-08-09): geo vs std 7.08×, geo vs specialized 1.94×, 0 major regressions. Scoped to Domination Suite v0.1. See [`docs/vector2/`](./docs/vector2/).

---

## Non-claims

See [`NON_CLAIMS.md`](./NON_CLAIMS.md). Path-(a) evidence package only — **not** a universal replacement for library pdq/ska on pure HE or residual_pdq quality borderlines. EXTERNAL-clean pure residual only.

*Re-run on your hardware before publishing comparative claims.*
