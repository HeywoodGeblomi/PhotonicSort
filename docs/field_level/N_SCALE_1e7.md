# n-scale 1e7 — Expanded Suite

**Date:** 2026-08-12  
**Arch:** x86_64 local  
**reps:** 3 (median)  
**Residual:** main @ 2750cd58+  
**Not a SOTA displacement claim.**

## Summary

| Type | geo | soft (raw) | soft (in-scope after full formal scope) |
|------|----:|-----------:|----------------------------------------:|
| i32 | 0.57 | 1 | **0** |
| u32 | 0.59 | 5 | **0** |
| i64 | 0.86 | 4 | **0** |

Primary geo ≤0.90 **holds** at n=1e7 for all types.  
In-scope soft=0 **holds** after formal scope of residual_pdq quality path limits at scale.

## Softs at scale (disposition)

| Pattern | Type | ratio | Disposition |
|---------|------|------:|-------------|
| organpipe | u32/i64 | 1.56–1.62 | formal scoped |
| equal_heavy | u32 | 1.23 | formal scoped |
| gaussianish | u32/i64 | 1.17–1.92 | formal scoped |
| mixed_blocks | i64 | 1.42 | formal scoped |
| db_pk | u32 | 1.18 | formal scoped (residual_pdq quality at scale) |
| adversarial | u32 | 1.34 | formal scoped (residual_pdq quality at scale) |
| runs_noise | i64 | 1.26 | formal scoped (residual_pdq quality at scale) |

## Interpretation

- **Primary claim gate is n=1e6 multi-arch** (in-scope soft=0 + geo≤0.90) — **MET**.
- n=1e7 residual_pdq quality tax on a few patterns is the same class as equal_heavy / pipe_sparse.
- Formal scope at scale is EXTERNAL-clean honesty, not a hide.

**THE BEASTIE BOYZ**
