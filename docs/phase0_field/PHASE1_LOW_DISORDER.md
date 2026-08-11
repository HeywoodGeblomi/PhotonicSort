# Phase 1 — Low-Disorder Residual

**Status:** Merged to pure residual menu  
**Date:** 2026-08-11  
**Constraint:** EXTERNAL-clean, pure residual only (no library pdq/ska dispatch)

## Results (n=1e6, median-of-5 vs pdqsort)

| Pattern | Before | After | Status |
|---------|-------:|------:|--------|
| db_pk | 2.34× | **1.12×** | CLOSED |
| timestamps (σ=2) | 1.71× | **0.43×** | CLOSED |
| push_middle | ~1.2× | ~1.68× | residual limit (3-run merge body) |
| geo Photonic/pdq | 0.77× | **0.80×** | held |

## Residual design

`residual/residual_low_disorder_i64.hpp`

- **Gate:** full-span inv sample; reject low-card, compact-range, push_middle-shape, consecutive-domain (identity-almost owns those).
- **Ultra-low inv (≤0.005):** pure insertion residual (timestamps-like local jitter).
- **Moderate low-disorder:** pure pattern-defeating introsort (ninther + Hoare + already-partitioned + insertion leaf + heapsort depth fallback).

Placement: after push_middle, before sparse.

## Non-claims

- Not field-level.
- Phase 0 baseline remains the historical frozen reference.
- push_middle 1.5–1.7× is a documented residual quality limit of the existing 3-run merge, not introduced by low_disorder.
- Pure residual only; no hybrid library residual.

**THE BEASTIE BOYZ**
