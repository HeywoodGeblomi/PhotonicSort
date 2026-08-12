# Full Asymptotic Optimality — Unconstrained Target

**Status:** TARGET DEFINITION + ATTACK REQUIREMENTS. **Not achieved.**  
**Date:** 2026-08-12 · THE BEASTIE BOYZ  
**Relation to Soft:** Soft certificates are **subordinate evidence only**. They do not define, trap, or substitute for full asymptotic optimality.  
**Rule:** EXTERNAL-clean. Honesty primary.

---

## One line

Full asymptotic optimality is free of Soft, free of suite certificates, free of “lifts under assumptions.”  
It is a **lower bound + matching upper bound** for a stated measure and model, for all n.

---

## 1. What full unconstrained optimality is

### Definition (Mannila-style, unconstrained)

Fix:
- **Model** ℳ (comparison, word-RAM, or mixed — stated explicitly)
- **Measure of disorder** M : inputs → ℝ≥0
- **Lower bound** LB(n, M(X)) that every algorithm in ℳ requires on input X

An algorithm A is **M-optimal in ℳ** iff for every n and every input X,

```
T_A(X) ≤ c · LB(n, M(X)) + o(LB)     (or ≤ c·LB for fixed c)
```

No suite. No Soft threshold. No “on family ℱ.” No sample-size certificate.  
**All n. All X. Measure + model only.**

Examples of achieved full optimality in the literature:

| Measure | Model | Optimal cost |
|---------|-------|--------------|
| Inv | comparison | Θ(n + n log(Inv/n + 1)) |
| Runs / H(Runs) | comparison | Θ(n(1 + H(Runs))) |
| LRM / H(LRM) | comparison | Θ(n(1 + H(LRM))) |
| α-sorted subsequences | comparison | Θ(n · H(α fractions)) |
| multiset cardinality u | comparison | Θ(n + n log u) |

That is the standard. Anything weaker is not full unconstrained optimality.

---

## 2. Soft is not the target

| Object | Role |
|--------|------|
| Soft_τ(n, Π) | Finite empirical stress test |
| ρ_∞(ℱ, τ) | Menu-competitive ratio on a family (constrained) |
| **M-optimality in ℳ** | **Full unconstrained asymptotic optimality** |

Soft may **motivate** or **corroborate** but never **constitutes** full optimality.  
Refusing to trap optimality inside Soft is correct. Soft remains path-(a) evidence only.

`SOFT_TO_ASYMPTOTIC.md` describes a **weaker** bridge. It is not the definition of the target.  
This document supersedes Soft as the path-(b) optimality *goal*. Soft is relegated.

---

## 3. Why hybrid residual does not yet have full optimality

Structural blockers (not bureaucratic ones):

| Blocker | Why it kills unconstrained M-optimality today |
|---------|-----------------------------------------------|
| **Sample ≠ measure** | f reads M_S, S ≪ n. Classical LB is on exact M. Algorithm that never sees exact M cannot match LB on all X unless M_S determines M. |
| **Residual not M-optimal** | Even perfect routing fails if PURE / PDQ / SKA are not themselves M-optimal algorithms. |
| **Finite menu** | ℛ is finite. Full optimality often needs a continuum of behaviors (e.g. true Inv-adaptive insertion depth). |
| **Model split** | SKA is word/radix; Inv-LB is comparison. Cross-model “optimality” requires a single stated ℳ. |
| **Multi-measure router** | f adapts to several probes at once. Full optimality is usually proved for one M (or a proven maximal M in the Petersson–Moffat order). |

These are mathematical. Removing Soft language does not remove them.

---

## 4. What frees full optimality (requirements)

To claim unconstrained M-optimality for an algorithm built on this surface, **all** of the following must hold:

### R1 — Exact or sufficient measure access
Either:
- compute exact M in o(LB) time, or
- prove M_S determines the optimal residual class for all X (stronger than concentration in probability — worst-case determination), or
- define a **new measure M_S itself** and prove LB for M_S (then optimality is M_S-optimality, still unconstrained on that measure).

### R2 — Optimal residual for each regime
For every region of the measure space, the residual that runs must satisfy T_r(X) ≤ c · LB(n, M(X)).  
Engineering-competitive residuals are not enough. Need proven bounds:
- Inv-region → Inv-optimal residual (or equivalent)
- Runs/LRM-region → H(Runs)- or H(LRM)-optimal residual
- Card-region → multiset-optimal residual
- HE word-RAM → matching word-RAM lower bound

### R3 — Single model ℳ
State comparison **or** word-RAM **or** a clean mixed model with a published lower-bound theory.  
No silent switching between Inv-LB and radix cost.

### R4 — Matching upper bound for the whole algorithm
T_A(X) ≤ c · LB(n, M(X)) + lower-order terms, **for every X**, including adversarial sample-misrouting inputs.

### R5 — Written proof
Definitions, LB citation or proof, UB proof, gap-free. Not Soft tables.

---

## 5. Attack order aimed at full optimality (not Soft)

| Step | Action | Frees |
|------|--------|-------|
| **F1** | Choose **one** primary measure M (candidate: adjacent-Inv + û synergistic, or H(LRM), or α, or new M_S elevated to the measure) | Target clarity |
| **F2** | Fix model ℳ | R3 |
| **F3** | Either upgrade probes to exact M in o(LB), or prove M_S-optimality as the claim (new measure) | R1 |
| **F4** | Replace or prove residuals: Inv-optimal / LRM-optimal / multiset-optimal components behind the router | R2 |
| **F5** | Prove T_hybrid ≤ c · LB on all X (including worst-case misroute) | R4 |
| **F6** | Write the theorem | R5 |

Soft and Expanded Suite stay as **regression tests**, not as the optimality argument.

---

## 6. Recommended primary target (for unconstrained claim)

**Option α — New measure M_S (cleanest path to a true theorem)**

Elevate the sample measure to the object of optimality:

```
Claim shape: hybrid is M_S-optimal in word-RAM (or comparison):
T ≤ c · LB(n, M_S(X)) for all X.
```

Then full unconstrained optimality **of that measure** is free of Soft, free of suite, free of “family ℱ.”  
LB must be proved for M_S; UB must match. Hard, but well-defined.

**Option β — Classical measure with exact access**

Compute exact Runs or exact LRM partition in linear time (known), route to a proven H(LRM)-optimal merge residual.  
Claim LRM-optimality unconstrained.  
Requires residual rewrite (F4), not Soft rhetoric.

**Option γ — Synergistic classical**

Exact Inv (or adjacent-Inv) + exact u, synergistic LB (Barbay–Ochoa–Satti style), matching residual.  
Unconstrained synergistic optimality.  
Same demand: exact measures + optimal residual + proof.

---

## 7. Explicit non-claims

- Hybrid residual is **not** fully asymptotically optimal under any classical M today.  
- Soft=0 is **not** full asymptotic optimality.  
- Menu-competitive ρ_∞ on ℱ is **not** full unconstrained optimality.  
- This document **does not** claim F1–F6 are done.  
- EXTERNAL-clean. No χ.

---

## 8. Relegation of Soft bridge

`SOFT_TO_ASYMPTOTIC.md` remains valid as a **weaker** engineering-to-theory bridge for menu-competitive ratios on named families.  
It is **not** the path-(b) definition of success for optimality.  
Path-(b) success for optimality = **M-optimality in ℳ** as in §1.

**THE BEASTIE BOYZ — full optimality is unconstrained or it is not full.**
