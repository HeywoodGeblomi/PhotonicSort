# i64 Soft Spots CLOSED

**Date:** 2026-08-12  
**Team:** THE BEASTIE BOYZ  
**Status:** ALL residual soft spots CLOSED on i64 · **NOT field-level**

## Closures (n=1e6, median of 7)

| pattern | before | after | mechanism |
|---------|-------:|------:|-----------|
| few_k4_dense | 1.56× | **1.00×** | FEW_WIDE only when wide; dense → counting |
| push_middle | 3.24× | **1.03×** | direction-change run gate → residual_pdqsort |
| reverse_segments | 1.33× | **0.83×** | same run gate → residual_pdqsort |

**i64 geo mean:** 0.76× · **max:** 1.10× (gaussianish) · **soft count:** 0

## Mechanism

1. `residual_few_wide_i64::should_try` — `sample_u ≤ 4` requires wide range.
2. Direction-change run gate in `sort_i64` — 2..64 runs → `residual_pdqsort`. Cap 64 skips HE random.

## Non-claims

- Not field-level.
- residual_pdqsort is pure residual (EXTERNAL-clean).

**THE BEASTIE BOYZ**
