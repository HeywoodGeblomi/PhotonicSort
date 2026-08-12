# Claim Surface — FROZEN

**Date:** 2026-08-12  
**Status:** **LOCKED**  
**Not field-level** (A4 claim package still required).

---

## 1. In-scope patterns (13)

| ID | Pattern | Group |
|----|---------|-------|
| P01 | sorted | Structure |
| P02 | reverse | Structure |
| P03 | almost_sorted | Structure |
| P05 | sawtooth | Structure |
| P06 | reverse_segments | Structure |
| P07 | push_middle | Runs |
| P09 | few_k4_dense | Low-card |
| P10 | few_k16_dense | Low-card |
| P12 | equal_heavy | Low-card |
| P13 | zipf_k16 | Low-card |
| P14 | db_pk | Real-shaped |
| P15 | timestamps | Real-shaped |
| P17 | adversarial | Adversarial |

## 2. Formal scope-outs (not in primary geo)

| Pattern | Reason |
|---------|--------|
| pipe_sparse | residual_pdqsort quality limit vs library pdq |
| random | ska wins HE uniform |
| gaussianish | continuous HE path tax |
| few_k16_wide | FEW_WIDE residual quality |
| runs_noise | residual quality / routing tax |
| organpipe | ska wins; residual cannot match radix on this shape |

## 3. Claim-surface results vs best specialized

### n=1e6 (primary)

| Type | geo | max | soft | Gate ≤0.90 / ≤1.15 |
|------|----:|----:|-----:|:------------------:|
| **i32** | **0.48×** | 1.09× | 0 | **PASS** |
| **u32** | **0.52×** | 1.12× | 0 | **PASS** |
| **i64** | **0.72×** | 1.03× | 0 | **PASS** |

### n=1e7 (A3 scale smoke, i64)

| Type | geo | max | soft | Gate |
|------|----:|----:|-----:|:----:|
| **i64** | **0.82×** | 1.03× | 0 | **PASS** |

## 4. Code with this freeze

- few_wide dense k≤4 requires wide (i32/u32/i64) → counting owns dense
- Early equal → residual_pdqsort
- Early counting before run scans
- Ultra-low inv + non-consecutive domain → residual_pdqsort (db_pk)
- low_disorder → residual_pdqsort

## 5. Next

- A3: float64 **or** generic comparator
- Multi-arch re-run on claim surface
- A4 only when float/generic + multi-arch held

**THE BEASTIE BOYZ**
