# Wave 2 Multi-Type Status Lock

**Date:** 2026-08-12  
**Stack:** residual polish on main — counting → push_middle → low_disorder → MSD HE  
**Harness:** reproduce/harness_multitype.cpp · n=1e6 · reps=5 (median) · all ok=1  
**Scope:** sequential POD int32 / uint32 only · i64 path protected · EXTERNAL-clean  

---

## Official Gap Maps (Photonic / pdqsort)

### int32 — geo **0.685×** vs pdq (was 0.846×)

| Pattern | vs pdq | Notes |
|---------|-------:|-------|
| sorted | 1.65× | soft (structure vs pdq early-exit) |
| reverse | 0.52× | |
| organpipe | 0.11× | |
| sawtooth | 0.31× | |
| almost_sorted | 2.30× | soft |
| uniform | 0.61× | |
| two_values | 0.39× | few_unique GREEN |
| few_k16_wide | 0.86× | few_unique GREEN |
| few_k16_dense | 0.78× | few_unique GREEN |
| zipf_k16 | 0.33× | few_unique GREEN |
| push_middle | 1.90× | soft (improved from ~2–5×) |
| db_pk | 1.61× | soft |

**Geo-mean Photonic/std = 0.146× · Photonic/pdq = 0.685× · Photonic/ska = 0.242×**

### uint32 — geo **0.661×** vs pdq (was 1.04×)

| Pattern | vs pdq | Notes |
|---------|-------:|-------|
| sorted | 0.78× | |
| reverse | 0.48× | |
| organpipe | 0.12× | |
| sawtooth | 0.33× | |
| almost_sorted | 2.28× | soft |
| uniform | 0.62× | |
| two_values | 0.40× | few_unique GREEN |
| few_k16_wide | 0.84× | few_unique GREEN |
| few_k16_dense | 0.62× | few_unique GREEN |
| zipf_k16 | 0.36× | few_unique GREEN |
| push_middle | 2.40× | soft |
| db_pk | 1.87× | soft |

**Geo-mean Photonic/std = 0.159× · Photonic/pdq = 0.661× · Photonic/ska = 0.232×**

---

## Barrier 4 Position

| Criterion | Status |
|-----------|--------|
| Multi-type surface (i32 + u32) | **MET** |
| few_unique GREEN both POD types | **MET** |
| Geo < 1.0× vs pdq both types | **MET** (i32 0.685×, u32 0.661×) |
| Soft spots closed (≤1.15×) | **NOT MET** — almost_sorted, push_middle, db_pk remain soft |
| i64 excellence protected | **HELD** |

**Barrier 4 (multi-type surface):** competitive geo achieved on both POD types. Soft-spot residual quality still open.

---

## Non-Claims

- Not field-level.
- Sequential POD int32/uint32 only.
- EXTERNAL-clean pure residual preference held.
- i64 path completely untouched by Wave 2 residual menus.
- Controlled suite only; no claim of general domination vs pdq/Timsort/std across all regimes.

**THE BEASTIE BOYZ**
