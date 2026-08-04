# PhotonicSort C 1.3.0-c — Plan A (win the war paths)

Closes the residual gaps where `std::sort` previously won.

## War-path results (n=1e6 vs libstdc++ std::sort)

| Pattern | v1.2.0 | **v1.3.0 Plan A** |
|---------|-------:|------------------:|
| sawtooth | 0.51× | **14.2×** |
| few_unique | 0.51× | **12.7×** |
| almost_sorted | 0.36× | **1.33×** |

## Plan A residuals

1. **LOW_CARD (route 3)** — counting sort when pilot unique_est ≤ 256 or small dense range. Crushes sawtooth (`i%64`) and few_unique.
2. **LOW_DISORDER (route 4)** — insertion for n≤4096; Hoare-partition pdq for large almost-sorted.
3. Pilot unique set (`ps_uset`, 128 slots) + sample min/max.
4. Zero-inversion pilot falls through to full structure probe (protects sorted/organpipe).
5. `PS_MAX_TRACKED_RUNS` 8 → 32.

Brand contract unchanged: **probe → structure early-exit → residual menu**.

## Verify

```bash
cd c && make test
```
