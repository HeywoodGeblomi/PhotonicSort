# T1 — Candidate Decision Procedure

**Status:** CANDIDATE ONLY. Not a theorem. Not a claim.  
**Date:** 2026-08-12 · THE BEASTIE BOYZ  
**Implements:** `residual/hybrid_residual_menu.hpp` v21  
**Frame:** `residual = f(Înv, Runŝ, û, êq, dôm)` with probe cost O(S), S ≪ n.

---

## One line

Sample visible metrics → choose residual class. Cost = probe O(S) + residual T(n).

---

## Inputs (probes)

| Symbol | Probe | S | Classical analog |
|--------|-------|---|------------------|
| Înv | sample inv rate | 512 | Inv / n |
| Runŝ | desc_runs / runs | 512 | Runs / LRM |
| û | sample unique count | 512 | cardinality / entropy proxy |
| êq | adjacent-equal rate | 256 | multiset mass |
| dôm | sample max−min | 512 | range / α-width |
| dinv | dense inv count | ~8k | local Inv |

All EXTERNAL-clean. No hidden state.

---

## f — the table (exact implementation order)

```
1. sorted_asc / sorted_desc     → STRUCTURE     (O(n) pure)
2. êq ≥ 3/4                      → PDQ           (equal_heavy)
3. û ≤ 32                        → PURE          (few-unique / counting)
4. û ≤ 128 ∧ Înv ∈ [1/2, 1/2]    → PURE          (sawtooth band)
5. desc_runs ≥ 3 ∧ Înv ≥ 3/5     → PURE          (reverse-segments / LRM)
6. Înv ≤ 1/20 ∧ dôm ≤ 2n         → PURE          (almost-sorted consecutive)
7. Înv ≤ 1/20 ∧ dôm > 2n         → PDQ           (sparse near-sorted)
8. û ≥ 3/4 ∧ Înv ≥ 2/5           → SKA           (HE / gaussianish)
9. dôm ≤ 2n ∧ dinv ≥ 100         → SKA (≤4B) / PDQ (8B)   (α-local / mixed_blocks)
10. else                         → PDQ           (default residual)
```

`PURE` = pure residual menu. `PDQ` = residual_pdqsort. `SKA` = ska_sort when present.

---

## Cost model (candidate)

```
T_hybrid(n) = O(S log S) + T_residual(n)
```

- Probe: fixed S ∈ {256, 512, ~8k} — independent of residual quality.
- Misrouting risk: `f(sample) ≠ f(full metrics)` — T3 territory.
- No claim that f is optimal. f is the **implemented** adaptive choice.

---

## What T1 is not

- Not a complexity theorem.
- Not a proof that hybrid dominates any baseline class.
- Not asymptotic novelty.
- Exact thresholds are engineering; the *form* is the candidate theory object.

---

## Next

| ID | Need |
|----|------|
| T2 | Map f’s partitions to Mannila / α / LRM classes in one page |
| T3 | Bound P[misroute] under sampling assumptions |

Kill: no concrete bound in fixed window → park path (b).

**EXTERNAL-clean. Candidate only.**
