# Harvest P4 — Unified pure residual menu + full suite

**Status:** ALL HARVEST GATES MET  
**Date:** 2026-08-11

## Pure residual menu (EXTERNAL-clean, no library dispatch)

1. **STRUCTURE** — sorted early-exit / reverse in-place  
2. **Majority** (`residual_adversarial_i64`) — Dutch-flag around sampled mode  
3. **Low-card counting** — sample unique estimate + range → counting sort  
4. **Sparse** (`residual_sparse_i64`) — pure MSD INS=64 BLOCK=256  
5. **Identity-almost** — misplaced-index recovery for near-identity permutations  
6. **HE** (`residual_he_msd_i64`) — classical MSD INS=96 BLOCK=512  

Selection uses only visible probe metrics. No hidden state. No calls to pdqsort / ska_sort / std::sort on the pure path.

## Full-suite results (n=1e6, 7-trial median)

| Pattern | pure ms | pdq ms | ska ms | pure/pdq | pure/ska |
|---------|--------:|-------:|-------:|---------:|---------:|
| uniform_i64 | 22.78 | 32.68 | 22.70 | **0.697×** | **1.003×** |
| sorted | 0.74 | 0.91 | 24.71 | **0.814×** | 0.030× |
| reverse | 1.05 | 1.64 | 24.21 | **0.642×** | 0.044× |
| almost_0.001 | 1.53 | 5.67 | 28.91 | **0.271×** | 0.053× |
| almost_0.01 | 3.12 | 9.72 | 30.80 | **0.321×** | 0.101× |
| organpipe | 17.62 | 33.55 | 27.74 | **0.525×** | 0.635× |
| sawtooth_64 | 1.33 | 4.19 | 19.86 | **0.318×** | 0.067× |
| few_unique_16 | 1.69 | 5.76 | 20.12 | **0.293×** | 0.084× |
| adversarial_pivot | 0.77 | 0.91 | 22.65 | **0.856×** | 0.034× |
| equal_heavy | 0.82 | 0.88 | 23.02 | **0.924×** | 0.035× |
| db_pk_sparse | 28.79 | 32.17 | 45.17 | **0.895×** | 0.637× |

## Gate check

| Gate | Target | Result | Status |
|------|--------|-------:|:------:|
| db_pk_sparse pure/pdq | ≤ 0.95× | 0.895× | **MET** |
| adversarial pure/pdq | ≤ 0.95× | 0.856× | **MET** |
| HE isolated pure/ska | ≤ 0.92× | 0.916× median (P2) | **MET** |
| full-suite geo pure/pdq | ≤ 0.55× | **0.538×** | **MET** |
| zero major regressions | held | 0 | **MET** |

## Comparison to prior freeze

| Metric | Prior freeze | Harvest pure menu |
|--------|-------------:|------------------:|
| geo pure/pdq | 0.673× | **0.538×** |
| db_pk pure/pdq | 2.56× | **0.90×** |
| adversarial pure/pdq | 1.81× | **0.86×** |
| HE pure/ska (isolated) | ~1.02–1.09× | **0.92×** |
| Library dispatch on pure path | std::sort on sparse | **none** |

**Not a field-level breakthrough beyond this suite.**

**THE BEASTIE BOYZ**
