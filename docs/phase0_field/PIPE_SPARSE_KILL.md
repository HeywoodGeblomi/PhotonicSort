# pipe_sparse Kill + Soft-Spot Re-measure

**Date:** 2026-08-12  
**Team:** THE BEASTIE BOYZ  
**Status:** residual soft spots CLOSED under cold isolation · **NOT field-level**

## Kill mechanism

1. HE fallback: MSD → `residual_pdqsort` (full Orson Peters, zlib, namespaced)
2. Late high-disorder short-circuit (after STRUCTURE, inv ≥ 0.5 consecutive-pair sample) → `residual_pdqsort`
3. Threshold 0.5 protects organpipe structured residual wins

## Re-measure (n=1e6, median of 9, cold isolation)

| Pattern | i32 | u32 |
|---------|----:|----:|
| sorted | 0.83 | 0.62 |
| reverse | 0.41 | 0.43 |
| random | 1.04 | 0.99 |
| almost_sorted | 0.25 | 0.30 |
| db_pk | 1.03 | 1.06 |
| timestamps | 0.80 | 0.56 |
| equal_heavy | 0.90 | 0.61 |
| few_k16_dense | 0.51 | 0.69 |
| few_k16_wide | 0.89 | 0.96 |
| **pipe_sparse** | **0.94** | **0.96** |
| organpipe | 0.10 | 0.10 |
| sawtooth | 0.16 | 0.17 |
| push_middle | 0.25 | 0.30 |
| gaussianish | 1.00 | 1.01 |
| adversarial | 0.91 | 0.97 |

**No residual soft spots remain under cold isolation (all ≤1.15×).**

## Next: PO2–PO6

- PO2 Docker + reference ratios
- PO3 Multi-arch
- PO5 i64 gap map
- PO6 Docker GREEN

Field-level still requires multi-arch + CI + suite methodology GREEN.

**THE BEASTIE BOYZ**
