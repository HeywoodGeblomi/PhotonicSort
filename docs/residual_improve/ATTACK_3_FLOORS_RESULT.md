# Attack: three residual floors — results

**Date:** 2026-08-11  
**Menu:** FEW_WIDE v2.2 (branchless k=2) + try_reverse_runs (reverse+merge) + early FEW_WIDE before STRUCTURE  

## Results (n=1e6, median-of-11, vs pdqsort)

| Floor | Before | After | Status |
|-------|-------:|------:|--------|
| two_values_split / k=2 | ~1.6× | **0.91×** | **CLOSED** |
| few_unique_heavy_k4 | 5.694× | **0.83×** | **CLOSED** |
| reverse_segments_8 | ~1.8–2.9× | **0.73×** | **CLOSED** |
| gauss_nearflat (HE) | ~1.1–1.4× | **1.20×** | residual HE floor |
| uniform_i64 | — | **1.02×** | parity |

## Changes

1. **FEW_WIDE v2.2** — branchless block partition for k=2 (matches pdq on balanced 2-value data)
2. **try_reverse_runs** — reverse descending runs ≥4, then successive pairwise merge of remaining runs
3. **Early FEW_WIDE** — fire before O(n) STRUCTURE scan to cut probe tax on few-unique wide

## Gaussian HE path

Still ~1.2× vs pdqsort. Residual quality on continuous near-uniform data. Documented floor; not a regression.

## Non-claims

Phase 0 baseline remains frozen reference. EXTERNAL-clean. Pure residual only. Not field-level.

**THE BEASTIE BOYZ**
