# Few-Unique Floor — Hunt Summary

**Critical baseline:** `few_unique_heavy_k4` = **5.694×** (Family-2 fixed seed)

## What the hunt found (45 points, n=1e6, median-of-7)

| Finding | Detail |
|---------|--------|
| Peak in hunt | **5.463×** (k=4, α=1.2, wide spacing) via HE path |
| k=3…5, moderate Zipf | Consistently 2–5× floors |
| Large k (32–128) + Zipf | Still 3–5× via HE (not counting) |
| Compact spacing (keys near each other) | Counting path fires → pure **wins** (0.3–0.4×) |
| Wide spacing (1e6+) | Counting rejected (range gate) → majority or HE → multi-fold lag |

## Root cause (routing)

1. **Counting residual** only accepts compact sample range (`srange < 2^20` and not too wide vs n). Zipf keys spaced by 1e6 fail the range gate.
2. **Majority / adversarial residual** fires on high equal-rate samples (k small, high α) but is slower than pdqsort on these patterns (~​1.2–5×).
3. **HE MSD** is the fallback for flatter Zipf / larger k with wide keys — also lags badly when true cardinality is tiny.

**Diagnosis:** The residual menu has no fast path for *few distinct values over a wide numeric range*. Counting needs compact range; majority/HE pay full work that pdqsort avoids via equal-key handling.

Phase 0 lock untouched. No residual menu changes this phase. Not field-level.

**THE BEASTIE BOYZ**
