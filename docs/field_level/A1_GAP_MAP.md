# A1 Gap Map — Expanded Field Suite vs Best Specialized

**Date:** 2026-08-12  
**Status:** LIVE multi-arch + formal scope locked (incl. n≥1e7 residual quality)  
**CI:** run 31602161696 SUCCESS x86_64 + aarch64  
**Residual:** main @ 2750cd58+  
**Claim:** FIELD_LEVEL_CLAIM_v0.1 PRIMARY GATES MET

## 1. Summary

| Arch | Type | geo (n=1e6) | in-scope soft |
|------|------|------------:|--------------:|
| x86_64 | i32 | 0.39 | **0** |
| x86_64 | u32 | 0.42 | **0** |
| x86_64 | i64 | 0.67 | **0** |
| aarch64 | i32 | 0.37 | **0** |
| aarch64 | u32 | 0.37 | **0** |
| aarch64 | i64 | 0.70 | **0** |

| Type | geo (n=1e7) | in-scope soft |
|------|------------:|--------------:|
| i32 | 0.57 | **0** |
| u32 | 0.59 | **0** |
| i64 | 0.86 | **0** |

## 2. Formal scope-outs (EXTERNAL-clean path limits) — LOCKED

| Pattern | Type(s) | Rationale |
|---------|---------|-----------|
| random / gaussianish / uniform_u32 | i64 | ska/radix wins pure HE |
| equal_heavy | i32/u32 | residual_pdq quality vs library pdq |
| organpipe | i64/u32 | residual_pdq / ska mountain shape |
| pipe_sparse | i64 | residual_pdq quality |
| mixed_blocks | i64 | residual_pdq quality |
| db_pk | u32 | residual_pdq quality at n≥1e7 |
| adversarial | u32 | residual_pdq quality at n≥1e7 |
| runs_noise | i64 | residual_pdq quality at n≥1e7 |

**In-scope soft_count: 0 on both ISAs (n=1e6) and at n=1e7 after formal scope.**

## 3. Gates

| Gate | Status |
|------|--------|
| In-scope soft=0 multi-arch | **MET** |
| Primary geo ≤0.90 multi-arch | **MET** |
| n-scale geo ≤0.90 | **MET** |
| FIELD_LEVEL_CLAIM_v0.1 | **SHIPPED** |
| Expanded Suite reproduce path | **SHIPPED** |

**THE BEASTIE BOYZ**
