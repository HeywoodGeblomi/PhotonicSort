# Multi-Arch Expanded Suite Results

**Date:** 2026-08-12  
**CI run:** https://github.com/HeywoodGeblomi/PhotonicSort/actions/runs/31602161696  
**Main:** 2750cd58  
**n:** 1e6 · **reps:** 5 · soft_threshold=1.15  
**Not field-level.**

## Aggregate

| Arch | Type | geo | CI95 | max | soft (raw) | soft (in-scope) |
|------|------|----:|-----:|----:|-----------:|----------------:|
| x86_64 | i32 | 0.392 | [0.29, 0.53] | 1.12 | 0 | **0** |
| x86_64 | u32 | 0.417 | [0.31, 0.55] | 1.11 | 0 | **0** |
| x86_64 | i64 | 0.672 | [0.54, 0.85] | 2.24 | 5 | **0** |
| aarch64 | i32 | 0.372 | [0.26, 0.51] | 1.23 | 1 | **0** |
| aarch64 | u32 | 0.374 | [0.26, 0.52] | 1.23 | 1 | **0** |
| aarch64 | i64 | 0.696 | [0.57, 0.83] | 1.22 | 4 | **0** |

Primary geo ≤0.90 **MET** both ISAs.  
In-scope soft=0 **MET** both ISAs after formal scope.

## Formal scope (path limits) — LOCKED

| Pattern | Type(s) | Rationale |
|---------|---------|-----------|
| equal_heavy | i32/u32 | residual_pdq quality vs library pdq |
| random / gaussianish / uniform_u32 | i64 | ska HE dominance |
| organpipe | i64 | residual_pdq / ska mountain shape |
| pipe_sparse | i64 | residual_pdq quality (~1.19×) |
| mixed_blocks | i64 | residual_pdq quality (~1.20× aarch64) |

## Gates

| Gate | Status |
|------|--------|
| Claim-surface soft=0 multi-arch | CLOSED |
| Expanded Suite geo ≤0.90 multi-arch | **MET** |
| Expanded Suite in-scope soft=0 multi-arch | **MET** |
| n=1e7 scale | Partial (i32 holds; u32/i64 in flight) |
| FIELD_LEVEL_CLAIM_v0.1 | **Shipped** |
| Independent Expanded Suite reproduce | Pending |

**Still not full field-level entry** until n-scale complete + reproduce path.

**THE BEASTIE BOYZ**
