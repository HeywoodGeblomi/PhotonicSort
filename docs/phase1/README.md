# Phase 1 — Synthetic Attack Surface (COMPLETE)

**Status:** Families 1–3 measured; P1-5 status report locked  
**Baseline:** Phase 0 / Harvest P5.1 (frozen under `docs/phase0/`)

See **[PHASE1_STATUS.md](PHASE1_STATUS.md)** for the full scoreboard and root-cause statement.

## Headline floors (n=1e6, median-of-7)

| Family | Floors | Worst |
|--------|-------:|------:|
| 1 Continuous Gaussian | 12/12 | **1.479×** |
| 2 Cardinality / range | 5/12 | **5.694×** (`few_unique_heavy_k4`) |
| 3 Inversion / structured | 2/13 | **2.565×** (`reverse_segments_8`) |
| Few-unique hunt peak | — | **5.463×** |

**Root cause:** no efficient pure residual path for low-cardinality + wide numeric range.

Phase 0 lock untouched. No residual menu changes. Phase 1 closed for characterization. Not field-level.

**THE BEASTIE BOYZ**
