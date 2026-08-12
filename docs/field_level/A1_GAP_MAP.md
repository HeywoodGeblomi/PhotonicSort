# A1 Gap Map — Expanded Field Suite vs Best Specialized

**Date:** 2026-08-12  
**Status:** SKELETON — awaiting Expanded Field Suite harness numbers  
**Parent:** [`FIELD_LEVEL_ENTRY_ATTACK.md`](./FIELD_LEVEL_ENTRY_ATTACK.md) · [`EXPANDED_FIELD_SUITE.md`](./EXPANDED_FIELD_SUITE.md) · [`METRICS_LOCKED.md`](./METRICS_LOCKED.md) · [`BASELINES_LOCKED.md`](./BASELINES_LOCKED.md)  
**Harness:** `scripts/expanded_field_bench.cpp` (in progress)  
**Not field-level.**

---

## 1. Purpose

Measure pure residual vs **best specialized** (`min(pdqsort, ska_sort where applicable, std::sort)`) on the full Expanded Field Suite (P01–P23). Soft spots redefined as any **in-scope** pattern with ratio > **1.15×**. Primary published metric = geometric mean of ratios over in-scope patterns.

Claim-surface soft gate is already CLOSED (13-pattern subset). This document is the evidence foundation for path-(a) field-level entry.

---

## 2. Methodology (frozen)

| Item | Value |
|------|-------|
| n primary | 1e6 |
| n scale | 1e7 (A3) |
| reps | median of odd R ≥ 7 (A1 default R=9; smoke R=5 OK) |
| seed | 42 |
| best | min(pdq, ska if applicable, std) |
| soft | ratio_best > 1.15 |
| CSV schema | `arch,type,pattern,n,reps,menu_ms,pdq_ms,ska_ms,std_ms,best_ms,ratio_best,ok` |
| Bootstrap | B ≥ 1000 on pattern-ratio vector for primary geo (A3/A4) |

Compiler: `-O3 -std=c++17 -DNDEBUG`.

---

## 3. Pattern inventory

### 3.1 Core 19 (in-scope unless formally scoped)

| ID | Pattern | Group | Expected residual path |
|----|---------|-------|------------------------|
| P01 | sorted | Structure | STRUCTURE early-exit |
| P02 | reverse | Structure | STRUCTURE / reverse |
| P03 | almost_sorted | Structure | low_disorder |
| P04 | organpipe | Structure | residual / ska relevance |
| P05 | sawtooth | Structure | counting / runs |
| P06 | reverse_segments | Structure | runs / residual |
| P07 | push_middle | Runs | residual |
| P08 | runs_noise | Runs | residual |
| P09 | few_k4_dense | Low-card | counting cnt4 |
| P10 | few_k16_dense | Low-card | counting |
| P11 | few_k16_wide | Low-card | few_wide residual |
| P12 | equal_heavy | Low-card | equal / counting |
| P13 | zipf_k16 | Low-card | counting |
| P14 | db_pk | Real-shaped | low_disorder / residual |
| P15 | timestamps | Real-shaped | low_disorder |
| P16 | pipe_sparse | Adversarial | residual_pdq quality |
| P17 | adversarial | Adversarial | residual |
| P18 | random | HE | residual / ska |
| P19 | gaussianish | HE | residual quality limit |

### 3.2 Extended 4 (A0 locked names)

| ID | Pattern | Group | Contract (seed=42) |
|----|---------|-------|--------------------|
| P20 | db_fk_zipf | Real-shaped | Zipf over ~√n distinct refs |
| P21 | timestamp_drift | Real-shaped | Monotonic base + regime drift every n/10 |
| P22 | mixed_blocks | Mixed | Alternating sorted blocks size 256 + shuffle islands |
| P23 | uniform_u32 | HE | Full-range uint32 uniform |

### 3.3 Formal scope-outs (candidates — confirm after numbers)

These may be formally scoped if residual quality cannot match radix-specialized baselines without breaking EXTERNAL-clean pure residual claim:

| Pattern | Likely reason |
|---------|---------------|
| pipe_sparse (P16) | residual_pdqsort quality vs library pdq |
| random (P18) | ska wins HE uniform |
| gaussianish (P19) | continuous HE path tax |
| few_k16_wide (P11) | FEW_WIDE residual quality |
| organpipe (P04) | ska wins; residual cannot match radix |
| uniform_u32 (P23) | ska HE dominance |

**Rule:** Do not hide losses inside geo mean. Either close (EXTERNAL-clean residual) or formal scope-out with rationale.

---

## 4. Results tables (PENDING)

### 4.1 i64 @ n=1e6

| Pattern | menu_ms | best_ms | ratio_best | soft | notes |
|---------|--------:|--------:|-----------:|:----:|-------|
| *(awaiting harness)* | | | | | |

**i64 aggregate:** geo=— · max=— · soft_count=— · CI95=[—, —]

### 4.2 i32 @ n=1e6

| Pattern | menu_ms | best_ms | ratio_best | soft | notes |
|---------|--------:|--------:|-----------:|:----:|-------|
| *(awaiting harness)* | | | | | |

**i32 aggregate:** geo=— · max=— · soft_count=— · CI95=[—, —]

### 4.3 u32 @ n=1e6

| Pattern | menu_ms | best_ms | ratio_best | soft | notes |
|---------|--------:|--------:|-----------:|:----:|-------|
| *(awaiting harness)* | | | | | |

**u32 aggregate:** geo=— · max=— · soft_count=— · CI95=[—, —]

### 4.4 Cross-type summary

| Type | geo_best | max | soft_count | Gate geo≤0.90 / soft=0 |
|------|---------:|----:|-----------:|:----------------------:|
| i64 | — | — | — | PENDING |
| i32 | — | — | — | PENDING |
| u32 | — | — | — | PENDING |

---

## 5. Soft-spot decisions (A2 input)

| Pattern | Type(s) | ratio | Decision (close / scope) | Rationale |
|---------|---------|------:|--------------------------|-----------|
| *(fill after numbers)* | | | | |

---

## 6. Non-claims

- This skeleton is **not** a field-level claim.
- Claim-surface soft=0 does not transfer to Expanded Field Suite.
- Numbers below (when filled) are path-(a) evidence only after soft_count=0 in-scope and geo≤0.90 with CI.
- EXTERNAL-clean pure residual only. No χ.

**THE BEASTIE BOYZ**
