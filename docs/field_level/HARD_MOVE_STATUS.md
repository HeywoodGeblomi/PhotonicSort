# Hard Move Status — Soft Gate CLOSED

**Date:** 2026-08-12  
**Main:** 1a6fdc9e  
**Not field-level.**

## Landed

1. GitHub Actions claim-surface matrix (x86_64 + aarch64)
2. Harness scripts/claim_surface_bench.cpp (i32/u32/i64/f64)
3. Bootstrap via field_metrics.py --bootstrap 1000
4. Lever A single-pass count+validate for range≤4
5. **soft=0 + CI95 upper <1.0 on ALL types × BOTH ISAs** (run 31597972932)

## Final claim-surface numbers (n=1e6)

| Arch | Type | geo | CI95 upper | soft |
|------|------|----:|-----------:|-----:|
| x86_64 | f64 | 0.618 | 0.740 | 0 |
| x86_64 | i32 | 0.382 | 0.524 | 0 |
| x86_64 | i64 | 0.612 | 0.764 | 0 |
| x86_64 | u32 | 0.434 | 0.579 | 0 |
| aarch64 | f64 | 0.664 | 0.810 | 0 |
| aarch64 | i32 | 0.369 | 0.545 | 0 |
| aarch64 | i64 | 0.605 | 0.762 | 0 |
| aarch64 | u32 | 0.359 | 0.520 | 0 |

## Next hard move

A1 multi-baseline gap map on Expanded Field Suite. No further residual micro-work on claim-surface softs.

**THE BEASTIE BOYZ**
