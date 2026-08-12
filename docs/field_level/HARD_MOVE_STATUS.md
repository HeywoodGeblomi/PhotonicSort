# Hard Move Status — Multi-Arch CI + Bootstrap

**Date:** 2026-08-12  
**Not field-level.**

## Landed

1. **GitHub Actions** `.github/workflows/claim-surface.yml` — x86_64 + aarch64 matrix
2. **Harness** `scripts/claim_surface_bench.cpp` — i32/u32/i64/f64 claim surface → CSV
3. **Reproduction** `reproduce/CLAIM_SURFACE.md`
4. Bootstrap via `scripts/field_metrics.py --bootstrap 1000`

## Local x86_64 i64 (n=1e6)

geo≈0.74–0.78, bootstrap CI95 upper ≈1.00 (near-exclusion of 1.0).

## Open gate

aarch64 Actions run must complete PASS. Until then path-(a) entry is blocked.

**THE BEASTIE BOYZ**
