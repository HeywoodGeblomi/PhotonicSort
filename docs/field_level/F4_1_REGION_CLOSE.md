# F4.1 Region-by-Region Close

**Status:** STRUCTURE **CLOSED**. HE **CLOSED** (fixed-width integers). equal_heavy **FAIL**. low-Înv / default **PARTIAL**.  
**Date:** 2026-08-12 · THE BEASTIE BOYZ  
**Gate:** F4.1 = chosen residual within constant c of minimax cost on every X ∈ 𝒞(m).  
**Model:** word-RAM w = Θ(log n), fixed-width integer keys (8/32/64-bit) unless noted.

---

## One line

STRUCTURE and HE close under stated model. equal_heavy does **not** — PDQ short-circuit loses to counting when u is tiny. That is a real F4 gap, not paperwork.

---

## Region 1 — STRUCTURE · CLOSED

### Detection

Full O(n) scans `is_sorted_asc` / `is_sorted_desc` **before** any sample residual:

```
if (is_sorted_asc(a, n)) return 0;
if (is_sorted_desc(a, n)) { reverse; return 0; }
```

Not a sample cell. Exact structure.

### Cell

Inputs that are fully ascending or fully descending.

### Minimax cost

μ = Θ(n) (must read the array; output is sorted or one reverse).

### Chosen residual

O(n) verify + optional reverse. Cost Θ(n).

### Close

```
T = Θ(n) = μ
```

Constant c = 1. **F4.1 CLOSED** for STRUCTURE.

---

## Region 2 — HE · CLOSED (fixed-width)

### Cell m_HE

```
û_S ≥ 3/4  ∧  Înv_S ≥ 2/5
```

Sample has ≥ (3/4)S distinct values and adjacent-inversion rate ≥ 2/5 on the grid.  
Hence every Y ∈ 𝒞(m_HE) has at least u ≥ (3/4)S distinct keys (S = 512 ⇒ u ≥ 384).

### Residual costs on 𝒞(m_HE)

| Residual | Worst-case cost on 𝒞(m_HE) |
|----------|----------------------------|
| SKA | O(n · (κ/w)) = **O(n)** for fixed-width κ ∈ {8,32,64}, w = Θ(log n) |
| PDQ | Θ(n log n) comparisons in the worst case over distinct-heavy inputs |
| PURE counting | O(n + U) only if universe U is small; HE cells have high sample uniqueness — universe may be large ⇒ counting infeasible or O(n+U) with U ≥ 2^κ |
| STRUCTURE | Not applicable (Înv_S high ⇒ not sorted) |

### Minimax

```
μ(m_HE) = min_r max_{Y∈𝒞} T_r(Y) = O(n)
```

achieved by SKA among ℛ under fixed-width word-RAM.  
Any comparison-only residual is Ω(n log u) ≥ Ω(n log 384) = Ω(n) with a larger leading constant for large n when u → n.

### Chosen residual

`ska_sort`. Cost O(n) fixed-width.

### Close

```
T_SKA(Y) ≤ c_HE · μ(m_HE)   for all Y ∈ 𝒞(m_HE)
```

with c_HE = O(1) depending on key width / implementation constants.

**F4.1 CLOSED for HE under fixed-width integer word-RAM.**

### Explicit limitation

If keys are variable-length or κ = ω(w log n), SKA is no longer O(n). Close does **not** extend to unbounded key length. Our PhotonicSort integer surfaces (i32/u32/i64) are fixed-width — in scope.

---

## Region 3 — equal_heavy · FAIL

### Detection

```
quick_equal_heavy: êq_S ≥ 3/4 on S=256 grid  →  residual_pdqsort
```

Short-circuits **before** `sample_full`, so û is unknown at decision time.

### Cell m_eq

High adjacent-equal rate on the grid. Compatible with:

- (a) u = 2 (two values alternating in blocks) — counting O(n)
- (b) u large but locally runny equals — PDQ strong
- (c) already-sorted runs of equals — STRUCTURE already filtered if fully sorted

### Minimax vs chosen

On (a): max cost of counting = O(n); max cost of PDQ = Θ(n log n) possible on adversarial equal-heavy patterns with structure that defeats branch prediction / partitions poorly, or simply larger constant × n log u with u=2 still involving overhead. More cleanly:

```
μ(m_eq) ≤ T_counting ≤ O(n + u) = O(n)   when u = O(1) ∈ 𝒞(m_eq)
T_PDQ ≥ c n log n on some equal-heavy instances (standard quicksort-style bounds with few distinct still pay partition overhead; even 3-way pdq is Ω(n log u) with u small giving Ω(n) but with worse constants than counting)
```

**Decisive failure mode:** 𝒞(m_eq) contains arrays with u = O(1). Counting (PURE) is O(n). Hybrid never selects counting on this path. PDQ is not minimax on those members of the cell.

### Close status

**F4.1 FAIL for equal_heavy** under current v21 dispatch.

### Repair (does not claim close until shipped)

After êq_S ≥ 3/4, take û (cheap subsample or finish sample_full) and route:

```
if (êq high ∧ û ≤ 32) → PURE counting
else if (êq high) → PDQ
```

Until that lands and is re-proved, equal_heavy remains **FAIL**.

---

## Region 4 — low-Înv · PARTIAL

### Cell m_low

```
Înv_S ≤ 1/20
```

Split by dôm:

- **consecutive:** dôm ≤ 2n → PURE low_disorder  
- **sparse:** dôm > 2n → PDQ

### Subcase consecutive · plausible close

Low adjacent-inversion rate + domain O(n) ⇒ input is nearly sorted with keys in a tight range.  
PURE low_disorder (insertion / pattern-defeating introsort on low Inv) is O(n + Inv_adj · polylog) class.  
PDQ is O(n log n).  
Minimax is the low-disorder residual when Inv_adj = o(n log n / polylog).

**Gap L-low.1:** Prove max_{Y∈𝒞(m_low,consec)} T_PURE(Y) ≤ c · μ. Requires PURE’s low_disorder path to be Inv-adaptive with explicit bound, and that every Y in the cell truly has low global Inv_adj (sample Înv_S ≤ 1/20 does **not** force global Inv_adj ≤ 1/20 — adversarial concentration of inversions off-grid).

**Off-grid attack:** Place all inversions between grid points. Then Înv_S = 0 but Inv_adj = Θ(n). PURE may degrade; PDQ may win. Cell membership by sample alone does **not** guarantee low global disorder.

### Subcase sparse · PDQ default

Low sample inv + large domain → not a counting fit; PDQ is reasonable. Minimax unclear vs SKA.

### Close status

**PARTIAL.** Conditional close if we add a linear verify of Inv_adj (or dense_inv threshold) before PURE, mirroring STRUCTURE’s full scan honesty. Without verify: **not closed** (off-grid inversion attack).

---

## Region 5 — default · PARTIAL

### Cell

Everything not captured above → PDQ.

### Argument

Default cell is heterogeneous. PDQ is a robust O(n log n) baseline.  
Minimax over a heterogeneous cell is often the robust comparison sort. SKA can win on some members (integer random); PURE on others (hidden low-u off-sample).

### Off-sample low-u attack

Sample û_S moderate but global u = 2 off-grid heavy collision — rare with S=512 systematic sample, but possible for structured adversaries. Then counting wins and PDQ is not minimax.

### Close status

**PARTIAL.** PDQ is a defensible default; not proved minimax for all Y in the residual cell. Strengthening: require û_S and Înv_S away from other region thresholds by a margin (Voronoi interior) so boundary adversaries are excluded from “default.”

---

## Summary table

| Region | Residual | F4.1 | Note |
|--------|----------|------|------|
| STRUCTURE | O(n) | **CLOSED** | Full scan |
| HE | SKA | **CLOSED** | Fixed-width word-RAM only |
| equal_heavy | PDQ | **FAIL** | u=O(1) members need counting |
| low-Înv consecutive | PURE | **PARTIAL** | Off-grid Inv attack |
| low-Înv sparse | PDQ | **PARTIAL** | — |
| default | PDQ | **PARTIAL** | Heterogeneous cell |

---

## Effect on F5–F6 theorem

Full unconstrained M_S-optimality for **current v21 hybrid** is **blocked** by equal_heavy FAIL and low-Înv/default PARTIAL.

**Restricted theorem (available now):**

> On the subclass of inputs routed to STRUCTURE or HE under v21, with fixed-width integer keys, hybrid achieves minimax residual cost on the corresponding cells (c = O(1)).

That is unconstrained on that subclass (all n, all such X), not on all X.

---

## Repair order to reopen full F4

1. **equal_heavy:** after high êq, measure û; if û ≤ 32 → PURE counting; else PDQ. Re-prove cell.  
2. **low-Înv consecutive:** linear Inv_adj verify (or dense_inv bound) before PURE; on verify fail → PDQ.  
3. **default:** margin zones around thresholds; optional second-stage û check.  
4. Re-run F4.1 on repaired regions → full theorem candidate.

---

## Non-claims

- Full M_S-optimality for all X is **not** closed.  
- HE close is **not** classical Inv-optimality.  
- Soft=0 is unused here.  
- EXTERNAL-clean. No χ.

**THE BEASTIE BOYZ — close what is true; fail what is false.**
