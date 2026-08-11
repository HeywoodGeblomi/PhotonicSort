# Family-2 Red-Gate Hits — Cardinality / Range Mixes

**n=1 000 000, median-of-7, frozen pure residual (Harvest P5.1 / v1.5.1-c)**  
**Date:** 2026-08-11  
**Phase:** 1 Family-2  

## Hits (pure/pdq ≥ 1.0×)

| id | pure_ms | pdq_ms | pure/pdq | severity |
|----|--------:|-------:|---------:|----------|
| **few_unique_heavy_k4** | 11.446 | 2.010 | **5.694×** | **critical** |
| two_values_split | 1.925 | 1.345 | **1.431×** | high |
| card_cliff_low_to_high | 20.134 | 14.243 | **1.414×** | high |
| range_dense_then_sparse | 24.908 | 21.929 | **1.136×** | medium |
| range_power_bits_24 | 35.173 | 31.922 | **1.102×** | mild |

## Wins (pure faster)

| id | pure/pdq |
|----|---------:|
| range_power_bits_16 | 0.156× |
| ascending_card | 0.164× |
| equal_runs_noise | 0.194× |
| all_equal | 0.477× |
| range_gap_sparse | 0.524× |
| hierarchical_pk | 0.741× |
| card_cliff_high_to_low | 0.846× |

## Summary

- **5 / 12** Family-2 generators are residual floors.
- **Critical:** `few_unique_heavy_k4` (Zipf-ish 4 values) at **5.69×** — worst floor found in Phase 1 so far.
- Counting / majority residual boundaries are the pressure points.
- Restricted 16-bit range and ascending cardinality remain strong pure wins.

## Phase 0 citation

```
docs/phase0/PHASE0_BASELINE_LOCK.md
docs/phase0/RED_GATE_STATUS.md
```

Phase 0 lock untouched. No residual menu changes. Not field-level.

**THE BEASTIE BOYZ**
