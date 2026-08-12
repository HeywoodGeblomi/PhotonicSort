# A2 Menu Gates Landed on Main Path

**Date:** 2026-08-12  
**Not field-level.**

## Changes

1. **i64** `pure_residual_menu.hpp`
   - Early equal-heavy → residual_pdqsort (before run tax)
   - Early counting after STRUCTURE (before direction-change residual_pdqsort)
   - Ultra-low inv + non-consecutive domain → residual_pdqsort
   - Consecutive-domain reject in try_counting (`range+1 >= n`)
   - eq-sample gate → residual_pdqsort

2. **u32/i32** menus
   - STRUCTURE before counting
   - Early equal before counting (pdq beats counting on block-equal)
   - Early counting after STRUCTURE
   - Consecutive-domain reject

3. **u32 few_wide** dense gate: `sample_u <= 4` only if wide (dense → counting)

## Local x86_64 (n=1e6, reps=7)

| Type | geo | CI95 upper | soft |
|------|----:|----------:|-----:|
| u32 | 0.48 | 0.64 | **0** |
| i64 | 0.72 | 0.89 | 1 (push_middle variance) |
| i32 | 0.48 | 0.68 | 2 (equal_heavy/db_pk residual limit) |

Re-dispatch claim-surface for multi-arch verdict.

**THE BEASTIE BOYZ**
