# Multi-Arch Claim Surface Results

**Date:** 2026-08-12  
**Run:** [Actions #31597972932](https://github.com/HeywoodGeblomi/PhotonicSort/actions/runs/31597972932)  
**SHA:** 1a6fdc9e (Lever A single-pass)  
**Not field-level.**

## Bootstrap CI (n=1e6, bootstrap=1000)

| Arch    | Type | geo   | ci95            | max   | soft | Gate |
|---------|------|------:|-----------------|------:|-----:|------|
| x86_64  | f64  | 0.618 | [0.499, 0.740]  | 1.123 | 0    | **PASS** |
| x86_64  | i32  | 0.382 | [0.286, 0.524]  | 0.928 | 0    | **PASS** |
| x86_64  | i64  | 0.612 | [0.483, 0.764]  | 1.058 | 0    | **PASS** |
| x86_64  | u32  | 0.434 | [0.339, 0.579]  | 1.033 | 0    | **PASS** |
| aarch64 | f64  | 0.664 | [0.523, 0.810]  | 1.017 | 0    | **PASS** |
| aarch64 | i32  | 0.369 | [0.261, 0.545]  | 0.989 | 0    | **PASS** |
| aarch64 | i64  | 0.605 | [0.469, 0.762]  | 0.977 | 0    | **PASS** |
| aarch64 | u32  | 0.359 | [0.257, 0.520]  | 0.928 | 0    | **PASS** |

## Interpretation

- soft=0 + CI95 upper <1.0 on **all types × both ISAs**.
- Claim-surface soft-spot gate **CLOSED**.
- Sole prior soft (x86_64 f64 few_k4_dense) attenuated to 1.123× via Lever A single-pass count+validate.

## Scope

Claim-surface 13-pattern set only. Expanded Field Suite multi-baseline gap map (A1) still required for path-(a) field-level entry.

**Still not field-level.**

**THE BEASTIE BOYZ**
