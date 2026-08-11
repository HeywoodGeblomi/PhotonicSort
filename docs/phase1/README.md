# Phase 1 — Synthetic Attack Surface

**Status:** OPEN (Family-1 complete; parameter hunt mapped)  
**Baseline:** Phase 0 / Harvest P5.1 (frozen under `docs/phase0/`)

## Family-1 (continuous Gaussian / multi-modal)

- 12 fixed-seed generators — **12/12 residual floors** at n=1e6
- Worst fixed-seed: `gauss_skew_strong_pos` **1.479×** pure/pdq
- Parameter hunt: floors are **broad** across skew, contamination, multi-modal separation, near-flat std
- Measurement variance is high on peak magnitude; fixed-seed suite is the citable ledger

### Artifacts

| File | Role |
|------|------|
| `FAMILY1_RESULTS.csv` | Fixed 12-generator n=1e6 median-of-7 |
| `FAMILY1_RED_GATE_HITS.md` | Floor ledger for the 12 |
| `FAMILY1_PARAM_HUNT_LEDGER.md` | Parameter-surface map + honesty note |
| `registry.yaml` | Generator registry schema + Family-1 entries |

Phase 0 lock untouched. No residual menu changes. Not field-level.

**THE BEASTIE BOYZ**
