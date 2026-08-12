# Metrics Locked — A0

**Date:** 2026-08-12  
**Status:** FROZEN  
**Parent:** [`FIELD_LEVEL_ENTRY_ATTACK.md`](./FIELD_LEVEL_ENTRY_ATTACK.md)

---

## 1. Per-pattern timing protocol

1. Generate input once per pattern with frozen seed.
2. **Warm-up:** 1 untimed run (discard).
3. **Timed runs:** odd count **R ≥ 7** (A1 default R=9 for primary tables).
4. Each timed run: copy input → sort → record wall time (steady_clock) → verify `ok=1`.
5. **Pattern time** = **median** of the R timed runs.
6. Report ms and ratio vs each applicable baseline and vs **best specialized**.

Cold isolation: for soft-spot decisions, re-run the single pattern alone (no suite cache pollution).

---

## 2. Aggregates

| Aggregate | Definition |
|-----------|------------|
| **Geo mean** | exp(mean(log(ratio_i))) over in-scope patterns |
| **Max ratio** | max ratio vs best specialized |
| **Soft count** | # patterns with ratio > 1.15 |
| **Group geo** | geo mean within Structure / Low-card / Runs / HE / Real-shaped / Adversarial |

Primary published number: **geo mean vs best specialized** at n=1e6, then n=1e7.

---

## 3. Statistical package (required for A3/A4 claim)

For the primary geo mean:

1. Collect the R medians per pattern (or bootstrap the R raw runs).
2. **Bootstrap** B ≥ 1000 resamples of the pattern-ratio vector → geo mean each time.
3. Report **2.5% / 97.5%** percentile interval (95% CI).
4. Field-level entry requires CI **excluding 1.0** (or clear one-sided superiority at 95%).

Pairwise: for each pattern, optional Wilcoxon / permutation test vs best specialized; not required if bootstrap geo CI is clean.

---

## 4. Kill criterion (from Entry Attack Plan)

After Phase A3:

- If primary geo mean vs best specialized **> 0.95×** **and** 95% CI overlaps 1.0 → **freeze specialized claim**; do **not** use field-level language.
- If any in-scope pattern remains > 1.15× without formal scope-out → A4 blocked.

Success target: geo ≤ **0.90×** with CI excluding 1.0; soft count = 0 in-scope.

---

## 5. Output schema (harness)

CSV (one row per pattern/type/n/arch):

```
arch,type,pattern,n,reps,menu_ms,pdq_ms,ska_ms,std_ms,best_ms,ratio_best,ok
```

Summary JSON/MD: geo_best, max_best, soft_count, ci_low, ci_high.

---

## 6. Freeze statement

Timing protocol, aggregates, bootstrap requirement, and kill criterion are frozen for path-(a) measurement.

**THE BEASTIE BOYZ**
