# Pure residual freeze — 2026-08-10

**Status:** Pure HE residual and full-menu baseline frozen. Gates red. Not field-level.

## What is frozen

- **HE residual:** classical MSD, fixed 8-bit digits, blocked scatter (BLOCK=256) + write-side prefetch, INS=192
- **Routing:** majority_v2 + sparse-cluster detector
- **Untouched:** STRUCTURE O(n) verify, Class 1 inv gate, almost / LowDisorder contracts

## Measured baseline (n=1e6, trials=7, best-of)

| Metric | Value |
|--------|------:|
| geo pure / pdqsort | **0.673×** |
| HE pure / ska (full menu) | **1.105×** |
| HE pure / ska (isolated residual band) | **~1.02–1.09×** |
| equal_heavy pure / pdq | **0.993×** |
| adversarial_pivot pure / pdq | **1.814×** |
| db_pk_sparse pure / pdq | **2.563×** |

## Language

Best pure HE residual to date (~1.02–1.09× isolated). Residual floors documented. Gates red. Not field-level.
Pure residual work stopped. External leverage only from this point.

**THE BEASTIE BOYZ**
