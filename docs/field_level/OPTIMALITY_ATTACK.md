# Optimality Attack — Known / Hidden / New Asymptotics

**Status:** ATTACK SURFACE ONLY. **No optimality claimed.**  
**Date:** 2026-08-12 · THE BEASTIE BOYZ  
**Depends on:** T1_DECISION_PROCEDURE.md, PATH_B_LATERAL.md  
**Rule:** EXTERNAL-clean. Kill if no concrete bound in fixed window.

---

## One line

Classical measure-optimality is **blocked** for hybrid `f`.  
Viable path = **new sample-measure** M_S, or synergistic residual that *matches* a known lower bound under a stated model.

---

## 1. Known asymptotics (comparison model, distinct keys)

| Measure | Lower bound (info-theoretic) | Optimal algorithms exist? |
|---------|------------------------------|---------------------------|
| **Inv** | Ω(n + n log(Inv/n + 1)) | Yes (A-Sort, Splaysort, …) |
| **Runs** | Ω(n + n log Runs) / Ω(n(1+H(Runs))) | Yes (run-merge / Huffman-merge) |
| **LRM** | Ω(n(1+H(LRM))), H(LRM)≤H(Runs) | Yes (LRM-Sorting, Barbay–Fischer–Navarro) |
| **Rem** | Ω(n + n log Rem) | Yes |
| **α-sorted subsequences** | Ω(n · H(α fractions)) | Yes (partition-sort, Sen arXiv:2506.08261) |
| **Card / multiset** | Ω(n + n log u) / entropy of multiplicities | Yes |
| **Par** | Ω(n log Par) | Yes |

Petersson–Moffat partial order: Inv-optimality implies weaker measures; maximal measures exist that dominate all known ones.

**Hybrid `f` vs known optimality**

| Region of `f` | Residual | Matches known optimal? |
|---------------|----------|-------------------------|
| STRUCTURE | pure O(n) | Yes (trivial) |
| equal_heavy / û low | pure counting / few_wide | Near multiset-optimal **in word model** if exact counts |
| low Înv + consecutive dôm | pure low_disorder | **No proof** Inv-optimal |
| desc_runs / reverse-seg | pure | **No proof** Runs/LRM-optimal |
| HE (û high ∧ Înv high) | ska | Word/radix model — not comparison Inv-optimal |
| default | pdq | Not adaptive-optimal |

**Verdict (known):** Cannot claim Inv-, Runs-, LRM-, or α-optimality for hybrid residual as implemented.

---

## 2. Hidden blockers (why classical claim fails)

1. **Sample ≠ full measure**  
   Probes use S ∈ {256, 512, ~8k}.  
   Înv_S, Runŝ_S, û_S are estimators. Classical optimality is defined on exact Inv, Runs, LRM of the full input.  
   Misrouting (T3) is structural, not a bug.

2. **Residual quality gap**  
   Even perfect routing does not imply measure-optimality unless the residual itself is optimal for that measure.  
   Pure residual menu is engineering-competitive, not asymptotically certified Inv/Runs optimal.

3. **Model mismatch**  
   Classical lower bounds are comparison-based.  
   ska / counting / integer MSD live in word / radix models.  
   Cross-model “optimality” requires explicit model statement.

4. **Multi-measure tradeoff**  
   Optimality for one measure does not transfer.  
   Hybrid is a **practical multi-measure router**, not a single-measure optimal algorithm in the Mannila sense.

5. **Synergistic gap**  
   Barbay–Ochoa–Satti synergistic sorting (presortedness ∧ multisets) is the closest classical target.  
   Hybrid approximates the idea (û + Înv) but without a published synergistic bound.

---

## 3. New asymptotics — attack surface (not claimed)

### Candidate A — Sample measure M_S

Define

```
M_S(X) := (Înv_S, Runŝ_S, û_S, êq_S, dôm_S)
```

with fixed S ≪ n.

**Attack:**

1. Prove Ω lower bound for comparison (or word) sorting given only access to M_S and the array.  
2. Show hybrid cost T_hybrid ≤ c · LB(M_S) + o(n log n) under stated model.  
3. If (1)+(2) hold → **sample-optimal residual selection** (new, weaker than classical measure-optimality).

**Honesty:** M_S is coarser than Inv/Runs. Any claim must say *sample-optimal w.r.t. M_S*, not Inv-optimal.

### Candidate B — α / LRM residual upgrade

If pure residual is replaced by:

- partition-sort optimal for α-sorted subsequences, or  
- LRM-Sorting (O(n(1+H(LRM)))),

and routing detects the right regime from probes, then **regime-conditional** optimality becomes claimable for those inputs only.

**Honesty:** Requires residual code change + proof; not free from current menu.

### Candidate C — Synergistic word-model bound

Joint adaptivity to (presortedness, cardinality) under word RAM:

```
T = O(n + n · min(log û, H_presort, w))
```

or similar. Hybrid’s û + Înv routing is a **heuristic for** this shape. Formalizing it is new work.

---

## 4. What we refuse to claim today

- Hybrid residual is **not** Inv-optimal, Runs-optimal, LRM-optimal, or α-optimal.  
- Soft=0 / geo ≤ 0.90 is **empirical path-(a)** evidence, not asymptotic optimality.  
- T1 is a **decision procedure**, not a complexity theorem.  
- No hidden χ or internal irreversible state is used or claimed.

---

## 5. Immediate hard moves (ordered)

| # | Move | Outcome if success |
|---|------|--------------------|
| 1 | Formalize M_S + state sample lower-bound sketch | Opens Candidate A |
| 2 | Prove or kill: pure low_disorder is Inv-competitive to within constant | Shrinks residual gap |
| 3 | Implement LRM or α-partition residual behind existing desc_runs / consecutive-dom gates | Opens Candidate B |
| 4 | Write synergistic word-model conjecture matching hybrid table | Opens Candidate C |

Kill criteria: if none of A/B/C yields a written bound in a fixed window → park path (b) optimality track; retain T1 as engineering decision procedure only.

---

## References (anchors only)

- Mannila 1985 — measures of presortedness + optimality definition  
- Estivill-Castro & Wood — survey / Inv / Runs  
- Petersson & Moffat — partial order on measures  
- Barbay, Fischer, Navarro — LRM-Trees, H(LRM) adaptive sort  
- Sen arXiv:2506.08261 — α-sorted subsequences, partition-sort optimal  
- Barbay, Ochoa, Satti — synergistic sorting (presortedness ∧ multisets)

**THE BEASTIE BOYZ — no optimality claimed.**
