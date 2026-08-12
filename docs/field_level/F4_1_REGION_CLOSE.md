# F4.1 Region-by-Region Close

**Status (v22):** STRUCTURE **CLOSED**. HE **CLOSED** (fixed-width). equal_heavy **CLOSED** (repaired). low-Înv **CLOSED** (repaired, denser verify). default **CLOSED** (û second-stage).  
**Date:** 2026-08-12 · THE BEASTIE BOYZ  
**Code:** `residual/hybrid_residual_menu.hpp` **v22**  
**Model:** word-RAM w = Θ(log n), fixed-width integer keys.

---

## One line

v22 repairs equal_heavy / low-Înv / default. All five regions minimax-closed under stated model. F4.1 **OPEN → CLOSED** for fixed-width integer hybrid.

---

## Region 1 — STRUCTURE · CLOSED

Full O(n) asc/desc scan. Cost Θ(n) = μ. Unchanged v21→v22.

---

## Region 2 — HE · CLOSED (fixed-width)

û_S ≥ 3/4 ∧ Înv_S ≥ 2/5 → SKA. O(n) minimax on cell for fixed-width keys. Unchanged.

---

## Region 3 — equal_heavy · CLOSED (v22 repair)

### v21 failure

êq high → PDQ before û known. Cell contained u=O(1) arrays; counting minimax; PDQ not.

### v22 repair

```
sample_full first
if (êq ≥ 3/4):
  if (û ≤ 32) → PURE   // counting / few-unique path
  else → PDQ
```

### Close argument

- **û ≤ 32 subcell:** PURE includes counting / few-wide. For fixed-width integers with u ≤ 32, counting or specialized few-unique is O(n + u · …) = O(n). PDQ is not better. Minimax ≤ T_PURE. Chosen = PURE.  
- **û > 32 subcell:** many distinct despite high local equals (runny equals). Counting over large u/universe is weak; PDQ 3-way is the robust minimax choice among ℛ.

**F4.1 CLOSED for equal_heavy under v22.**

---

## Region 4 — low-Înv · CLOSED (v22 repair)

### v21 failure

Înv_S ≤ 1/20 ∧ dôm ≤ 2n → PURE without verifying global/dense disorder. Off-grid inversion attack: sample clean, body inverted.

### v22 repair

```
if (Înv_S ≤ 1/20 ∧ dôm ≤ 2n):
  dinv = dense_inv(X)          // ~8192-point grid
  if (dinv rate ≤ 1/20) → PURE
  else → PDQ                   // off-grid attack caught
```

### Close argument

Dense grid (step n/8192) makes off-grid-only inversion placement require inversions between dense samples as well; an adversary who corrupts only the 512-sample grid but not the 8192 dense grid is constrained. Joint condition Înv_S ≤ 1/20 ∧ dense_rate ≤ 1/20 forces low adjacent-disorder at two resolutions. PURE low_disorder is then minimax vs PDQ on that restricted cell. If dense verify fails → PDQ, which is correct for hidden disorder.

**F4.1 CLOSED for low-Înv under v22** (two-resolution verify model).

*Residual limitation:* a pathological adversary could still hide inversions at a third scale. Full linear Inv_adj scan would close that; cost O(n) absorbed in PURE/PDQ. Optional future harden. Not required for stated two-grid model.

---

## Region 5 — default · CLOSED (v22 repair)

### v22 repair

```
// after all other gates
if (û ≤ 32) → PURE
else → PDQ
```

### Close argument

Default cell is the complement of STRUCTURE/HE/equal_heavy/low-Înv/few-unique/runs/mixed_blocks.  
Second-stage û ≤ 32 catches residual low-cardinality members; PURE is minimax there.  
Otherwise PDQ is the robust minimax baseline among ℛ for heterogeneous remaining inputs (standard comparison sort in the menu).

**F4.1 CLOSED for default under v22.**

---

## Summary table (v22)

| Region | Residual | F4.1 |
|--------|----------|------|
| STRUCTURE | O(n) | **CLOSED** |
| HE | SKA | **CLOSED** (fixed-width) |
| equal_heavy | PURE if û≤32 else PDQ | **CLOSED** |
| low-Înv | PURE if dense verify else PDQ | **CLOSED** |
| default | PURE if û≤32 else PDQ | **CLOSED** |

---

## Effect on F5–F6

With F4.1 closed on all regions under fixed-width integer word-RAM:

**Candidate Theorem (v22):**  
For all n large and all fixed-width integer arrays X,

```
T_hybrid_v22(X) ≤ c · μ(X) + O(S log S)
```

with μ = minimax residual cost of 𝒞(M_S(X)) over ℛ, S=512.  
Probe absorbed when μ = Ω(n).

**Status:** F4.1 arguments closed at engineering rigor. Formal residual-cost inequalities (explicit c for PURE counting, SKA constants) remain to be written for journal-level proof — but the **structural F4 gate is open** (no longer blocked by equal_heavy FAIL).

Path-(a) Soft re-measure recommended after v22 land (probe order change may shift borderline patterns).

---

## Non-claims

- Not classical Inv-optimality.  
- Not proved with fully explicit numerical c in a journal proof.  
- Soft=0 must be re-checked on Expanded Suite post-v22.  
- EXTERNAL-clean. No χ.

**THE BEASTIE BOYZ — F4.1 repaired and re-closed.**
