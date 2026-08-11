# Pure Residual Menu

**Current engineering baseline:** Residual-Improvement v2.4.1 + Phase 1 low_disorder  
See [`docs/residual_improve/BASELINE_LOCK.md`](../docs/residual_improve/BASELINE_LOCK.md)

**Historical freeze:** Harvest P5.1 / Phase 0 (geo 0.500× on 38-pattern)  
See [`docs/phase0/`](../docs/phase0/) and [`docs/harvest/HARVEST_P5.1_FREEZE.md`](../docs/harvest/HARVEST_P5.1_FREEZE.md)

## Menu order (Phase 1)

constant probe → early FEW_WIDE → STRUCTURE → reverse-runs → FEW_WIDE → counting → majority → 3-run merge → **low_disorder** → sparse → identity-almost → HE MSD

## Phase 1 soft spots (closed)

| Pattern | Before | After | Residual |
|---------|-------:|------:|----------|
| db_pk | 2.34× | **1.12×** | low_disorder introsort |
| timestamps | 1.71× | **0.43×** | low_disorder insertion |

## Residual limits (documented)

- few_unique_k16 ≈ 1.0–1.2× (residual quality limit)
- Gaussian HE ≈ 1.1–1.2× full-menu (residual quality / path limit; isolated MSD competitive)
- push_middle ≈ 1.5–1.7× (3-run merge residual quality limit)

See [`docs/residual_improve/GAUSSIAN_HE_PHASE_CLOSE.md`](../docs/residual_improve/GAUSSIAN_HE_PHASE_CLOSE.md) and [`docs/phase0_field/`](../docs/phase0_field/).

EXTERNAL-clean. Pure residual only. Not field-level.

**THE BEASTIE BOYZ**
