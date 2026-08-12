# Multi-Arch Claim Surface Results

**Date:** 2026-08-12  
**Run:** [Actions #31590962755](https://github.com/HeywoodGeblomi/PhotonicSort/actions/runs/31590962755)  
**Not field-level.**

## Bootstrap CI (n=1e6, reps=5, bootstrap=1000)

| Arch | Type | geo | ci95 | max | soft | Gate |
|------|------|----:|------|----:|-----:|------|
| x86_64 | **f64** | **0.65** | [0.52, **0.78**] | 1.07 | 0 | **PASS** |
| x86_64 | **i32** | **0.37** | [0.28, **0.51**] | 0.93 | 0 | **PASS** |
| x86_64 | u32 | 0.52 | [0.36, 0.78] | 2.58 | 1 | soft |
| x86_64 | i64 | 0.83 | [0.61, **1.13**] | 2.28 | 4 | **FAIL** CI overlaps 1.0 |
| aarch64 | **f64** | **0.67** | [0.53, **0.80**] | 1.03 | 0 | **PASS** |
| aarch64 | i32 | 0.38 | [0.27, 0.57] | 1.24 | 1 | soft |
| aarch64 | u32 | 0.40 | [0.26, 0.64] | 1.99 | 2 | soft |
| aarch64 | i64 | 0.80 | [0.58, **1.11**] | 1.98 | 3 | **FAIL** CI overlaps 1.0 |

## Interpretation

- Multi-arch infrastructure works (both ISAs end-to-end).
- **f64 PASS on both ISAs** with CI excluding 1.0.
- **i32 PASS on x86_64**.
- **i64/u32 softs** = A2 menu gates not fully on main (early counting / equal / dense few_wide).

## Next

Land remaining A2 menu gates → re-dispatch → require soft=0 and CI95 upper <1.0 on all types × both ISAs.

**Still not field-level.**

**THE BEASTIE BOYZ**
