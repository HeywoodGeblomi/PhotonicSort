# Phase 1 Status Report — Synthetic Attack Surface

**Date:** 2026-08-11  
**Baseline residual:** Harvest P5.1 / v1.5.1-c (frozen)  
**Phase 0 lock:** intact (`docs/phase0/`)  
**Claim language:** Not a field-level breakthrough beyond the published suite.

---

## Scoreboard — fixed-seed floors (n=1e6, median-of-7)

### Family-1 — Continuous near-uniform / multi-modal Gaussians

| Result | Value |
|--------|------:|
| Floors | **12 / 12** |
| Worst | `gauss_skew_strong_pos` **1.479×** |
| Best in family | `gauss_mm3_equal` **1.070×** |

All continuous Family-1 patterns route to the HE residual. Generalizes the Phase 0 gaussian **1.102×** floor across skew, multi-modal, kurtosis, and micro-structure.

### Family-2 — Cardinality / range mixes

| Result | Value |
|--------|------:|
| Floors | **5 / 12** |
| Critical | `few_unique_heavy_k4` **5.694×** |
| Other floors | two_values_split 1.431×, card_cliff_low_to_high 1.414×, range_dense_then_sparse 1.136×, range_power_bits_24 1.102× |
| Strong wins | range_power_bits_16 0.156×, ascending_card 0.164×, equal_runs_noise 0.194× |

### Few-unique parameter hunt (45 points)

| Result | Value |
|--------|------:|
| Peak in hunt | **5.463×** (k=4, α=1.2, wide spacing) |
| Routing | MAJORITY_ADVERSARIAL or HE_OR_OTHER when keys are widely spaced |
| Recovery regime | Compact key spacing → COUNTING fires → pure **wins** (0.3–0.4×) |

---

## Root cause (citable)

**The pure residual menu has no efficient route for low-cardinality inputs over a wide numeric range.**

1. **Counting residual** rejects when sample range is large (`srange` gates). Zipf keys spaced by ≥ ~1e6 fail the gate.
2. **Majority / adversarial residual** fires on high equal-rate samples but lags pdqsort (~​1.2–5×) on these patterns.
3. **HE MSD** is the fallback for flatter Zipf / larger k with wide keys — also multi-fold slower when true cardinality is tiny.
4. **Compact spacing** is the only regime where counting wins cleanly.

This is a residual-menu gap, not a probe failure. Documented under `docs/phase1/FEW_UNIQUE_ROOT_CAUSE.md`.

---

## Artifacts (public on main)

```
docs/phase1/
  README.md
  FAMILY1_RESULTS.csv
  FAMILY1_RED_GATE_HITS.md
  FAMILY1_PARAM_HUNT_LEDGER.md
  FAMILY2_RESULTS.csv
  FAMILY2_RED_GATE_HITS.md
  FEW_UNIQUE_HUNT.csv
  FEW_UNIQUE_HUNT_LEDGER.md
  FEW_UNIQUE_ROOT_CAUSE.md
  PHASE1_STATUS.md          ← this file
```

---

## Phase 1 ticket status

| ID | Deliverable | Status |
|----|-------------|--------|
| P1-1 | Registry schema | GREEN |
| P1-2 | ≥120 generators (Family-1 + Family-2 = 24 fixed-seed) | PARTIAL — taxonomy open, critical floors mapped |
| P1-3 | Residual hunting harness | GREEN (Family-1 grid + few-unique 45-point) |
| P1-4 | Floor-ledger automation | MANUAL — ledgers public |
| P1-5 | Phase 1 status report | **GREEN** (this document) |

---

## Non-claims (reaffirmed)

- Not a field-level breakthrough beyond the published suite.
- Phase 0 baseline remains the frozen reference.
- Pure residual contract unchanged (no library dispatch, EXTERNAL-clean, O(n log n) worst-case).
- Residual menu was **not** modified in Phase 1; floors are documented, not erased.
- Measurement variance exists on peak magnitudes; fixed-seed suites are the citable evidence.

---

## Recommended next

**Open Family-3** — inversion-density, run-length, and structured-disorder generators — under the same registry discipline. The few-unique root cause is mapped; expanding coverage is higher leverage than further hunting inside the same narrow region.

**THE BEASTIE BOYZ**
