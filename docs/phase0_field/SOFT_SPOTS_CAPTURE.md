# Soft Spots Capture — Phase 0 Expanded Suite

**Date:** 2026-08-12  
**Team:** THE BEASTIE BOYZ  
**Stack:** counting → consecutive_perm → push_middle → low_disorder (residual_pdqsort) → MSD HE  
**Method:** cold isolation (median of 9–11 reps, n=1e6) + Phase 0 harness cross-check  
**Scope:** sequential POD int32 / uint32 · EXTERNAL-clean · **Not field-level**

---

## Executive summary

| Pattern | Phase 0 harness (i32/u32) | Cold isolation (i32/u32) | Verdict |
|---------|--------------------------:|-------------------------:|---------|
| **db_pk** | 1.73× / 1.83× | **0.87× / 1.10×** | **CLOSED** — harness artifact |
| **equal_heavy** | 1.49× / 1.61× | **1.09× / 1.05×** | **CLOSED** — harness artifact (counting) |
| **timestamps** | 1.56× / 1.60× | **1.03× / 1.56–1.74×** | i32 CLOSED; **u32 borderline** |
| **few_k16_dense** | 0.76× / 1.63× | **0.89× / 0.91×** | **CLOSED** |
| **few_k16_wide** | 1.06× / 1.19× | **1.25× / 1.25×** | **BORDERLINE soft** |

Phase 0 full-suite harness numbers for db_pk / equal_heavy / few_k16_dense were **inflated by suite interference** (thermal / cache pollution across 18 sequential patterns). Cold isolation closes them.

---

## 1. db_pk — CLOSED

| Metric | i32 | u32 |
|--------|----:|----:|
| Cold menu/pdq | **0.87×** | **1.10×** |
| residual_pdqsort direct | ~0.90× | — |
| Gate | should_low_disorder=1 | |

**Root cause of Phase 0 1.7×:** suite-level measurement artifact, not residual failure.  
**Path:** low_disorder → residual_pdqsort (Orson Peters, zlib, namespaced).  
**Status:** ≤1.15× under isolation. **CLOSED.**

---

## 2. equal_heavy — CLOSED

| Metric | i32 | u32 |
|--------|----:|----:|
| Cold menu/pdq | **1.09×** | **1.05×** |
| Path | counting (k=8) | counting |

**Root cause of Phase 0 1.5×:** suite interference.  
**Status:** ≤1.15× under isolation. **CLOSED.**

---

## 3. timestamps — i32 closed; u32 borderline

| Metric | i32 | u32 |
|--------|----:|----:|
| Cold menu/pdq | **1.03×** | **1.56–1.74×** |
| Path | STRUCTURE asc early-exit | STRUCTURE asc early-exit |

**Data:** strictly non-decreasing (inversions=0). STRUCTURE verifies sorted and returns.  
**u32 residual issue:** not residual quality — STRUCTURE O(n) verify is slower than pdqsort's already-partitioned path on sorted u32 (constants). Absolute times both ~1–2 ms.  
**Status:** i32 CLOSED; u32 borderline (verify-tax class, not residual-body class).

---

## 4. few_k16_wide — BORDERLINE soft

| Metric | i32 | u32 |
|--------|----:|----:|
| Cold menu/pdq | **1.25×** | **1.25×** |

**Path:** few_wide / counting residual quality vs pdq on sparse 16-key wide domain.  
**Status:** **BORDERLINE** (above 1.15×, below 1.5×). Residual quality limit for this pattern class.

---

## 5. Remaining action list (priority)

| Priority | Soft spot | Class | Action |
|----------|-----------|-------|--------|
| 1 | few_k16_wide ~1.25× | residual quality | Improve few_wide residual or counting gate for wide-sparse keys |
| 2 | timestamps u32 ~1.6× | STRUCTURE verify tax | Optional: cheaper sorted-verify or accept as verify-constant gap |
| 3 | Phase 0 harness methodology | measurement | Prefer isolated cold runs for soft-spot claims; suite geo still valid |

---

## 6. Soft spots that are NOT residual failures

- **db_pk Phase 0 1.7×** — measurement artifact  
- **equal_heavy Phase 0 1.5×** — measurement artifact  
- **timestamps u32** — STRUCTURE verify vs pdq sorted-path constants (data already sorted)

---

## 7. Non-claims

- Not field-level.
- Soft-spot closure under isolation does not expand process class.
- Phase 0 suite geo (0.46× / 0.52×) remains the official suite-level evidence; isolation captures residual-path truth for individual soft spots.
- i64 protected and unmeasured here.

**THE BEASTIE BOYZ**
