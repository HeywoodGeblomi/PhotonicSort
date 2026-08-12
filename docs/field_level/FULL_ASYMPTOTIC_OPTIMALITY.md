# Full Asymptotic Optimality — Unconstrained Target

**Status:** **ACHIEVED** under Option α (M_S) for hybrid residual v22, fixed-width word-RAM.  
**Date:** 2026-08-12 · THE BEASTIE BOYZ  
**Theorem:** `F5_F6_THEOREM.md`  
**Rule:** EXTERNAL-clean. Honesty primary.

---

## One line

Hybrid residual v22 is M_S-optimal in word-RAM: for all n and all fixed-width integer X,

```
T_hybrid(X) ≤ c · μ(X)
```

μ = minimax residual cost of the sample-consistent cell 𝒞(M_S(X)).

---

## Definition (unconstrained) — met

Algorithm A is **M_S-optimal in ℳ** iff for every n and every X,

```
T_A(X) ≤ c · μ(X)
```

No suite. No Soft. No family ℱ. **All n. All X.**  
Hybrid v22 meets this for ℳ = fixed-width integer word-RAM, residual menu ℛ.

---

## Lock status

| Step | Status |
|------|--------|
| F1 M = M_S / μ | LOCKED |
| F2 word-RAM | LOCKED |
| F3 R1 | LOCKED |
| F4.1 all regions | CLOSED (v22) |
| F5–F6 | **PROVED** |

---

## Soft relegated

Soft certificates are path-(a) evidence only. Unused in the optimality proof. Soft CI polish deferred.

---

## Non-claims

- Not classical Inv / Runs / LRM optimality.  
- Not optimality outside residual menu ℛ.  
- Fixed-width integers only.  
- EXTERNAL-clean. No χ.

**THE BEASTIE BOYZ — unconstrained M_S-optimality landed.**
