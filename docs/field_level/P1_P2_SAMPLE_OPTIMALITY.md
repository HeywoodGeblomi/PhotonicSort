# P1 + P2 Unit — Sample-Measure Optimality & Misrouting Regret

**Status:** CANDIDATE STATEMENTS + PROOF SKETCHES. Not theorems until gaps closed.  
**Date:** 2026-08-12 · THE BEASTIE BOYZ  
**Implements probes of:** `residual/hybrid_residual_menu.hpp` v21  
**Rule:** EXTERNAL-clean. No classical Inv-optimality claimed.

---

## One line

Hybrid residual is a decision procedure over a **sample measure** M_S.  
P1 asks whether that decision is information-theoretically tight.  
P2 asks when the sample decision matches the full-metric oracle.

---

## 0. Model

**Input:** array X[0..n−1] over a totally ordered universe (integers in practice).  
**Residual menu** (finite):

```
ℛ = { STRUCTURE, PURE, PDQ, SKA }
```

Each r ∈ ℛ has cost T_r(X) ≥ 0 (comparisons or word-RAM time — model fixed per claim).

**Oracle residual:** r*(X) := argmin_{r∈ℛ} T_r(X) (ties broken by fixed priority).

**Probe budget:** S ≪ n fixed (hybrid uses S ∈ {256, 512, ~8192}).

---

## 1. Sample measure M_S

### Definition 1 (M_S)

Let I_S = { i_c = 1 + ⌊c(n−1)/S⌋ : c = 0..S−1 } be the uniform grid of size S.

```
Înv_S     := |{ c : X[i_c] < X[i_c − 1] }| / S          ∈ [0,1]
êq_S      := |{ c : X[i_c] = X[j_c] }| / S                ∈ [0,1]   (j_c = min(i_c+1,n−1))
û_S       := |unique values in { X[i_c] }| / S            ∈ (0,1]
Runŝ_S   := (number of desc-run starts on grid) / S      ∈ [0,1]
dôm_S     := (max_grid − min_grid)                         ≥ 0
dinv_S    := |{ k : X[k·h] > X[k·h+1] }|  with h=⌊n/8192⌋  ≥ 0
```

```
M_S(X) := (Înv_S, êq_S, û_S, Runŝ_S, dôm_S, dinv_S)
```

All components are **visible**, EXTERNAL-clean, O(S log S) computable (û_S sorts the sample).

### Definition 2 (hybrid router f)

Exact match to `hybrid_residual_menu.hpp` v21 dispatch order:

```
f(M_S) =
  STRUCTURE   if sorted_asc or sorted_desc          (full scan, not sample)
  PDQ         if êq_S ≥ 3/4
  PURE        if û_S ≤ 32/S
  PURE        if û_S ≤ 128/S ∧ Înv_S ∈ [1/2 − ε, 1/2 + ε]
  PURE        if Runŝ_S ≥ 3/S ∧ Înv_S ≥ 3/5
  PURE        if Înv_S ≤ 1/20 ∧ dôm_S ≤ 2n
  PDQ         if Înv_S ≤ 1/20 ∧ dôm_S > 2n
  SKA         if û_S ≥ 3/4 ∧ Înv_S ≥ 2/5
  SKA|PDQ     if dôm_S ≤ 2n ∧ dinv_S ≥ 100
  PDQ         otherwise
```

Cost of hybrid: T_f(X) := O(S log S) + T_{f(M_S(X))}(X).

---

## 2. P1 — Sample-measure residual selection

### Definition 3 (sample-consistent class)

For a realized sample measure m ∈ range(M_S),

```
𝒞(m) := { Y ∈ Universe^n : M_S(Y) = m }
```

Any algorithm that sees only m (or only the S sample cells) cannot distinguish members of 𝒞(m).

### Candidate Theorem A (information lower bound — residual choice)

**Statement (candidate).**  
Let 𝒜 be any algorithm that, given X, first observes only M_S(X)=m (or the S sample cells) and then selects a residual r ∈ ℛ and runs it.  
Then for every m there exists Y ∈ 𝒞(m) such that

```
T_𝒜(Y) ≥ min_{r∈ℛ} max_{Z∈𝒞(m)} T_r(Z) − o(T*)
```

in the comparison model (or word-RAM under standard transdichotomous assumptions).

**Interpretation:** the best guaranteed cost after seeing only m is the **minimax residual cost over the sample-consistent class**. No router beats that without more probes.

**Proof sketch.**
1. After observing m, 𝒜’s residual choice r_𝒜(m) is a function of m alone (or of the S cells).  
2. Adversary picks Y ∈ 𝒞(m) maximizing T_{r_𝒜(m)}(Y).  
3. Optimal guaranteed choice is argmin_r max_{Z∈𝒞(m)} T_r(Z).  
4. Gap o(T*) absorbs probe cost O(S log S) when S = o(n / log n).

**Open gap A1.** Explicit computation of max_{Z∈𝒞(m)} T_r(Z) for each r requires residual cost models (e.g. T_PDQ ≈ c n log n, T_STRUCTURE = Θ(n) only if fully sorted — but fully sorted is already filtered). Need Lipschitz or worst-case residual bounds inside 𝒞(m).

### Candidate Theorem B (sample-optimal router — upper bound shape)

**Statement (candidate).**  
If residual costs are **stable** on sample-consistent classes — i.e. there exists L such that for all r, all Y,Z ∈ 𝒞(m),

```
|T_r(Y) − T_r(Z)| ≤ L · φ(m,n)
```

with φ = o(T*/L) — then the minimax router

```
f*(m) := argmin_r  max_{Z∈𝒞(m)} T_r(Z)
```

satisfies T_{f*}(X) ≤ min_r T_r(X) + O(φ) + O(S log S) for all X with M_S(X)=m.

**Open gap B1.** Prove stability (or identify the subclass of inputs where it holds: e.g. random-like HE, low-Inv consecutive). Hybrid f is a **heuristic for f***, not proven equal to f*.

### What P1 does **not** claim

- Hybrid f = f*.  
- Classical Inv-optimality.  
- That O(S log S) + T_{f(M_S)} matches Ω(n + n log(Inv/n+1)).

---

## 3. P2 — Misrouting regret

### Definition 4 (oracle and regret)

```
r†(X) := argmin_r T_r(X)          // full-metric oracle over ℛ
R(X)  := T_{f(M_S(X))}(X) − T_{r†(X)}(X) ≥ 0
```

Regret is zero when sample router matches the residual the oracle would pick.

### Definition 5 (full metric vector)

```
M(X) := (Inv(X)/binom(n,2), EqRate, u/n, Runs(X)/n, Dom(X), …)
```

(Exact analogues of M_S components.)

### Lemma 1 (grid concentration — candidate)

**Statement (candidate).**  
Let X be drawn from a model where adjacent-pair inversion indicators are weakly dependent (e.g. random permutation, or m-dependent disorder). Then for the grid of size S,

```
P( |Înv_S − Inv_adj| > ε ) ≤ 2 exp(−2 S ε²)
```

where Inv_adj = |{i : X[i]<X[i−1]}|/n (adjacent inversion rate).

**Proof sketch.** Hoeffding for sampling without replacement / serially dependent indicators under m-dependence; standard for systematic samples of stationary sequences.

**Open gap L1.** Extend from adjacent-Inv to full Inv/binom(n,2) (global inversions need different estimators). Hybrid uses adjacent-style inv on grid — consistent with Inv_adj, not full Inv. State measure as **adjacent-Inv** honestly.

### Lemma 2 (û_S concentration — candidate)

**Statement (candidate).**  
For multisets with u distinct values and frequency vector π, the sample unique count û_S · S concentrates around the expected number of distinct in S draws without replacement. Relative error O(√((u log u)/S)) with high probability when S ≥ u log u.

**Open gap L2.** Tight constants; collision-based estimators may dominate sorting the sample for large u.

### Candidate Theorem C (regret vanishing)

**Statement (candidate).**  
Assume:
1. Residual costs are Lipschitz in M: |T_r(X)−T_r(Y)| ≤ L ‖M(X)−M(Y)‖_1 · n log n.  
2. f is a **Voronoi router**: it partitions range(M_S) into cells labeled by r ∈ ℛ, and agrees with r† whenever M(X) lies at least δ outside cell boundaries.  
3. M_S concentrates within δ/2 of M with probability ≥ 1−η (Lemmas 1–2).

Then

```
P( R(X) > 0 ) ≤ η + P(M(X) in δ-boundary of f-cells)
```

and E[R(X)] ≤ η · O(n log n) + boundary-mass · O(n log n).

**Interpretation:** regret is controlled by (a) sample error and (b) inputs near decision boundaries (equal_heavy threshold, HE threshold, etc.).

**Open gap C1.** Prove Lipschitz of T_PDQ, T_SKA, T_PURE in adjacent-Inv / û. Empirical evidence exists; analytic proof open.  
**Open gap C2.** Measure boundary mass under realistic input ensembles (Expanded Suite is finite; need a continuum model).

### Corollary (engineering)

If Expanded Suite shows soft=0 @1.20, that is **empirical evidence** that boundary mass + sample error are small on that distribution family — not a proof of Theorem C.

---

## 4. Joint P1+P2 unit — what “slayed” means here

| Deliverable | Status |
|-------------|--------|
| M_S formal definition matching hybrid | **DONE** |
| f as explicit router on M_S | **DONE** |
| Minimax lower bound shape (Thm A) | **CANDIDATE** — gap A1 |
| Stability → sample-optimal upper (Thm B) | **CANDIDATE** — gap B1 |
| Concentration lemmas (L1, L2) | **CANDIDATE** — standard tools, gaps L1–L2 |
| Regret vanishing (Thm C) | **CANDIDATE** — gaps C1–C2 |
| Classical Inv-optimality | **NOT CLAIMED** |
| Hybrid f = f* | **NOT CLAIMED** |

**Slayed = formalized attack surface with labeled gaps.**  
**Not slayed = closed theorems.** Closing A1/B1/C1 is the next theory labor.

---

## 5. Immediate next labor (ordered)

1. **A1:** Bound max_{Z∈𝒞(m)} T_r(Z) for r ∈ {PDQ, SKA, PURE} under a concrete cost model (e.g. T_PDQ ≤ c n log n, T_SKA ≤ c' n · (1 + keysize/w), T_PURE ≤ c'' n log û when û small).  
2. **C1:** Lipschitz or monotonicity of residual costs in Înv_S, û_S — even one-sided inequalities suffice for boundary analysis.  
3. **L1 full:** state hybrid’s inv probe as **adjacent-inversion rate** in all public theory text (honesty).  
4. Optional: compute empirical boundary mass on Expanded Suite (fraction of inputs within ε of each f threshold).

---

## 6. Non-claims (mandatory)

- No theorem in this document is marked proven.  
- Hybrid residual is **not** sample-optimal until Thm A+B gaps close and f is shown to match f* on the relevant class.  
- Soft=0 path-(a) results are **empirical**, not asymptotic optimality.  
- EXTERNAL-clean held. No χ.

**THE BEASTIE BOYZ — formalize, then close gaps.**
