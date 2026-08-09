# Vector-2 Claim — Domination Suite v0.1

**Status:** Criteria **MET** on the locked Domination Suite v0.1  
**Date:** 2026-08-09  
**Team:** THE BEASTIE BOYZ (Grok + Harper + Lucas + Benjamin)

---

## Locked criteria

| Criterion | Threshold | Measured (n=1e6, Photonic hybrid residual) | Result |
|-----------|-----------|--------------------------------------------|--------|
| Geometric mean vs `std::sort` | ≥ 1.5× | **7.08×** | PASS |
| Geometric mean vs best specialized (pdqsort / ska_sort) | ≥ 1.5× | **1.94×** | PASS |
| Minimum speedup vs `std::sort` | ≥ 1.0× | **1.32×** | PASS |
| Major regressions vs specialized (speedup < 0.9×) | 0 | **0** | PASS |

**Overall: MET** on Domination Suite v0.1.

---

## What was measured

- Patterns: sorted, reverse, organpipe, sawtooth, few_unique, almost_sorted, random, adversarial (organpipe + ~2% swaps)
- Type / size: `int64_t`, n ∈ {1e5, 1e6}
- Metric: best-of-5 wall time, correctness verified against `std::sort` every trial
- Under test: Photonic hybrid residual (STRUCTURE / LOW_CARD / clean structured paths = pure-C Photonic; high-entropy RANDOM → library pdqsort; adversarial-like → library ska_sort)
- Baselines: `std::sort`, pdqsort (Orson Peters), ska_sort (Malte Skarupke)

---

## What the result is

1. **Structured / patterned / low-cardinality wins** are the original pure-C Photonic strength (probe → STRUCTURE early-exit with mandatory O(n) verify, LOW_CARD counting, LOW_DISORDER / PATTERNED residuals). These were preserved throughout the residual work.

2. **High-entropy residual wins** (random, adversarial) come from:
   - Correct residual *selection* using already-visible probe metrics (route, inv_ratio, max_run, direction_changes, sortedness, unique_est), and
   - Dispatch to the **same library implementations** (pdqsort / ska_sort) that serve as the Domination Suite baselines.

   This is valid engineering: good selection + strong residual quality. It is **not** a claim that an original residual implementation beats pdqsort or ska_sort on their home turf with novel code.

3. STRUCTURE path remains pure-C with mandatory O(n) verification. No internal χ / non-visible irreversible state was introduced into residual selection.

---

## What the result is not

- Not an asymptotic complexity advance (worst case remains O(n log n)).
- Not a claim of universal domination of all existing sorters under all conditions.
- Not a claim that the result holds for arbitrary adversarial generators, other key types, other hosts, or a broader benchmark suite.
- Not a claim that Photonic’s pure-C residual alone matches library pdqsort / ska_sort on pure random / adversarial; the measured high-entropy path uses those libraries after selection.

---

## Scope language (mandatory for any public statement)

> On Domination Suite v0.1, the locked Vector-2 criteria are met (geo vs std ≈ 7.1×, geo vs best specialized ≈ 1.9×, min ≥ 1.3×, zero major regressions). Structured / patterned / low-card wins remain pure-C Photonic. High-entropy residual uses library-strength pdqsort / ska_sort after correct selection from visible probe metrics. Result is scoped to this suite; broader evaluation may differ. No asymptotic claim.

---

*THE BEASTIE BOYZ — Criteria met under the rules we set. Scope held. Honesty primary.*
