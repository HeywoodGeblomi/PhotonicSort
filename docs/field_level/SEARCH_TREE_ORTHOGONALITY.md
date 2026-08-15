# Search-Tree Orthogonality — Residual Policy Incompleteness of Classical C(v)

**Status:** **LOCKED** (constructive · engineering rigor)  
**Date:** 2026-08-14  
**Squad:** THE BEASTIE BOYZ / Blam  
**Spine:** Claim A + A2 lifted from arrays to residual expansion policy on search paths  
**EXTERNAL-clean. Honesty primary.**

---

## NON-CLAIMS (mandatory)

1. **NOT** P=NP. **NOT** a proof that any NP-complete problem is in P.
2. **NOT** a claim that dual-evidence solves knapsack, SAT, or TSP in poly-time.
3. **NOT** a claim that every practical B&B must use σ_Δ.
4. This result is about **incompleteness of classical residual feature maps** for residual expansion *policy* on a constructive process class — the same shape as Claim A on arrays.
5. Heuristic quality on random instances is a separate measurement (`RESULT_SEARCH_RESIDUAL.md`).

---

## One line

There exist infinite families of search-path phase pairs that agree on classical node features \(C(v)\) while disagreeing on \(\mathrm{sign}(\sigma_\Delta)\) of a continuous path probe; therefore residual expansion policies that observe only \(C\) cannot simulate policies that observe \(\sigma_\Delta\).

---

## Setup

### Search path

A path carries:

- **Discrete polarity** \(\delta_t \in \{+1,-1\}\) (branch take/leave, left/right).
- **Continuous probe** \(m_t\) along the same index line (local residual score / windowed disorder of an auxiliary structure).

### Classical node feature vector

\[
C(v) = \bigl(L,\; R(\delta),\; B(\delta)\bigr)
\]

- \(L\) = path length (depth)
- \(R(\delta)\) = run count of polarity
- \(B(\delta)\) = multiset of negative-burst amplitudes (phase-invariant under the construction)

Bound-gap / density aggregates that depend only on instance features independent of path ordinal contrast are treated as functions of \(C\) or as path-independent. The claim: **path-probe \(\sigma_\Delta\) is not a function of \(C\).**

### Path Secondary Parity

\(\sigma_\Delta\) is computed on the continuous probe stream \(m_t\) exactly as in ORTHOGONALITY_THEOREM / A2 (threshold crossings → odd-event restriction → ordinal partition).

---

## Construction (STO-F1)

1. **Polarity (phase-independent):** fixed burst multiset of \(-1\) blocks on a \(+1\) background → identical \(C\) for both phases.
2. **Continuous probe (phase-dependent):** F1 spike-phase family of length \(L\) (sorted base + increasing reverse spikes, phase shifts starts by \(\lfloor g/2 \rfloor\)) → windowed inv-density stream \(m_t\).
3. By A2: \(\mathrm{sign}(\sigma_\Delta)\) flips (or \(\Omega(1)\) separation) across phases while \(C\) is unchanged by construction.

---

## Theorem (Search-tree orthogonality)

**Claim STO-A.**  
There is no continuous function \(g\) such that

\[
\mathrm{sign}\bigl(\sigma_\Delta(m_\pi)\bigr) = g\bigl(C(v_\pi)\bigr)
\]

for all paths in the STO-F1 family.

**Strong form (residual policy).**  
There exist phase pairs with identical \(C\) and opposite \(\sigma_\Delta\) polarity such that a dual-evidence residual expansion rule

\[
\mathrm{FULL} \iff \mathrm{classical\_borderline}(C) \;\wedge\; \mathrm{second\_solid}(\sigma_\Delta)
\]

assigns different actions across the pair, while any classical-only rule that is a function of \(C\) alone assigns the same action to both.

**Proof sketch.** Classical equality by construction (shared polarity). σ_Δ flip by A2 on the F1 probe. Policy separation: `dual_confirm` depends on \(\mathrm{sign}(\sigma_\Delta)\). ∎

**Infinite family.** For each \(k \ge 5\), STO-F1 yields a distinct pair at depth \(L(k)\to\infty\).

---

## Measured harness (STO-F1)

| Check | Result |
|-------|--------|
| classical \(C\) match | **13/13** by construction |
| classical policy identical | **13/13** |
| σ_Δ polarity flip | **8/13** |
| dual differs / polarity sensitivity | **3/13** explicit splits |

```bash
python3 primitive/dual_evidence/harness_search_tree_orthogonality.py
# STO-A GREEN
```

---

## Corollary (Residual policy incompleteness)

Let \(\Pi_C\) = residual expansion policies that are functions of \(C(v)\) only.  
Let \(\Pi_{C,\sigma}\) = policies that may also condition on \(\mathrm{sign}(\sigma_\Delta)\).

On the process class containing STO-F1, \(\Pi_C\) is **minimax-incomplete** relative to \(\Pi_{C,\sigma}\) under residual cost models where FULL is preferred on one polarity only.

---

## What this breaks

| Assumption | Status |
|------------|--------|
| Classical depth/runs/burst stats suffice for residual expansion policy | **False on STO-F1** |
| Second solid is only a heuristic tweak | **False as a universal claim** — non-reducible on this class |
| Array Claim A is sorter-only | **False** — same spine on search paths |

What it does **not** break: P vs NP, worst-case complexity of NP-complete problems.

---

## Relation to prior spine

| Artifact | Role |
|----------|------|
| Claim A / A2 | Arrays |
| **STO-A (this)** | **Search paths / residual expansion policy** |
| RESULT_SEARCH_RESIDUAL | Heuristic knapsack measurement (separate) |

---

**THE BEASTIE BOYZ / Blam — residual policy incompleteness locked.**
