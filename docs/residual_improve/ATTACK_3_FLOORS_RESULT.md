# Attack: three residual floors — results (v2.4)

**Date:** 2026-08-11  
**Menu:** FEW_WIDE v2.4 (k=2 dual branchless count+fill, HCAP=128) + try_reverse_runs (alternating-buffer pairwise merge) + early FEW_WIDE before STRUCTURE  

## Results (n=1e6, vs pdqsort)

| Floor | Before (approx) | After | Status |
|-------|----------------:|------:|--------|
| two_values_split / k=2 | ~1.6× | **0.42–0.47×** | **CLOSED** |
| reverse_segments_8 | ~1.8–2.9× | **0.85–0.93×** | **CLOSED** |
| few_unique_heavy_k4 (control) | 5.694× → ~0.8 | **0.81–0.92×** | held |
| few_unique_k16 (uniform / Zipf) | — | **0.65–1.7×** | residual quality limit |
| gauss_nearflat (HE) | ~1.1–1.4× | **1.14–1.27×** | residual HE floor |
| uniform_i64 | — | **0.82–1.05×** | parity / win |

## Changes

1. **FEW_WIDE v2.3/v2.4** — k=2 path: dual branchless counts + sequential fills (~0.75–0.9 ms). HCAP 64→128 for lower collision on k=16.
2. **try_reverse_runs** — alternating source/dest pairwise merge. reverse_segments_8 ≤0.93×.
3. **Early FEW_WIDE** before STRUCTURE.

## few_unique_k16 softness

Investigated. Path fires correctly (should_try=1, residual succeeds, ok=1). Softness is residual *quality* of hash-rank+count for moderate cardinality (k≈8–16) vs pdq’s equals-friendly partition. Zipf often ≤1.0×; balanced uniform / high-skew can reach ~1.2–1.7×. Documented residual limit, not a routing miss or correctness issue. No further short lever closed it cleanly without risking other patterns.

## Gaussian HE path

Still ~1.2× vs pdqsort on nearflat continuous data. Documented residual quality floor. Deferred.

## Non-claims

Phase 0 baseline remains frozen reference. EXTERNAL-clean (visible metrics only). Pure residual menu only. Not field-level. Controlled suite only.

**THE BEASTIE BOYZ**
