# Remaining Soft Spots — Imminent Closure List

**Date:** 2026-08-12  
**Stack:** few_wide v2.6.1 + STRUCTURE v2 + residual_pdqsort (low_disorder) + MSD HE  
**Method:** cold isolation, n=1e6, median of 9  
**Status:** **NOT field-level**

---

## Executive inventory

| Pattern | i32 | u32 | Class | Imminent action |
|---------|----:|----:|-------|-----------------|
| **pipe_sparse** | **7.9–8.9×** | **8.5×** | HE residual → weak MSD | **PRIMARY KILL** — route HE fallback to residual_pdqsort |
| db_pk | 0.95× | 1.09× | residual quality | held (≤1.15×) |
| few_k4 | 1.02× | 1.02× | residual quality | held |
| all other controlled patterns | ≤1.0× | ≤1.0× | — | **CLOSED** |

**Only residual soft spot remaining under cold isolation: `pipe_sparse`.**

---

## 1. pipe_sparse — PRIMARY (imminent closure)

| Metric | Value |
|--------|------:|
| i32 menu/pdq | **7.9–8.9×** |
| u32 menu/pdq | **~8.5×** |
| residual_pdqsort direct | **0.71×** (closes cleanly) |
| std::sort | ~4× vs pdq |

**Root cause:**  
Pattern has ~666k uniques, inv≈0.67 → few_wide/counting/low_disorder gates all reject → falls through to **MSD HE**, which is ~8–9× slower than pdq on this shape.

**Closure path (minimal):**  
Change HE fallback from MSD to `residual_pdqsort` (already vendored, zlib, used by low_disorder). Expected post-fix: ≤1.0× on pipe_sparse; no regression expected on patterns that currently hit MSD competitively (random ~0.7× may stay similar or improve).

**Alternative:** keep MSD for true uniform HE where it wins; add a cheap entropy/disorder sample that routes structured-HE (pipe_sparse class) to residual_pdqsort. More complex; residual_pdqsort-as-default-HE is simpler and already proven.

---

## 2. Borderline (not soft under 1.15× gate)

| Pattern | i32 | u32 | Note |
|---------|----:|----:|------|
| db_pk | 0.95× | 1.09× | held |
| few_k4 | 1.02× | 1.02× | held |

No action required for ≤1.15× gate.

---

## 3. Closed under isolation (post STRUCTURE v2 + few_wide v2.6.1)

sorted, reverse, random, almost_sorted, timestamps, equal_heavy, few_k16_dense, few_k16_wide, organpipe, sawtooth, push_middle, gaussianish, adversarial, ascending_runs — all ≤1.0× (many ≪1.0×).

---

## 4. Field-level gates (not residual soft spots)

These are **not residual soft spots** but still block field-level language:

| Gate | Status |
|------|--------|
| Multi-arch numbers | NOT MET |
| Statistical CI | NOT MET |
| Phase 0 suite re-measure post-stack | PENDING (PO1) |
| Docker GREEN + reference ratios | PENDING (PO2/PO6) |
| i64 measured | PENDING (PO5) |

**Still NOT field-level.** Closing pipe_sparse does not by itself make the claim field-level.

---

## 5. Recommended kill order

1. **pipe_sparse** — HE fallback → residual_pdqsort (or hybrid gate)
2. Re-measure cold isolation + Phase 0 suite (PO1)
3. Confirm no regression on random/gaussianish/adversarial
4. Then resume PO2–PO6

**THE BEASTIE BOYZ**
