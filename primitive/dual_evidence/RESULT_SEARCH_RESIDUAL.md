# Heuristic Search Residual — Dual-Evidence Branch Gate

**Date:** 2026-08-14  
**Status:** **GREEN (heuristic only)**  
**Domain:** 0/1 knapsack DFS branch-and-bound  
**Squad:** THE BEASTIE BOYZ / Blam  
**EXTERNAL-clean.**

---

## NON-CLAIMS (read first)

1. **NOT** a polynomial-time algorithm for knapsack that changes complexity class.
2. **NOT** P=NP. **NOT** an NP-complete breakthrough.
3. Dual mode is a **heuristic** residual gate. It may return suboptimal incumbents.
4. Classical mode is the complete B&B baseline for quality comparison.
5. Small-n synthetic existence proof only: dual-evidence can change which residual expansions fire in search.

---

## One line

Same CHEAP vs FULL residual commitment shape as the sensor/spike demos: on borderline bound gaps, require σ_Δ second solid on the path polarity stream before FULL node expansion; otherwise abate (CHEAP prune).

---

## Protocol

| Item | Detail |
|------|--------|
| Problem | 0/1 knapsack, density-sorted, fractional UB |
| Classical | expand iff UB > incumbent |
| Dual | strong gaps always expand; borderline require `dual_confirm` on path ±1 stream |
| n | 12, 16, 20, 24 · 12 seeds each |

---

## Results

| Metric | Value |
|--------|------:|
| trials | 48 |
| opt_match (dual = classical best) | 13/48 |
| **median quality** (d_best/c_best) | **0.990** |
| min quality | 0.955 |
| **median exp_ratio** | **0.239** |
| min exp_ratio | 0.049 |

Dual expands fewer residual nodes (~4× median reduction). Quality stays high on median but is not guaranteed optimal — expected for heuristic abate.

---

## Interpretation

| Analogy | Search |
|---------|--------|
| FULL residual | Expand borderline node |
| CHEAP residual | Prune / skip expansion |
| Second solid fails | Abate → fewer nodes, possible quality gap |
| Classical complete | Bound-only B&B baseline |

This is **residual selection inside search**, not a complexity result.

---

## Reproduce

```bash
cd primitive/dual_evidence
python3 demo_search_residual.py
# SELF-CHECK GREEN
```

---

**THE BEASTIE BOYZ / Blam — heuristic only, receipts intact.**
