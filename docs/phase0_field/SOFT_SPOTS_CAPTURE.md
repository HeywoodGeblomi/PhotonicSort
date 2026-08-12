# Soft Spots Capture — Phase 0 Expanded Suite (updated)

**Date:** 2026-08-12  
**Team:** THE BEASTIE BOYZ  
**Stack:** counting → consecutive_perm → push_middle → low_disorder (residual_pdqsort) → MSD HE  
**few_wide:** v2.6.1 single-pass hash  
**Scope:** sequential POD int32 / uint32 · EXTERNAL-clean · **Not field-level**

---

## Executive summary (post few_wide v2.6.1)

| Pattern | Prior cold | Now | Verdict |
|---------|----------:|----:|--------|
| **db_pk** | 0.87–1.10× | held | **CLOSED** |
| **equal_heavy** | 1.05–1.09× | held | **CLOSED** |
| **few_k16_dense** | 0.89–0.91× | **0.53×** | **CLOSED** |
| **few_k16_wide** | **1.25×** | **0.93–0.98×** | **CLOSED** |
| **timestamps i32** | 1.03× | held | **CLOSED** |
| **timestamps u32** | ~1.6× | ~1.6× | borderline verify-tax only |

**All residual-quality soft spots on the controlled/Phase 0 pattern set are CLOSED under cold isolation.**

Only remaining non-residual gap: timestamps u32 STRUCTURE verify vs pdq sorted-path constants (~1–2 ms absolute).

---

## few_k16_wide kill (v2.6.1)

**Before:** ~1.25× (double-pass hash collect + count)  
**After:** **0.93–0.98×** (single-pass open-address key+count, HCAP=64, writeback unroll)

No regressions: almost_sorted, db_pk, uniform, equal_heavy, two_values, few_k16_dense held.

---

## Non-claims

- Not field-level.
- Soft-spot closure under isolation does not expand process class.
- Phase 0 suite geo remains official suite-level evidence.
- i64 protected.

**THE BEASTIE BOYZ**
