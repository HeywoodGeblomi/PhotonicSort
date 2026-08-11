# Family-1 Red-Gate Hits

**n=1 000 000, median-of-7, frozen pure residual (Harvest P5.1 / v1.5.1-c)**  
**Date:** 2026-08-11  
**Phase:** 1 (synthetic attack surface)  
**Family:** continuous near-uniform / multi-modal Gaussians

## Rule

Every pure/pdq ratio **≥ 1.0×** is a residual floor. Logged here with full generator id. Phase 0 lock untouched. No residual menu changes.

## Hits (all 12 Family-1 generators)

| id | pure_ms | pdq_ms | pure/pdq | severity |
|----|--------:|-------:|---------:|----------|
| gauss_skew_strong_pos | 45.907 | 31.037 | **1.479×** | worst |
| gauss_skew_strong_neg | 47.901 | 34.203 | **1.401×** | high |
| gauss_nearflat_tiny_std | 42.285 | 30.733 | **1.376×** | high |
| gauss_leptokurtic_contam | 48.624 | 36.804 | **1.321×** | high |
| gauss_nearflat_skew_neg | 39.927 | 30.463 | **1.311×** | high |
| gauss_mm2_imbalanced_70_30 | 41.556 | 31.753 | **1.309×** | high |
| gauss_mm2_imbalanced_90_10 | 42.334 | 33.434 | **1.266×** | medium |
| gauss_nearflat_skew_pos | 39.075 | 30.980 | **1.261×** | medium |
| gauss_mm3_unequal | 37.326 | 30.559 | **1.221×** | medium |
| gauss_mm2_balanced | 36.844 | 30.447 | **1.210×** | medium |
| gauss_nearflat_microstructure | 38.132 | 32.594 | **1.170×** | mild |
| gauss_mm3_equal | 35.726 | 33.389 | **1.070×** | mild |

## Summary

- **12 / 12** Family-1 generators produce pure/pdq ≥ 1.0× at n=1e6.
- Worst: strong skew-normal (**1.48×**).
- Best in family: three-mode equal (**1.07×**) — still a floor.
- Confirms and generalizes the Phase 0 gaussian **1.102×** floor across skew, multi-modal, kurtosis, and micro-structure variants.
- Pure residual HE path is the common bottleneck (no STRUCTURE / LOW_CARD / counting early-exit on these distributions).

## Phase 0 baseline citation

```
docs/phase0/PHASE0_BASELINE_LOCK.md
docs/phase0/RED_GATE_STATUS.md
```

## Next

- Keep Family-1 on the permanent floor ledger.
- Parameter surface mapped (see FAMILY1_PARAM_HUNT_LEDGER.md).
- Do **not** change residual code solely to erase these floors; document first.

**THE BEASTIE BOYZ**
