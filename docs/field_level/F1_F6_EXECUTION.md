# F1–F6 Execution — Full Unconstrained M_S-Optimality

**Status:** F1–F3 **LOCKED**. F4 **PARTIAL** (STRUCTURE+HE closed; equal_heavy FAIL; low-Înv/default PARTIAL). F5–F6 restricted theorem only.  
**Date:** 2026-08-12 · THE BEASTIE BOYZ  
**F4.1 detail:** `F4_1_REGION_CLOSE.md`

---

## One line

STRUCTURE + HE are minimax-closed. equal_heavy is a real fail (counting). Full theorem waits on repairs.

---

## F1 — M · LOCKED

M_S deterministic sample measure; μ(X) = minimax residual cost of 𝒞(M_S(X)).

## F2 — ℳ · LOCKED

Word-RAM w = Θ(log n), fixed-width integers for HE close.

## F3 — R1 · LOCKED

M_S exact by definition.

## F4 — residuals on cells · PARTIAL

| Region | Status |
|--------|--------|
| STRUCTURE | **CLOSED** |
| HE (fixed-width) | **CLOSED** |
| equal_heavy | **FAIL** — repair: û gate → counting |
| low-Înv | **PARTIAL** — off-grid Inv; repair: verify |
| default | **PARTIAL** |

## F5–F6

**Restricted theorem (proved sketch):** on STRUCTURE ∪ HE inputs, fixed-width keys, T_hybrid ≤ c · μ on those cells.

**Full theorem:** blocked until equal_heavy FAIL repaired and PARTIALs closed.

See `F4_1_REGION_CLOSE.md` for proofs and failure modes.

---

## Non-claims

- Full M_S-optimality for all X **not** proved.  
- Soft unused. EXTERNAL-clean. No χ.

**THE BEASTIE BOYZ — F4.1 honest.**
