# Residual-Improvement Phase

Post-Harvest pure residual attack cycle (2026-08-11).  
Closes targeted residual floors on top of Phase 0 / Harvest P5.1.

## Status

**Baseline locked** — see [`BASELINE_LOCK.md`](./BASELINE_LOCK.md)

## Menu (v2.4.1)

constant probe → early FEW_WIDE → STRUCTURE → reverse-runs → FEW_WIDE → counting → majority → 3-run merge → sparse → identity-almost → HE MSD

## Key results

| Floor | Result | Status |
|-------|--------|--------|
| two_values / k=2 | 0.42–0.47× | CLOSED |
| reverse_segments_8 | 0.85–0.93× | CLOSED |
| few_unique_heavy_k4 | ~0.8–0.9× | held |
| few_unique_k16 | ~1.0–1.2× | residual quality limit |
| Gaussian HE | ~1.1–1.3× | residual floor (deferred) |

16-pattern post-merge geo **0.648×**. Full extended surface ~0.55–0.67× (indicative).

## Documents

- [`BASELINE_LOCK.md`](./BASELINE_LOCK.md) — engineering freeze
- [`ATTACK_3_FLOORS_RESULT.md`](./ATTACK_3_FLOORS_RESULT.md)
- [`POST_MERGE_SUITE_RESULT.md`](./POST_MERGE_SUITE_RESULT.md)
- [`FEW_WIDE_RESULT.md`](./FEW_WIDE_RESULT.md)
- [`PHASE1_REMEASURE_REPORT.md`](./PHASE1_REMEASURE_REPORT.md)

Phase 0 remains the historical frozen reference. EXTERNAL-clean. Not field-level.

**THE BEASTIE BOYZ**
