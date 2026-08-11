# Frozen Menu Baseline Snapshot

**Date:** 2026-08-10  
**Residual:** Classical MSD + blocked scatter + prefetch (frozen)  
**Routing:** majority_v2 + sparse-cluster + Class1 inv gate + STRUCTURE  
**Language:** Best pure HE residual to date (~1.02–1.09× isolated). Residual floors documented. Gates red. Not field-level.

---

## 1. Probe overhead on pure uniform (n=1e6, best-of-7)

| Stage | ms |
|-------|---:|
| probe_only | ~0.00 (stratified sample; below timer noise) |
| residual_only (MSD) | 22.49 |
| full pure_adaptive | 22.20 |
| ska | 21.89 |
| residual/ska | **1.027×** |
| full/ska | **1.014×** |

**Finding:** On pure uniform the probe is not a material tax. Earlier 1.022× isolated vs ~1.10× full-menu gap was largely cross-run variance, not a fixed O(n) probe cost. No residual change. No STRUCTURE safety change.

---

## 2. Full-suite snapshot (n=1e6, trials=7, best-of)

| Pattern | pure | pdq | ska | std | pure/pdq | pure/ska |
|---------|-----:|----:|----:|----:|---------:|---------:|
| uniform_i64 | 23.85 | 30.22 | 21.58 | 79.15 | **0.789×** | **1.105×** |
| sorted | 0.36 | 0.79 | 22.15 | 11.20 | **0.461×** | 0.016× |
| reverse | 0.65 | 1.53 | 24.21 | 7.87 | **0.427×** | 0.027× |
| almost_0.001 | 2.74 | 5.74 | 28.67 | 11.22 | **0.477×** | 0.095× |
| almost_0.01 | 4.44 | 10.11 | 30.43 | 13.43 | **0.440×** | 0.146× |
| organpipe | 17.54 | 31.12 | 26.40 | 86.64 | **0.564×** | 0.664× |
| sawtooth_64 | 1.50 | 3.84 | 19.04 | 24.54 | **0.392×** | 0.079× |
| few_unique_16 | 1.51 | 3.93 | 19.13 | 21.14 | **0.385×** | 0.079× |
| adversarial_pivot | 1.44 | 0.79 | 22.09 | 10.74 | **1.814×** | 0.065× |
| equal_heavy | 0.75 | 0.75 | 22.62 | 10.77 | **0.993×** | 0.033× |
| db_pk_sparse | 76.58 | 29.87 | 41.23 | 76.51 | **2.563×** | 1.857× |

### Summary

| Metric | Value |
|--------|------:|
| geo pure/pdq | **0.673×** |
| min pure/pdq | 0.385× |
| geo pure/std | **0.144×** |
| HE pure/ska (full menu) | **1.105×** |
| HE pure/ska (isolated residual band) | **~1.02–1.09×** |

All correctness OK.

---

## 3. Documented residual floors

| Pattern | pure/pdq | Status |
|---------|---------:|--------|
| HE vs ska | ~1.02–1.11× | best pure HE to date; ≤0.90× **not met** |
| equal_heavy | **0.993×** | near parity |
| adversarial_pivot | **1.814×** | pure residual floor accepted |
| db_pk_sparse | **2.563×** | pure residual floor accepted |

---

## 4. Stand down

- Pure HE residual **frozen** (classical MSD + blocked scatter + prefetch).
- No further pure scatter/permute or residual-structure work this cycle.
- Probe is not the binding constraint on uniform.
- External leverage required to reopen HE ≤0.90× or the adversarial/db_pk floors.

**Gates: RED. Not field-level.**

**THE BEASTIE BOYZ**
