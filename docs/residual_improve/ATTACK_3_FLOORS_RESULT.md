# Attack: three residual floors — results (v2.3)

**Date:** 2026-08-11  
**Menu:** FEW_WIDE v2.3 (k=2 dual branchless count+fill) + try_reverse_runs (alternating-buffer pairwise merge) + early FEW_WIDE before STRUCTURE  

## Results (n=1e6, median-of-11 / focused medians, vs pdqsort)

| Floor | Before (approx) | After | Status |
|-------|----------------:|------:|--------|
| two_values_split / k=2 | ~1.6× | **0.42–0.47×** | **CLOSED** |
| reverse_segments_8 | ~1.8–2.9× | **0.85–0.93×** | **CLOSED** |
| few_unique_heavy_k4 (control) | 5.694× → ~0.8 | **0.81–0.92×** | held |
| gauss_nearflat (HE) | ~1.1–1.4× | **1.14–1.27×** | residual HE floor |
| uniform_i64 | — | **0.82–1.05×** | parity / win |

## Changes

1. **FEW_WIDE v2.3** — k=2 path replaced branchless partition with dual branchless count (`c0 += (a[i]==v0)`) + two sequential fills. Full dual-count verifies no third value. ~0.75–0.9 ms raw; menu 0.42× vs pdq.
2. **try_reverse_runs** — successive pairwise merge rewritten to alternating source/dest buffers (one final memcpy if needed). Eliminates per-pair memcpy tax. Remaining-runs ≤32 gate retained. reverse_segments_8 now consistently ≤0.93×.
3. **Early FEW_WIDE** — retained (fires before O(n) STRUCTURE).

## Gaussian HE path

Still ~1.2× vs pdqsort on nearflat continuous data. Classic MSD radix (8-bit blocked scatter + INS=96) vs pdq branchless partition tradeoff on ~20–25 effective bits. Constant retunes (INS 48/192, BLOCK 256) did not close the gap reliably under variance. Documented residual quality floor; no routing regression. Further gains require deeper HE residual engineering (adaptive digit / hybrid leaf / pure introsort residual) — deferred.

## Non-claims

Phase 0 baseline remains frozen reference. EXTERNAL-clean (visible metrics only). Pure residual menu only. Not field-level. Controlled suite only.

**THE BEASTIE BOYZ**
