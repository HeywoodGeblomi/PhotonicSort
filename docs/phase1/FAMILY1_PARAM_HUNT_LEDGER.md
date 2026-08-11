# Family-1 Parameter Hunt Ledger

**Date:** 2026-08-11  
**Baseline residual:** Harvest P5.1 / v1.5.1-c (frozen)  
**Explore:** n=200k, median-of-5 (49 grid points)  
**Confirm:** n=1e6, median-of-7..11  

## Honesty note on measurement variance

Absolute wall-times and single-shot ratios on this host show **high run-to-run variance** (thermal / cache / scheduler). The same parameter point can report 1.4× on one confirm and 2.5× (or occasionally <1.0×) on another.

**What is robust:**
- Continuous Family-1 distributions **frequently** produce pure/pdq ≥ 1.05×.
- The original fixed-seed 12-generator suite (FAMILY1_RESULTS.csv) is the cleaner floor evidence.
- Parameter space is **not** a narrow singularity — floors appear across skew, contamination, multi-modal separation, and near-flat std.

**What is not robust:**
- Exact ranking of “worst parameter” or peak magnitude beyond ~1.5× without multi-host / longer-trial methodology.

## Explore grid hits (n=200k, ratio ≥ 1.05×) — signal map

### skew_alpha
Hits at α ∈ {−8, −6, −4, −3, −1, 0, +1, +2, +3, +5, +6, +8}.  
Sparse misses at α ∈ {−5, −2, +4}. Floor is **broad** across skew, not a single spike.

### contam_frac
Hits at frac ∈ {0.01, 0.02, 0.05, 0.15}.  
Misses / wins at {0.08, 0.10, 0.20, 0.30}. Intermediate contamination is the danger zone.

### mm2 sep × weight
Largest explore signals: sep=8e9 w0=0.9; sep=2e9 w0=0.7; sep=8e9 w0=0.5.  
Balanced moderate separation sometimes wins (pure < pdq).

### nearflat_std
Hits when std ≥ 1e6. Very small std (1e4–5e5) often favors pure (counting / structure-like behavior).

## Confirmed n=1e6 samples (illustrative, noisy)

| axis | params | observed ratio range | notes |
|------|--------|---------------------:|-------|
| contam_frac | 0.01 | ~1.4–2.4× | repeated floor |
| contam_frac | 0.05 | 0.56–3.6× | **high variance** |
| skew_alpha | −6 | ~1.2–2.2× | repeated floor |
| mm2 | sep=2e9, w0=0.7 | ~1.1–3.4× | high variance |
| mm2 | sep=8e9, w0=0.5 | ~1.2–1.9× | repeated floor |
| nearflat | std=1e7 | ~1.06× | mild floor |

## Primary floor claim (citable)

Use the **fixed 12-generator Family-1 suite** as the public floor ledger:

- 12/12 ≥ 1.07× at n=1e6 median-of-7 (see FAMILY1_RESULTS.csv)
- Worst fixed-seed point: `gauss_skew_strong_pos` **1.479×**
- Parameter hunt shows the floor region is **wide**, not an isolated seed artifact

## Phase 0 citation

```
docs/phase0/PHASE0_BASELINE_LOCK.md
docs/phase0/RED_GATE_STATUS.md
```

Phase 0 lock untouched. No residual menu changes. Not field-level.

**THE BEASTIE BOYZ**
