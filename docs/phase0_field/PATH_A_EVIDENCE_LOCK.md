# Path (a) Evidence Lock — Phase 0 Suite + Reproduction + Evaluation

**Date:** 2026-08-12  
**Team:** THE BEASTIE BOYZ  
**Stack:** counting → consecutive_perm → push_middle → low_disorder (residual_pdqsort) → MSD HE  
**Scope:** sequential POD int32 / uint32 · i64 protected · EXTERNAL-clean  

---

## 1. Full Phase 0 Suite (18 patterns)

Real-world + adversarial + classic families locked in `reproduce/harness_phase0.cpp`:

| Family | Patterns |
|--------|----------|
| Classic structured | sorted, reverse, organpipe, sawtooth |
| Almost-sorted | almost_sorted, push_middle, nearly_sorted_blocks |
| High-entropy | uniform, gauss_mix |
| Few-unique | two_values, few_k16_dense, few_k16_wide, zipf_k16 |
| Application-like | db_pk, timestamps |
| Adversarial | equal_heavy, adversarial_pipe, reverse_segments_8 |

## 2. n-matrix results (median of reps)

### n=1e6 · reps=5 · all ok=1

| Type | Geo vs std | Geo vs pdq | Geo vs ska |
|------|----------:|----------:|----------:|
| **int32** | **0.111×** | **0.462×** | **0.184×** |
| **uint32** | **0.116×** | **0.519×** | **0.198×** |

### n=1e5 · reps=7 · all ok=1

| Type | Geo vs std | Geo vs pdq | Geo vs ska |
|------|----------:|----------:|----------:|
| **int32** | **0.162×** | **0.612×** | **0.243×** |
| **uint32** | **0.150×** | **0.653×** | **0.235×** |

### Soft spots vs pdq at n=1e6 (full-suite harness)

| Pattern | i32 | u32 | Notes |
|---------|----:|----:|-------|
| db_pk | 1.73× | 1.83× | isolation micro ~0.97×; harness variance |
| timestamps | 1.56× | 1.60× | near-monotonic residual quality |
| equal_heavy | 1.49× | 1.61× | residual quality |
| few_k16_wide | 1.06× | 1.19× | borderline |
| few_k16_dense (u32) | 0.76× | 1.63× | u32 soft |

Strong wins: nearly_sorted_blocks 0.06–0.07×, adversarial_pipe 0.09×, organpipe 0.10×, almost_sorted 0.26–0.28×.

## 3. Independent reproduction

Harness: `reproduce/harness_phase0.cpp`  
Baselines: vendored pdqsort + ska_sort  
Residual stack: path-1 main (consecutive_perm + residual_pdqsort low_disorder)

```bash
cd reproduce
g++ -O3 -std=c++17 -DNDEBUG -I. -Iresidual -Ibaselines -o harness_phase0 harness_phase0.cpp
./harness_phase0 --type i32 --n 1000000 --reps 5 --out results
./harness_phase0 --type u32 --n 1000000 --reps 5 --out results
```

Compare **ratios**, not absolute milliseconds.

## 4. Evaluation against field-level criteria

| Criterion | Status |
|-----------|--------|
| Expanded reproducible suite (18 patterns, multi-type, n-matrix) | **MET** |
| Independent reproduction harness | **MET** |
| Geo < 1.0× vs pdq both types at n=1e6 | **MET** (0.46× / 0.52×) |
| Soft spots ≤1.15× across suite | **NOT MET** (db_pk / timestamps / equal_heavy) |
| Multi-architecture / statistical CI | **NOT MET** |
| Library integration (path c) | **NOT MET** |
| Novel complexity theorem (path b) | **NOT MET** |

### Field-level verdict

**NOT field-level.**

Geo-mean strength is real. Soft spots on application-like near-monotonic patterns remain under full-suite measurement. Path (a) requires those soft spots closed under the expanded suite before any field-level claim.

## 5. Non-claims

- Not field-level.
- Sequential POD int32/uint32 only; i64 path protected and unmeasured in this lock.
- EXTERNAL-clean pure residual preference held.
- Controlled synthetic suite only; no claim of general domination vs pdq/Timsort/std across all regimes or architectures.

**THE BEASTIE BOYZ**
