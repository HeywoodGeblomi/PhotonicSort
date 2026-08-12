# PO5 — i64 Phase 0 Gap Map + Full Suite

**Date:** 2026-08-12  
**Team:** THE BEASTIE BOYZ  
**Stack:** pure_residual_menu.hpp (i64) + STRUCTURE v2 + late pipe_sparse gate  
**Method:** n=1e6, median of 7, vs orlp pdqsort  
**Status:** **PO5 measured** · residual soft spots documented · **NOT field-level**

---

## i64 results

| pattern | menu/pdq | status |
|---------|----------:|--------|
| sorted | 0.73× | CLOSED |
| reverse | 0.62× | CLOSED |
| random | 0.80× | CLOSED |
| almost_sorted | 0.49× | CLOSED |
| db_pk | 1.14× | CLOSED |
| timestamps | 0.72× | CLOSED |
| few_k4_dense | **1.56×** | **SOFT** |
| few_k16_dense | 0.84× | CLOSED |
| few_k16_wide | 0.58× | CLOSED |
| equal_heavy | 0.72× | CLOSED |
| zipf_k16 | 0.81× | CLOSED |
| organpipe | 0.61× | CLOSED |
| sawtooth | 0.25× | CLOSED |
| pipe_sparse | **0.85×** | CLOSED (was ~7.5×) |
| push_middle | **3.24×** | **SOFT** residual limit |
| reverse_segments | **1.33×** | **SOFT** residual limit |
| gaussianish | 1.09× | CLOSED |
| adversarial | 0.99× | CLOSED |
| runs_noise | 0.86× | CLOSED |

**i64 geo mean:** 0.84× · **Soft count:** 3

### Residual limits (honest)

| Soft spot | Ratio | Notes |
|-----------|------:|-------|
| push_middle | ~3.2× | 3-run merge residual quality |
| few_k4_dense | ~1.6× | FEW_WIDE vs pdq on ultra-low dense card |
| reverse_segments | ~1.3× | reverse-runs residual quality |

pipe_sparse kill ported (late compact + not-full-unique → residual_pdqsort).

---

## i32 continuity (same suite)

All patterns **CLOSED** (max 0.96×). No regressions.

| pattern | menu/pdq |
|---------|----------:|
| sorted | 0.50× |
| reverse | 0.38× |
| random | 0.62× |
| almost_sorted | 0.25× |
| db_pk | 0.96× |
| timestamps | 0.53× |
| few_k16_wide | 0.86× |
| organpipe | 0.09× |
| pipe_sparse | 0.72× |
| gaussianish | 0.51× |
| adversarial | 0.09× |
| push_middle | 0.23× |

---

## Non-claims

- Not field-level.
- i64 soft spots are residual quality limits.
- Multi-arch MET is i32/u32; i64 multi-arch not re-run on ARM in this ticket.
- Sequential POD integers only.

**THE BEASTIE BOYZ**
