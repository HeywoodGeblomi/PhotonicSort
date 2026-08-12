# A2 Soft-Spot Close / Scope — vs Best Specialized

**Date:** 2026-08-12  
**Status:** PARTIAL CLOSE + FORMAL SCOPE  
**Parent:** A1 gap map · Field-Level Entry Attack Plan  
**Not field-level.**

---

## 1. Closed (were >1.15× on A1 i64)

| Pattern | A1 ratio | A2 ratio | Mechanism |
|---------|-------:|-------:|-----------|
| **db_pk** | **57.1×** | **0.89×** | Ultra-low inv + non-consecutive domain → residual_pdqsort (killed sparse-inversion insertion) |
| **few_k4_dense** | 2.24× | **0.61×** | Early counting before expensive run scans |
| **equal_heavy** | 1.59× | **0.54×** | Early equal-sample → residual_pdqsort (before STRUCTURE tax) |

---

## 2. Formal scope-outs (residual quality limits)

| Pattern | A2 ratio | Best | Rationale |
|---------|-------:|------|----------|
| **pipe_sparse** | ~2.5–2.8× | pdq | Condensed residual_pdqsort quality limit vs library pdqsort |
| **random** | ~1.5× | **ska** | Integer radix wins full-range uniform; pure residual does not claim to beat ska here |
| **gaussianish** | ~1.4–1.5× | pdq | Continuous HE residual path tax (documented limit) |
| **few_k16_wide** | ~1.6–1.7× | pdq | FEW_WIDE residual quality limit on wide mid-card |
| **runs_noise** | ~1.4–1.7× | pdq | Residual quality / routing tax on noisy multi-run |

These are **honest residual quality limits**, not routing bugs.

---

## 3. Code changes (EXTERNAL-clean)

1. `residual_low_disorder_i64.hpp` — ultra-low inv uses residual_pdqsort, not insertion
2. `pure_residual_menu.hpp` — early counting; ultra-low+non-consecutive → residual_pdqsort; early equal → residual_pdqsort; HE MSD default for HE fallback

---

## 4. A2 exit

| Criterion | Status |
|-----------|--------|
| Close every soft OR formal scope-out | **YES** (3 closed, 5 scoped) |
| EXTERNAL-clean | **YES** |
| Field-level language | **Still forbidden** |

i64 geo on full 19-pattern suite after A2 patches: **~0.87×** vs best specialized (includes scoped softs).

**THE BEASTIE BOYZ**
