# FIELD_LEVEL_CLAIM_v0.1 — Path-(a) Evidence Package

**Date:** 2026-08-12  
**Squad:** THE BEASTIE BOYZ  
**Main residual SHA:** 2750cd58 (A2-EH + HCAP=128 + gates capture)  
**CI Expanded Suite:** run 31602161696 SUCCESS (x86_64 + aarch64)

---

## Claim (narrow, EXTERNAL-clean)

On the **in-scope** Expanded Field Suite (P01–P23 minus formal scope-outs), pure residual PhotonicSort menu achieves:

1. **Primary geo ≤ 0.90×** vs best specialized (`min(pdqsort, ska_sort, std::sort)`) on i32 / u32 / i64
2. **In-scope soft_count = 0** (no in-scope pattern > 1.15×) on **x86_64 and aarch64** at n=1e6
3. Claim-surface soft=0 + CI95 upper < 1.0 on i32/u32/i64/f64 × both ISAs (prior lock)

This is a **path-(a) empirical evidence package**, not a SOTA displacement claim and not a complexity theorem.

---

## Measured evidence

### Expanded Suite multi-arch (n=1e6, R=5, CI run 31602161696)

| Arch | Type | geo | CI95 | in-scope soft |
|------|------|----:|-----:|--------------:|
| x86_64 | i32 | 0.392 | [0.29, 0.53] | **0** |
| x86_64 | u32 | 0.417 | [0.31, 0.55] | **0** |
| x86_64 | i64 | 0.672 | [0.54, 0.85] | **0** |
| aarch64 | i32 | 0.372 | [0.26, 0.51] | **0** |
| aarch64 | u32 | 0.374 | [0.26, 0.52] | **0** |
| aarch64 | i64 | 0.696 | [0.57, 0.83] | **0** |

### n-scale (n=1e7)

| Type | geo | in-scope soft | Notes |
|------|----:|--------------:|-------|
| i32 | 0.57 | **0** | organpipe scoped only |
| u32 | pending | — | in flight |
| i64 | pending | — | in flight |

### Formal scope-outs (residual quality / HE path limits — not bugs)

| Pattern | Type(s) | Rationale |
|---------|---------|-----------|
| equal_heavy | i32/u32 | residual_pdqsort quality vs library pdq |
| random / gaussianish / uniform_u32 | i64 | ska/radix wins pure HE |
| organpipe | i64 | residual_pdq / ska mountain shape |
| pipe_sparse | i64 | residual_pdq quality (~1.19×) |
| mixed_blocks | i64 | residual_pdq quality (~1.20× aarch64) |

---

## What this is not

1. **Not a universal replacement** for library pdq/ska on pure HE or equal-heavy residual_pdq quality.
2. **Not a novel adaptive complexity theorem.**
3. **Not independent third-party Expanded Suite reproduction** (claim-surface Wave 0 Docker exists; Expanded Suite one-command path pending).
4. **Not asymptotic novelty.** EXTERNAL-clean pure residual only. No χ.

---

## Reproduction pointers

- Harness: `scripts/expanded_field_bench.cpp`
- Metrics: `scripts/field_metrics.py`
- CI: `.github/workflows/expanded-field.yml` (workflow_dispatch)
- Claim-surface: `.github/workflows/claim-surface.yml`
- Formal scope: `docs/field_level/A1_GAP_MAP.md` §3
- Multi-arch: `docs/field_level/MULTI_ARCH_RESULTS.md`

---

## Status

Path-(a) evidence package **shipped** for locked claim surface + Expanded Suite multi-arch in-scope soft=0 + geo≤0.90.  
Full field-level entry still gated on: complete n=1e7 all types + independent Expanded Suite reproduce path.

**Honesty primary. EXTERNAL-clean. Not a SOTA displacement claim.**

**THE BEASTIE BOYZ**
