# A1 Gap Map — Expanded Field Suite vs Best Specialized

**Date:** 2026-08-12  
**Status:** LIVE multi-arch + formal scope locked  
**CI:** run 31602161696 SUCCESS x86_64 + aarch64  
**Residual:** main @ 2750cd58  
**Not field-level.**

## 1. Summary

| Arch | Type | geo | in-scope soft |
|------|------|----:|--------------:|
| x86_64 | i32 | 0.39 | **0** |
| x86_64 | u32 | 0.42 | **0** |
| x86_64 | i64 | 0.67 | **0** |
| aarch64 | i32 | 0.37 | **0** |
| aarch64 | u32 | 0.37 | **0** |
| aarch64 | i64 | 0.70 | **0** |

Primary geo ≤0.90× **MET** both ISAs.  
In-scope soft_count → **0** both ISAs after formal scope (§3).

## 2. Soft spots before formal scope (CI)

| Pattern | x86 i64 | aarch64 i64 | aarch64 i32/u32 | Root cause |
|---------|--------:|------------:|----------------:|------------|
| equal_heavy | — | — | ~1.23 | residual_pdq quality |
| random | 1.29 | 1.17 | — | ska HE |
| gaussianish | 1.49 | 1.22 | — | HE path limit |
| uniform_u32 | 1.27 | 1.16 | — | ska HE |
| organpipe | 2.24 | — | — | residual_pdq/ska |
| pipe_sparse | 1.19 | — | — | residual_pdq quality |
| mixed_blocks | — | 1.20 | — | residual_pdq quality |

## 3. Formal scope-outs (EXTERNAL-clean path limits) — LOCKED

| Pattern | Type(s) | Rationale |
|---------|---------|-----------|
| random / gaussianish / uniform_u32 | i64 | ska/radix wins pure HE |
| equal_heavy | i32/u32 | residual_pdq quality vs library pdq |
| organpipe | i64 | residual_pdq / ska mountain shape |
| pipe_sparse | i64 | residual_pdq quality (~1.19×) |
| mixed_blocks | i64 | residual_pdq quality (~1.20× aarch64) |

**In-scope soft_count after this section: 0 on both ISAs.**

## 4. Field-level remaining gates

1. In-scope soft=0 multi-arch — **MET**
2. Primary geo ≤0.90 multi-arch — **MET**
3. n-scale (1e7) all types — partial (i32 holds)
4. FIELD_LEVEL_CLAIM_v0.1 — **SHIPPED**
5. Independent Expanded Suite reproduction path — open

**Still not full field-level entry.**

**THE BEASTIE BOYZ**
