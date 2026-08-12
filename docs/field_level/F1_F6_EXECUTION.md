# F1–F6 Execution — Full Unconstrained M_S-Optimality

**Status:** F1–F3 **LOCKED**. F4 **OPEN** (blocks theorem). F5–F6 candidate package ready.  
**Date:** 2026-08-12 · THE BEASTIE BOYZ  
**Green light:** User 1–6.  
**Target:** Full unconstrained asymptotic optimality — all n, all X — for measure M_S in model ℳ.  
**Soft:** Relegated. Not part of the optimality argument.

---

## One line

Hybrid residual aims at **M_S-optimality in word-RAM**: match the minimax residual cost forced by the sample measure, for every input.

---

## F1 — Choose M · LOCKED

**Option α locked.**

### Measure M_S

M_S is a **deterministic function of X** (fixed uniform grid of size S, S = 512 primary; dense_inv grid ~8192):

```
M_S(X) = (Înv_S, êq_S, û_S, Runŝ_S, dôm_S, dinv_S) ∈ ℝ^6
```

as in `P1_P2_SAMPLE_OPTIMALITY.md` Definition 1.

**Scalar cost index** (for LB statements):

```
μ(X) := minimax residual cost of the sample-consistent class
μ(X) = min_{r∈ℛ}  max_{Y: M_S(Y)=M_S(X)}  T_r(Y)
```

μ is determined by M_S(X) alone. Full unconstrained optimality will be stated w.r.t. μ (or w.r.t. M_S with μ as the LB).

**Why M_S not classical Inv:** Classical Inv is blocked (sample ≠ exact Inv). Elevating M_S makes the measure **exactly** what the algorithm sees — unconstrained claim becomes possible in principle.

**Alternate held in reserve:** Option β (exact H(LRM)) — requires residual rewrite; not F1 primary.

---

## F2 — Fix model ℳ · LOCKED

**Model ℳ = Word-RAM(w) with w = Θ(log n), unit-cost arithmetic and array access.**

Residual costs counted as:

| Residual | Cost model in ℳ |
|----------|-----------------|
| STRUCTURE | Θ(n) scans |
| PURE (counting / few-unique) | O(n + u) … O(n log u) depending on path |
| PURE (low-disorder / runs) | O(n + Inv_adj) … O(n log n) |
| PDQ | O(n log n) worst-case; typical O(n log n) |
| SKA | O(n · (1 + κ/w)) key-length dependent integer sort |

Comparison lower bounds may be cited as **advice** for PURE/PDQ regions; the official model for the theorem is word-RAM.  
No silent model switching in the final theorem statement.

---

## F3 — Measure access · LOCKED (R1 satisfied)

M_S is computed exactly by the algorithm in O(S log S + n/S · S) = O(S log S + n) time along the hybrid path (grid scan + sample sort).

For S = o(n / log n), probe cost is lower order relative to Ω(n log n) hard instances.  
**R1 discharged for M_S-as-measure:** the algorithm’s measure is exact M_S, not an estimate of a hidden classical M.

---

## F4 — Optimal residuals on each cell · OPEN (blocks theorem)

### Requirement (R2)

For every attainable m in the range of M_S, the residual r = f(m) that hybrid runs must satisfy:

```
T_{f(m)}(Y) ≤ c · μ_m     for all Y ∈ 𝒞(m)
```

where μ_m = min_{r'} max_{Z∈𝒞(m)} T_{r'}(Z).

Equivalently: **f(m) must be a minimax-optimal residual for the cell 𝒞(m).**

### Status of each residual

| Cell type (m) | f(m) | Minimax-optimal on 𝒞(m)? |
|---------------|------|---------------------------|
| STRUCTURE (detected by full scan) | O(n) | **Yes** if detection correct |
| High êq (equal_heavy) | PDQ | **Unknown** — counting might win on 𝒞(m) |
| Low û | PURE counting | **Plausible** if u exact; sample û may undercount |
| HE (high û, high Înv_S) | SKA | **Plausible** in word-RAM for random-like cells; **not proved** worst-case on 𝒞(m) |
| Low Înv_S consecutive | PURE low_disorder | **Unknown** vs PDQ on cell |
| Default | PDQ | Default is often minimax when cell is mixed; **not proved** |

### F4 gap (the hard gap)

**Gap F4.1:** Prove, for each region of the dispatch table, that the chosen residual is within constant c of minimax cost on every Y in 𝒞(m).

Until F4.1 closes, **no full M_S-optimality theorem.**

### F4 attack plan

1. **Cell geometry:** characterize 𝒞(m) — what full arrays share the same grid probes.  
2. **Worst-case residual cost in cell:** bound max_{Y∈𝒞(m)} T_r(Y) for r ∈ ℛ.  
3. **Match:** show f(m) attains the min over r of those maxima, up to c.  
4. Start with HE cells and STRUCTURE cells (easiest); equal_heavy and low-Inv last.

---

## F5 — Upper bound for all X · CONDITIONAL

### Candidate upper bound

For every X:

```
T_hybrid(X) = O(S log S) + T_{f(M_S(X))}(X)
```

If F4.1 holds with constant c:

```
T_hybrid(X) ≤ c · μ(X) + O(S log S)
```

For S ≤ n / log n and μ(X) = Ω(n), the additive term is absorbed:

```
T_hybrid(X) ≤ c' · μ(X)     for all X with μ(X) = Ω(n)
```

STRUCTURE / O(n) cells: μ(X) = Θ(n), probe absorbed.

**Worst-case misrouting:** under F4.1 there is no misrouting relative to μ — f is minimax on the cell. Adversarial X still only forces μ(X), which hybrid matches.

---

## F6 — Theorem package · CANDIDATE (not proved)

### Candidate Theorem (Full M_S-optimality)

**Model.** Word-RAM with w = Θ(log n).  
**Measure.** M_S as in F1; μ(X) = minimax residual cost of 𝒞(M_S(X)).  
**Algorithm.** Hybrid residual router f of `hybrid_residual_menu.hpp` v21 (or successor satisfying F4).

**Claim (candidate).**  
There exists constant c such that for all n ≥ n_0 and all arrays X of length n,

```
T_hybrid(X) ≤ c · μ(X)
```

**Lower bound (any algorithm).**  
Any algorithm that selects a residual from ℛ after observing only M_S(X) (or the S grid cells) requires, on some input in each cell, cost ≥ μ(X). Hence μ is an information-theoretic lower bound for residual-selection algorithms of this form.

**Together:** hybrid is **μ-optimal** (equivalently **M_S-optimal** for residual selection over ℛ) in ℳ — unconstrained, all n, all X.

### Proof outline

1. **LB:** adversary argument on 𝒞(m) (P1 Thm A) — **essentially done**.  
2. **UB:** F4.1 + probe absorption (F5) — **blocked on F4.1**.  
3. **Constants:** explicit c from residual cost models.

### What this theorem is

- Full unconstrained asymptotic optimality **with respect to M_S / μ**.  
- Not Inv-optimality, not Runs-optimality, not classical Mannila M for exact Inv.  
- Free of Soft, free of suites, free of family ℱ.

### What this theorem is not

- Not proved until F4.1 closes.  
- Not a claim that hybrid beats unrestricted optimal algorithms outside ℛ.  
- Not classical synergistic Inv+u optimality.

---

## Execution checklist

| Step | Item | Status |
|------|------|--------|
| F1 | Measure M_S / μ | **LOCKED** |
| F2 | Model word-RAM | **LOCKED** |
| F3 | R1 measure access | **LOCKED** |
| F4 | Minimax residuals on 𝒞(m) | **OPEN — F4.1** |
| F5 | UB all X | Conditional on F4 |
| F6 | Theorem written | **CANDIDATE package** — not proved |

---

## Immediate next (only F4 remains to unlock F5–F6)

1. Formalize 𝒞(m) for the HE region (û_S ≥ 3/4, Înv_S ≥ 2/5): show SKA is within c of minimax on that cell in word-RAM.  
2. STRUCTURE cell: trivial.  
3. equal_heavy cell: compare PDQ vs counting worst-case on high-êq cells.  
4. low-Înv cell: PURE vs PDQ.

When all regions of f satisfy F4.1 → mark theorem **proved** and publish.

---

## Non-claims

- Full M_S-optimality is **not proved**.  
- Hybrid is **not** Inv-optimal.  
- Soft=0 is **not** used in this argument.  
- EXTERNAL-clean. No χ.

**THE BEASTIE BOYZ — F1–F3 locked; F4 is the gate.**
