# Wave 2 Multi-Type Status Lock (post soft-spot kill)

**Date:** 2026-08-12  
**Stack:** counting → consecutive_perm → push_middle → low_disorder → MSD HE  
**Harness:** reproduce/harness_multitype.cpp · n=1e6 · reps=5 (median) · all ok=1  
**Scope:** sequential POD int32 / uint32 only · i64 path protected · EXTERNAL-clean  

---

## Official Gap Maps (Photonic / pdqsort)

### int32 — geo **0.440×** vs pdq (was 0.685× → 0.846×)

| Pattern | vs pdq | Notes |
|---------|-------:|-------|
| sorted | 0.83× | |
| reverse | 0.56× | |
| organpipe | 0.12× | |
| sawtooth | 0.30× | |
| **almost_sorted** | **0.20×** | consecutive_perm kill |
| uniform | 0.70× | |
| two_values | 0.18× | few_unique GREEN |
| few_k16_wide | 0.89× | few_unique GREEN |
| few_k16_dense | 0.82× | few_unique GREEN |
| zipf_k16 | 0.31× | few_unique GREEN |
| **push_middle** | **0.36×** | consecutive_perm kill |
| db_pk | 1.59× | soft (remaining) |

**Geo-mean Photonic/std = 0.106× · Photonic/pdq = 0.440× · Photonic/ska = 0.165×**

### uint32 — geo **0.500×** vs pdq (was 0.661× → 1.04×)

| Pattern | vs pdq | Notes |
|---------|-------:|-------|
| sorted | 1.05× | borderline |
| reverse | 0.43× | |
| organpipe | 0.12× | |
| sawtooth | 0.38× | |
| **almost_sorted** | **0.29×** | consecutive_perm kill |
| uniform | 0.60× | |
| two_values | 0.27× | few_unique GREEN |
| few_k16_wide | 0.94× | few_unique GREEN |
| few_k16_dense | 0.69× | few_unique GREEN |
| zipf_k16 | 0.37× | few_unique GREEN |
| **push_middle** | **0.49×** | consecutive_perm kill |
| db_pk | 2.16× | soft (remaining) |

**Geo-mean Photonic/std = 0.108× · Photonic/pdq = 0.500× · Photonic/ska = 0.161×**

---

## Soft-spot kill summary

| Pattern | Before | After |
|---------|-------:|------:|
| almost_sorted | ~2.3× | **0.20–0.29×** |
| push_middle | ~1.9–2.4× | **0.36–0.49×** |
| db_pk | ~1.6–1.9× | 1.59–2.16× (still soft) |

## Barrier 4 Position

| Criterion | Status |
|-----------|--------|
| Multi-type surface (i32 + u32) | **MET** |
| few_unique GREEN both POD types | **MET** |
| Geo < 1.0× vs pdq both types | **MET** (0.440× / 0.500×) |
| Soft spots closed (≤1.15×) | **NEAR** — only db_pk remains soft |
| i64 excellence protected | **HELD** |

---

## Non-Claims

- Not field-level.
- Sequential POD int32/uint32 only.
- EXTERNAL-clean pure residual preference held.
- i64 path completely untouched by Wave 2 residual menus.
- Controlled suite only; no claim of general domination vs pdq/Timsort/std across all regimes.

**THE BEASTIE BOYZ**
