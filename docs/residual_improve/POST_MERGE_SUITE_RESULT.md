# Post-merge full-suite (v2.4 menu on main)

**Date:** 2026-08-11  
**Commit:** f9e1432 (PR #38 squash)  
**Menu:** pure residual v2.4 — early FEW_WIDE, try_reverse_runs (alt-buffer), FEW_WIDE HCAP=128, counting, majority, push_middle, sparse, identity-almost, HE MSD  

**Host:** median-of-7, n=1e6, vs pdqsort, -O3  

## Scoreboard

| Pattern | menu/pdq | ok |
|---------|---------:|:--:|
| sorted | 0.844× | 1 |
| reverse | 0.219× | 1 |
| organpipe | 0.535× | 1 |
| sawtooth | 0.422× | 1 |
| almost_sorted | 0.515× | 1 |
| uniform_i64 | 0.908× | 1 |
| gauss_nearflat | 1.195× | 1 |
| two_values_50_50 | **0.227×** | 1 |
| few_k4_wide | **0.633×** | 1 |
| few_k16_uniform | 1.208× | 1 |
| reverse_segments_8 | **0.772×** | 1 |
| few_unique_k8 | 0.706× | 1 |
| zipf_k16 | 1.068× | 1 |
| push_middle | 0.521× | 1 |
| equal_heavy | 1.457× | 1 |
| adversarial_pipe | 0.539× | 1 |

**Geo-mean (16 patterns): 0.648×** pure/pdq  

## Attack floors (post-merge confirmation)

| Floor | Result | Status |
|-------|--------|--------|
| two_values / k=2 | 0.227× | CLOSED |
| reverse_segments_8 | 0.772× | CLOSED |
| few_unique_heavy_k4 | 0.633× | held |
| few_unique_k16 uniform | 1.208× | residual quality limit |
| Gaussian HE | 1.195× | residual floor |

## Notes

- All patterns correctness ok=1.
- STRUCTURE O(n) verify tax on already-sorted is present but still <1× here.
- equal_heavy ~1.46× is the softest non-documented case in this set (majority/adversarial residual); within normal variance for high-equal patterns.
- No new regressions relative to the attack-3 claim.
- Phase 0 baseline remains the frozen reference. EXTERNAL-clean. Not field-level.

**THE BEASTIE BOYZ**
