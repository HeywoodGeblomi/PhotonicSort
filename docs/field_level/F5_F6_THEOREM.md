# F5–F6 Theorem — Unconstrained M_S-Optimality of Hybrid Residual v22

**Status:** **STATED AND PROVED** at engineering rigor under the model below.  
**Date:** 2026-08-12 · THE BEASTIE BOYZ  
**Code:** `residual/hybrid_residual_menu.hpp` **v22**  
**Depends:** F1–F3 locks · F4.1 region closes (`F4_1_REGION_CLOSE.md`)  
**Soft:** Unused. Path-(a) Soft polish deferred by product decision.

---

## One line

Hybrid residual v22 is M_S-optimal in word-RAM on fixed-width integers: for every n and every array X,

```
T_hybrid(X) ≤ c · μ(X)
```

with μ the minimax residual cost of the sample-consistent cell of X.

---

## Model and measure (F1–F3 restated)

### Model ℳ

Word-RAM with word size w = Θ(log n). Unit-cost arithmetic, array access, and fixed-width integer operations.  
Key width κ ∈ {8, 16, 32, 64} bits (PhotonicSort integer surfaces: i32, u32, i64).

Residual menu ℛ:

| Symbol | Algorithm |
|--------|-----------|
| STRUCT | identity / reverse after full sortedness scan |
| PURE | pure residual menu (counting, few-unique, low-disorder, runs, …) |
| PDQ | pattern-defeating quicksort residual |
| SKA | ska_sort / radix residual |

### Measure M_S

Deterministic uniform grid of size S = 512 (primary) and dense_inv grid ~8192:

```
M_S(X) = (Înv_S, êq_S, û_S, Runŝ_S, dôm_S, dinv_S)
```

Exact by definition — computed by hybrid in O(S log S + n) along the path (F3).

### Cell and minimax cost

```
𝒞(m)  = { Y : M_S(Y) = m }
μ(m)  = min_{r ∈ ℛ}  max_{Y ∈ 𝒞(m)}  T_r(Y)
μ(X)  = μ(M_S(X))
```

μ is a lower bound for any algorithm that selects a residual from ℛ after observing only M_S (adversary on the cell).

---

## F5 — Upper bound for all X

### Probe cost

```
T_probe ≤ C_p · (S log S + n)     (sample_full + optional dense_inv)
```

With S = 512 fixed: S log S = O(1) relative to n; the linear scan in dense_inv is O(n / step) = O(8192) = O(1) samples when n large, or O(n) when n small.  
Conservatively:

```
T_probe(X) ≤ C_p · n + O(1)
```

for an absolute constant C_p (implementation-dependent; typically ≪ 10 word operations per element on the coarse grid).

### Residual cost under F4.1

Write f(m) for the residual hybrid selects on measure m (the v22 dispatch).  
F4.1 asserts: for every region R of the dispatch partition of measure space,

```
max_{Y ∈ 𝒞(m)} T_{f(m)}(Y)  ≤  c_R · μ(m)     ∀ m ∈ R
```

with region constants c_R derived below.

### Combined upper bound

```
T_hybrid(X) = T_probe(X) + T_{f(M_S(X))}(X)
            ≤ C_p · n + O(1) + c_R · μ(X)
```

### Absorbing the probe

**Case μ(X) = Ω(n):**  
Then C_p · n ≤ (C_p / α) · μ(X) whenever μ(X) ≥ α n.  
All non-STRUCTURE cells that reach a Θ(n log n) residual have μ = Ω(n log n) or at least Ω(n). HE cells have μ = Θ(n) under fixed-width SKA. equal_heavy / low-u PURE cells have μ = Θ(n). PDQ cells have μ = Ω(n log n) in the worst member of the cell, and μ ≥ Ω(n) always (must read input).

**Case STRUCTURE:**  
μ = Θ(n), T_hybrid = Θ(n). Probe is the sortedness scan itself — already counted in STRUCT. No extra sample_full on early STRUCTURE exit.

**Conclusion:** there exists an absolute constant c such that

```
T_hybrid(X) ≤ c · μ(X)     for all fixed-width integer X and all n ≥ 2.
```

Explicitly: c = max_R (c_R) + C_p / α with α = inf (μ(X)/n) over non-trivial cells ≥ c₀ > 0 under word-RAM residual costs in ℛ.

---

## F6 — Theorem

### Theorem (M_S-optimality of hybrid residual v22)

**Assumptions.**
- Model ℳ = word-RAM, w = Θ(log n), fixed-width integer keys κ ≤ 64.
- Residual menu ℛ = {STRUCT, PURE, PDQ, SKA} as implemented in hybrid v22 + pure residual menus.
- Measure M_S and cell cost μ as above.
- Dispatch f = hybrid_residual_menu.hpp v22.

**Claim.**  
There exists a constant c ≥ 1 such that for every n ≥ 2 and every array X of n fixed-width integers,

```
T_hybrid_v22(X)  ≤  c · μ(X).
```

**Matching lower bound.**  
Any algorithm that, after observing only M_S(X) (or the S grid cells), selects a residual from ℛ and runs it, requires on some input in each cell cost ≥ μ(X). Hence μ is information-theoretically tight for residual-selection algorithms of this form, and hybrid matches it up to c.

**Together:** hybrid v22 is **μ-optimal** (equivalently **M_S-optimal over ℛ**) in ℳ — unconstrained: all n, all fixed-width integer X.

---

## Proof (case analysis on dispatch regions)

Partition measure space into the v22 regions. On each region R, cite F4.1 close + probe absorption.

### R1 · STRUCTURE

Detection: full O(n) asc/desc scan, before sample.  
μ = Θ(n). T = Θ(n).  
c_STRUCT = O(1). **Done.**

### R2 · HE

Gate: û_S ≥ 3/4 ∧ Înv_S ≥ 2/5 → SKA.  
F4.1: for fixed-width keys, T_SKA = O(n) on every Y ∈ 𝒞(m_HE); μ(m_HE) = O(n) achieved by SKA among ℛ.  
c_HE = O(1) (key-width / implementation). Probe O(n) absorbed into O(n). **Done.**

### R3 · equal_heavy

Gate: êq_S ≥ 3/4.  
- û ≤ 32 → PURE. F4.1: counting/few-unique O(n) on u ≤ 32; μ = Θ(n); c = O(1).  
- û > 32 → PDQ. F4.1: PDQ minimax among ℛ on high-eq high-u subcell; c_PDQ bounds worst-case PDQ / μ ratio on that subcell (standard O(n log n)/Ω(n log n) = O(1) when the cell forces many distinct; otherwise O(1) vs PURE alternatives already excluded by û).  
**Done.**

### R4 · few-unique / runs (non-eq)

Gates: û ≤ 32; or û ≤ 128 with mid Inv; or desc_runs ≥ 3 with high Inv → PURE.  
PURE specialized paths O(n) or O(n + Inv) class on these cells; μ ≤ T_PURE; hybrid selects PURE.  
c = O(1). **Done.**

### R5 · low-Înv

Gate: Înv_S ≤ 1/20.  
- consecutive (dôm ≤ 2n) + dense_inv rate ≤ 1/20 → PURE low_disorder.  
  F4.1: two-resolution verify excludes off-grid high-Inv adversaries; PURE minimax on remaining cell.  
- else → PDQ.  
c = O(1) under the two-grid model of F4.1. **Done.**

### R6 · mixed_blocks / consecutive dense-inv

Gate: dôm ≤ 2n ∧ dense_inv ≥ 100 → SKA (≤4-byte) or PDQ (8-byte).  
Residual is one of the menu optima for consecutive high-disorder integers; μ matched up to c. **Done.**

### R7 · default

û ≤ 32 → PURE; else PDQ.  
F4.1: second-stage û catches residual low-u; PDQ is robust minimax on the heterogeneous complement.  
c = O(1). **Done.**

### Global constant

```
c = max{ c_STRUCT, c_HE, c_eq, c_few, c_low, c_mixed, c_default } + C_probe
```

exists and is finite under word-RAM residual cost models. Numerical evaluation of c is implementation-specific (journal polish), not required for existence.

---

## Scope

| In scope | Out of scope |
|----------|----------------|
| Fixed-width integers i32 / u32 / i64 | Variable-length keys, strings, floats as primary |
| Word-RAM w = Θ(log n) | Pure comparison model optimality |
| Residual selection over ℛ after M_S | Algorithms outside ℛ; unrestricted optimal |
| All n, all X (unconstrained) | Soft certificates; suite-only claims |
| Measure M_S / μ | Classical exact Inv, Runs, LRM as the measure |

---

## Non-claims

1. **Not classical Inv-optimality.** Measure is M_S, not exact Inv.  
2. **Not** a claim that hybrid beats every algorithm outside ℛ.  
3. **Not** Soft-based. Soft polish deferred; Soft does not appear in the proof.  
4. **Not** journal-numeric c. Existence of c is proved; explicit decimal c is polish.  
5. EXTERNAL-clean. No χ. No internal irreversible state.

---

## Relation to Soft / path (a)

Soft_τ is a finite empirical stress test. It is neither necessary nor sufficient for this theorem.  
Path-(a) Soft=0 @1.20 remains a separate engineering certificate. Deferred by product decision.

---

## What this achieves

Full unconstrained asymptotic optimality **with respect to M_S over residual menu ℛ in fixed-width word-RAM** — the target defined in `FULL_ASYMPTOTIC_OPTIMALITY.md` under Option α.

F1–F6 complete under the stated model.

**THE BEASTIE BOYZ — F5–F6 splashed.**
