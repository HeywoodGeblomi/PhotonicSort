# Phase 0 Gap Map — pure residual v2.4.1

**n=1000000** · reps=5 (median) · arch=local  
**Date:** 2026-08-11  
**Config:** EXTERNAL-clean pure residual only (no library dispatch)

Ratios = Photonic / baseline (lower is better for Photonic).

| Pattern | vs std | vs pdq | vs ska | photonic ok |
|---------|-------:|-------:|-------:|:-----------:|
| sorted | 0.074× | 0.672× | 0.024× | 1 |
| reverse | 0.082× | 0.341× | 0.034× | 1 |
| organpipe | 0.140× | 0.478× | 0.504× | 1 |
| sawtooth | 0.043× | 0.301× | 0.060× | 1 |
| almost_sorted | 0.188× | 0.388× | 0.071× | 1 |
| uniform_i64 | 0.396× | 1.077× | 1.209× | 1 |
| gauss_nearflat | 0.461× | 1.039× | 1.347× | 1 |
| two_values | 0.413× | 0.961× | 0.264× | 1 |
| few_k4_wide | 0.188× | 0.795× | 0.222× | 1 |
| few_k16_wide | 0.167× | 1.008× | 0.245× | 1 |
| few_k16_dense | 0.081× | 0.432× | 0.087× | 1 |
| zipf_k16 | 0.129× | 0.576× | 0.115× | 1 |
| reverse_segments_8 | 0.280× | 1.102× | 0.684× | 1 |
| push_middle | 0.527× | 1.171× | 0.286× | 1 |
| equal_heavy | 0.327× | 1.230× | 0.307× | 1 |
| adversarial_pipe | 0.455× | 0.509× | 0.526× | 1 |
| db_pk | 1.393× | **2.335×** | 0.628× | 1 |
| timestamps | 1.355× | **1.708×** | 0.925× | 1 |

**Geo-mean Photonic/std = 0.240× · Photonic/pdq = 0.769× · Photonic/ska = 0.241×**

## Soft spots (drive Phase 1)

| Pattern | vs pdq | Diagnosis |
|---------|-------:|----------|
| db_pk | 2.34× | Near-monotonic with sparse gaps — residual routing miss or HE path on almost-sorted-like data |
| timestamps | 1.71× | Near-monotonic + jitter — similar |
| equal_heavy | 1.23× | Majority residual variance |
| push_middle | 1.17× | 3-run merge residual quality |
| reverse_segments_8 | 1.10× | Borderline |
| uniform_i64 / gauss_nearflat | ~1.04–1.08× | Known HE residual / path limit |

## Strengths

Structured (sorted/reverse/almost/sawtooth/organpipe), dense few-unique, zipf, adversarial_pipe — strong wins vs pdq.

## Non-claims

- Not field-level.
- Single machine, single arch, synthetic + DB-shaped generators only.
- Gap map only — does not claim displacement.

**THE BEASTIE BOYZ**
