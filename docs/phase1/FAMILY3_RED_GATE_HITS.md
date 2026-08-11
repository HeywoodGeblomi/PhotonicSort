# Family-3 Red-Gate Hits — Inversion / Run-Length / Structured Disorder

**n=1 000 000, median-of-7, frozen pure residual (Harvest P5.1 / v1.5.1-c)**  
**Date:** 2026-08-11  
**Phase:** 1 Family-3  

## Hits (pure/pdq ≥ 1.0×)

| id | pure_ms | pdq_ms | pure/pdq | severity |
|----|--------:|-------:|---------:|----------|
| **reverse_segments_8** | 21.758 | 8.482 | **2.565×** | high |
| inv_density_05 | 72.020 | 38.900 | **1.851×** | high |

## Wins (pure faster)

| id | pure/pdq |
|----|---------:|
| inv_density_001 | 0.165× |
| sawtooth_noisy | 0.165× |
| inv_density_005 | 0.183× |
| runs_variable_32 | 0.230× |
| inv_density_01 | 0.255× |
| island_cluster_12 | 0.326× |
| push_middle_large_1k | 0.332× |
| organpipe_noisy | 0.403× |
| island_periodic | 0.445× |
| inv_density_15 | 0.745× |
| runs_equal_64 | 0.749× |

## Summary

- **2 / 13** Family-3 generators are residual floors.
- **High:** `reverse_segments_8` (**2.565×**) — multiple reversed segments on a sorted backbone; neither STRUCTURE early-exit nor Class-1 island residual covers this cleanly.
- **High:** `inv_density_05` (**1.851×**) — uniform 5% swaps; past the light-almost regime, before HE becomes competitive.
- Structured islands (cluster / periodic), light inversion (≤1%), push-middle, sawtooth/organpipe with noise: **pure wins strongly** (Class-1 / LowDisorder / counting paths working as designed).

## Interpretation

Family-3 mostly validates existing structured residuals. The two floors point at:
1. Multi-segment reverse patterns (not single-island Class-1).
2. Mid-band uniform inversion density (~​5%) where almost-paths lose and HE has not yet taken over.

Phase 0 lock untouched. No residual menu changes. Not field-level.

**THE BEASTIE BOYZ**
