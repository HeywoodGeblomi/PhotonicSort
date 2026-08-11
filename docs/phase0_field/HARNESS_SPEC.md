# Phase 0 — Multi-Baseline Harness Specification

**Status:** IMPLEMENTED (initial)  
**Parent:** [`FIELD_LEVEL_ATTACK_PLAN.md`](../FIELD_LEVEL_ATTACK_PLAN.md)  
**Companion:** [`SUITE_DESIGN.md`](./SUITE_DESIGN.md)

## Purpose

One-command / Docker-reproducible harness measuring pure residual v2.4.1 against sequential SOTA baselines on the expanded suite.

## Baselines (current)

| Baseline | Status |
|----------|--------|
| std::sort | live |
| pdqsort | live (vendored) |
| ska_sort | live (vendored) |
| fluxsort / IPS4o / vqsort | not yet |

## Interface

```bash
./harness --n 1000000 --reps 7 --out results --arch-tag zen3
```

## Statistical protocol

Warm-up + median of odd reps; full correctness verify every run.

## Acceptance

Gap map published (`PHASE0_GAP_MAP.md`). All ok=1 on standard 18-pattern suite @ n=1e6.

**THE BEASTIE BOYZ**
