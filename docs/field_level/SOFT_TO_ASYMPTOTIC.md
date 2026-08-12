# Soft=0 ⇒ Asymptotic Optimality — Refit Bridge

**Status:** BRIDGE FORMALISM. Candidate only. Soft=0 alone does **not** imply asymptotic optimality.  
**Date:** 2026-08-12 · THE BEASTIE BOYZ  
**Depends on:** FIELD_LEVEL_CLAIM_v0.2, P1_P2_SAMPLE_OPTIMALITY.md  
**Rule:** EXTERNAL-clean. Honesty primary.

---

## One line

Soft=0 is a **finite-n certificate**.  
It becomes asymptotic competitive-ratio control **only** under concentration + residual stability + menu completeness on a stated instance family.

---

## 0. What Soft=0 is (and is not)

### Definition 1 (finite Soft)

Fix suite Π = {π_1,…,π_k} of pattern generators, size n, threshold τ ≥ 1:

```
Soft_τ(n, Π) := max_{π∈Π}  T_hybrid(π(n)) / T_best(π(n))
```

where T_best = min(T_pdq, T_ska, T_std, …) on that instance.

**Measured (path-a claim v0.2):** Soft_{1.20}(10^6, ExpandedSuite) = 1 on x86_64 (soft_count=0).

### What it is not

- Not ∀n Soft_τ(n)=1.  
- Not ∀ inputs Soft_τ=1.  
- Not classical Inv/Runs/LRM optimality.  
- Not a complexity theorem.

---

## 1. Asymptotic target (refit object)

### Definition 2 (asymptotic soft-optimality on a family)

Let ℱ be an instance family (distribution over inputs, or set of generators closed under n→∞).

```
ρ_∞(ℱ, τ) := limsup_{n→∞}  ess-sup_{X∼ℱ_n}  T_hybrid(X) / T*(X)
```

where T*(X) = min_{r∈ℛ} T_r(X) (oracle residual on the menu).

**Asymptotic soft-optimality at level τ:** ρ_∞(ℱ, τ) ≤ τ.

**Competitive form:** ρ_∞(ℱ, 1+ε) ≤ 1+ε for every ε>0 (after N_ε).

This is **menu-competitive asymptotic optimality** — weaker than classical measure-optimality, stronger than a single Soft=0 point.

---

## 2. Bridge theorem (candidate)

### Assumptions

**(A1) Concentration.**  
For X ∼ ℱ_n, M_S(X) concentrates on M(X):

```
P( ‖M_S(X) − M(X)‖ > δ ) ≤ η(n,S,δ) → 0 as n→∞ for fixed S≥S_0(δ), or S=S(n)→∞ slowly.
```

(See P1_P2 Lemmas L1–L2; adjacent-Inv form.)

**(A2) Residual Lipschitz / stability.**  
For each r ∈ ℛ, on the support of ℱ,

```
|T_r(X) − T_r(Y)| ≤ L · ‖M(X)−M(Y)‖_1 · Ψ(n)
```

with Ψ(n) = O(n log n) (or the relevant residual scale). Equivalently: T_r is nearly constant on sample-consistent classes 𝒞(m) (P1 gap B1).

**(A3) Menu completeness on ℱ.**  
For X ∼ ℱ, the oracle residual r†(X) ∈ ℛ achieves

```
T_{r†}(X) ≤ (1+γ) · T_opt_unrestricted(X)
```

with γ small — i.e. the menu is rich enough that the best menu residual is near the unrestricted optimum **on this family**. (Not claimed for all inputs.)

**(A4) Router agreement outside boundaries.**  
f is a Voronoi router on M_S that agrees with r† whenever M(X) is ≥δ from decision boundaries (P2 Thm C).

**(A5) Soft certificate at scale.**  
There exists n_0 and suite Π that **ε-covers** ℱ at scale n_0 (every high-mass region of ℱ has a representative in Π), with Soft_τ(n_0, Π) ≤ τ.

### Candidate Theorem D (Soft ⇒ asymptotic competitive ratio)

**Statement (candidate).**  
Under (A1)–(A5),

```
ρ_∞(ℱ, τ + ε_δ + ε_η + γ) ≤ τ + ε_δ + ε_η + γ
```

where ε_δ → 0 as boundary mass → 0, ε_η → 0 as concentration improves, γ from menu completeness.

**In particular:** if Soft_{1.20}(n_0, Π)=1, boundary mass is negligible, concentration holds, and menu is complete for ℱ with γ≤0.05, then

```
ρ_∞(ℱ, 1.25) ≤ 1.25
```

(menu-competitive asymptotic soft-optimality at 1.25 on family ℱ).

**Proof sketch.**
1. Soft certificate ⇒ for representatives of each mass region, T_hybrid ≤ τ · T_best ≤ τ · T_{r†}.  
2. Concentration (A1) + stability (A2) ⇒ T_hybrid(X) ≈ T_hybrid(representative) for X in the same M-cell.  
3. Router agreement (A4) ⇒ f(M_S)=r† outside δ-boundaries; boundary mass controlled.  
4. Menu completeness (A3) lifts T_{r†} to near unrestricted opt.  
5. limsup of ess-sup ratio ≤ τ + boundary penalty + concentration penalty + γ.

### Open gaps (must close to claim Thm D)

| Gap | Need |
|-----|------|
| **D1** | Prove (A2) Lipschitz for T_PDQ, T_SKA, T_PURE in (Înv, û) on ℱ |
| **D2** | Define ℱ explicitly (e.g. Expanded Suite generators as parametric families) |
| **D3** | Measure or bound boundary mass of f under ℱ |
| **D4** | Justify (A5) ε-cover — suite design as covering, not cherry-pick |
| **D5** | (A3) menu completeness: when is min_{ℛ} within (1+γ) of unrestricted? |

---

## 3. Refit dictionary

| Empirical object | Asymptotic object |
|------------------|-------------------|
| Soft_τ(n, Π)=1 | Certificate at scale n for cover of ℱ |
| soft_count=0 @1.20 | τ=1.20 finite |
| geo ≤ 0.90 | Average-case ratio on Π — separate from Soft max |
| Thm C (regret→0) | R→0 ⇒ ratio → 1 relative to menu oracle |
| Thm A/B (sample-opt) | Router is minimax-optimal given M_S |
| **ρ_∞(ℱ,τ)≤τ** | **Menu-competitive asymptotic soft-optimality** |

**Refit success condition:** Thm D gaps D1–D5 closed for a named ℱ, with Soft certificate published for that ℱ’s cover.

---

## 4. What we can say today (honest)

| Claim | Status |
|-------|--------|
| Soft_{1.20}(10^6, ExpandedSuite)=1 on x86_64 | **Measured** (path-a) |
| Soft ⇒ ρ_∞ ≤ 1.20 for some ℱ | **Blocked** on D1–D5 |
| Soft ⇒ classical Inv-optimality | **False** — never |
| Soft + P1+P2 gaps closed ⇒ menu-competitive asymptotic soft-opt | **Candidate bridge** (this doc) |

**Public language until gaps close:**

> Soft=0 is a finite-n path-(a) certificate. Under concentration, residual stability, and menu completeness on a stated family ℱ, it lifts to menu-competitive asymptotic ratio control (SOFT_TO_ASYMPTOTIC.md). That lift is **not yet proved**.

---

## 5. Immediate labor to complete the refit

1. **Name ℱ** — parametric generators for Expanded Suite patterns (random, runs, zipf, gaussianish, …) with n→∞.  
2. **D1** — one-sided inequalities: e.g. T_SKA decreases as û increases in HE regime; T_PURE ≤ c n log û.  
3. **D3** — count Expanded Suite instances within ε of each f threshold (empirical boundary mass).  
4. **D4** — write SUITE_AS_COVER.md: why Π ε-covers ℱ.  
5. Only then: claim ρ_∞(ℱ, τ') ≤ τ' for explicit τ'.

---

## 6. Non-claims

- Soft=0 **does not** by itself imply asymptotic optimality.  
- This document **does not** assert Thm D as proved.  
- Menu-competitive asymptotic soft-optimality ≠ Inv/Runs/LRM optimality.  
- EXTERNAL-clean held. No χ.

**THE BEASTIE BOYZ — Soft certificates lift only under stated assumptions.**
