# A1 Gap Map — vs Best Specialized

**Date:** 2026-08-12  
**Status:** COMPLETE (measurement only)  
**Suite:** Expanded Field Suite core 19 (A0 frozen)  
**n:** 1e6 · **reps:** 5 (median) · **arch:** x86_64  
**Compiler:** g++ -O3 -std=c++17 -DNDEBUG  
**Not field-level.**

Primary metric: `ratio = pure_residual_ms / best_ms` where  
`best_ms = min(pdqsort, ska_sort, std::sort)` per pattern.

CSV: [`a1_gap_map.csv`](./a1_gap_map.csv)

---

## 1. Summary

| Type | geo vs best | max | soft (>1.15×) | ok fail |
|------|------------:|----:|--------------:|--------:|
| **i32** | **0.56×** | 2.72× | 4 | 0 |
| **u32** | **0.63×** | 2.86× | 5 | 0 |
| **i64** | **1.23×** | 57.1× | 11 | 0 |

Best-specialized source across 57 rows: **pdq 48 · ska 9 · std 0**.

i32/u32 already clear the primary geo target (≤0.90×) **on this suite** — but soft spots remain. i64 does **not**.

---

## 2. Soft spots vs best specialized

### Cross-type (appear on ≥2 types)

| Pattern | i32 | u32 | i64 | Who wins |
|---------|----:|----:|----:|----------|
| **few_k4_dense** | 2.72× | 2.86× | 2.24× | **pdq** |
| **pipe_sparse** | 1.77× | 1.90× | 2.49× | **pdq** |
| **equal_heavy** | 1.45× | 1.63× | 1.59× | **pdq** |
| **runs_noise** | 1.41× | 1.57× | 1.63× | **pdq** |

### i64-only (additional)

| Pattern | ratio | Who wins | Note |
|---------|------:|----------|------|
| **db_pk** | **57.1×** | pdq | Critical: sparse-inversion low_disorder path tax (inv≈0.2%, menu 377ms vs pdq 6.6ms) |
| few_k16_wide | 1.58× | pdq | residual quality |
| gaussianish | 1.54× | pdq | HE residual path |
| zipf_k16 | 1.46× | pdq | mid-card |
| random | 1.40× | **ska** | radix wins HE uniform |
| few_k16_dense | 1.27× | pdq | |
| organpipe | 1.17× | **ska** | borderline |

### u32-only extra

| Pattern | ratio | Who wins |
|---------|------:|----------|
| db_pk | 1.18× | pdq (borderline soft) |

---

## 3. Where pure residual wins (examples)

| Pattern | i32 | u32 | i64 |
|---------|----:|----:|----:|
| reverse_segments | 0.12× | 0.15× | 0.71× |
| sawtooth | 0.15× | 0.17× | 0.20× |
| almost_sorted | 0.25× | 0.29× | 0.47× |
| push_middle | 0.24× | 0.29× | 0.89× |
| organpipe | 0.30× | 0.32× | 1.17× |
| sorted / reverse / timestamps | ≪1× | ≪1× | ≪1× |

Structure / run-heavy remains the strength. HE + dense ultra-low-card + sparse-inversion are the gaps.

---

## 4. Diagnostic notes (for A2)

1. **i64 db_pk (57×)** — Generator is near-monotonic with ~0.2% scattered swaps. `residual_low_disorder` insertion-style path is pathological on sparse inversions (O(n·distance) shifts). **A2 priority #1.**
2. **few_k4_dense** — Counting residual should dominate; routing or counting quality loses to pdq. **A2 priority #2.**
3. **pipe_sparse** — Late residual_pdqsort not winning enough vs library pdq; residual_pdqsort quality gap. **A2 priority #3.**
4. **random i64** — ska is best specialized; pure residual cannot beat integer radix on uniform without becoming ska. Scope decision: accept HE loss vs ska **or** document out-of-scope for pure residual claim.
5. **equal_heavy / runs_noise** — pdq residual body quality / routing.

---

## 5. A1 exit checklist

| Item | Status |
|------|--------|
| Ratios vs pdq, ska, std, best specialized | **DONE** |
| i32 / u32 / i64 @ n=1e6 | **DONE** |
| Soft spots redefined vs best specialized | **DONE** |
| CSV published | **DONE** |
| Residual code changed | **No** |

---

## 6. Next: A2

Close or formally scope every soft spot in §2. Priority: i64 db_pk → few_k4 → pipe_sparse → equal_heavy / runs_noise → i64 HE vs ska scope decision.

**Still not field-level.**

**THE BEASTIE BOYZ**
