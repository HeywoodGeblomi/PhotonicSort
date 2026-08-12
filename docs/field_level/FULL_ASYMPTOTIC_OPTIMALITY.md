# Full Asymptotic Optimality — Unconstrained Target

**Status:** TARGET + F1–F3 LOCKED under Option α (M_S). **Not achieved** (F4 open).  
**Date:** 2026-08-12 · THE BEASTIE BOYZ  
**Active execution:** `F1_F6_EXECUTION.md`  
**Rule:** EXTERNAL-clean. Honesty primary.

---

## One line

Full asymptotic optimality is free of Soft and free of suite certificates.  
It is a lower bound + matching upper bound for measure M_S in word-RAM, for all n, all X.

---

## Definition (unconstrained)

Algorithm A is **M_S-optimal in ℳ** iff for every n and every X,

```
T_A(X) ≤ c · μ(X)
```

where μ(X) = min_r max_{Y: M_S(Y)=M_S(X)} T_r(Y) (minimax residual cost of the sample-consistent class).

No suite. No Soft. No family ℱ. **All n. All X.**

---

## Lock status (green light F1–F6)

| Step | Status |
|------|--------|
| F1 Choose M = M_S / μ | **LOCKED** |
| F2 Model = word-RAM | **LOCKED** |
| F3 Measure access | **LOCKED** |
| F4 Optimal residuals on 𝒞(m) | **OPEN** |
| F5 UB all X | Conditional on F4 |
| F6 Theorem | Candidate — not proved |

---

## Soft relegated

Soft certificates are path-(a) evidence only. They are not the optimality argument.

---

## Non-claims

- Full M_S-optimality is **not proved**.  
- Not Inv-optimal.  
- EXTERNAL-clean. No χ.

**THE BEASTIE BOYZ — F4 is the gate.**
