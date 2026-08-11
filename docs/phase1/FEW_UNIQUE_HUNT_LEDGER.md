# Few-Unique Parameter Hunt Ledger

**n=1000000, median-of-7, frozen pure residual (Harvest P5.1)**
**Baseline critical floor:** few_unique_heavy_k4 = 5.694×

## Hits (ratio ≥ 1.0×), sorted by severity

| k | alpha | spacing | dom_boost | pure_ms | pdq_ms | ratio | path |
|--:|------:|--------:|----------:|--------:|-------:|------:|------|
| 4 | 1.2 | 1000000 | 0.0 | 18.105 | 3.314 | **5.463×** | HE_OR_OTHER |
| 3 | 1.0 | 1000000 | 0.0 | 12.803 | 2.366 | **5.410×** | MAJORITY_ADVERSARIAL |
| 4 | 1.0 | 1000000 | 0.0 | 28.574 | 5.446 | **5.247×** | HE_OR_OTHER |
| 64 | 1.5 | 1000000 | 0.0 | 23.442 | 4.650 | **5.041×** | HE_OR_OTHER |
| 8 | 2.5 | 1000000 | 0.0 | 26.806 | 5.817 | **4.608×** | MAJORITY_ADVERSARIAL |
| 4 | 1.5 | 1000000 | 0.0 | 11.861 | 2.816 | **4.213×** | MAJORITY_ADVERSARIAL |
| 4 | 0.8 | 1000000 | 0.0 | 14.088 | 3.510 | **4.014×** | HE_OR_OTHER |
| 8 | 1.5 | 1000000 | 0.0 | 15.040 | 3.882 | **3.874×** | MAJORITY_ADVERSARIAL |
| 128 | 1.5 | 1000000 | 0.0 | 22.369 | 5.810 | **3.850×** | HE_OR_OTHER |
| 5 | 1.5 | 1000000 | 0.0 | 12.259 | 3.398 | **3.607×** | MAJORITY_ADVERSARIAL |
| 32 | 1.5 | 1000000 | 0.0 | 19.669 | 5.484 | **3.587×** | HE_OR_OTHER |
| 16 | 1.5 | 1000000 | 0.0 | 28.235 | 9.003 | **3.136×** | HE_OR_OTHER |

**Hunt maximum:** k=4, alpha=1.2, spacing=1000000, dom_boost=0.0 → **5.463×** via `HE_OR_OTHER`

## Routing notes

- COUNTING_ATTEMPT: sample gates admit counting residual
- MAJORITY_ADVERSARIAL: equal-rate probe ≥ 0.35 → adversarial residual
- HE_OR_OTHER: fell through to HE MSD / sparse / other

**Key insight:** Compact key spacing → counting wins; wide spacing → multi-fold floor.

Phase 0 lock untouched. No residual menu changes. Not field-level.

**THE BEASTIE BOYZ**
