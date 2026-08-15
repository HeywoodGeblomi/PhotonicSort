# Neural Modulation — Priority-Ranked Performance

**Date:** 2026-08-14  
**Status:** **GREEN**  
**Base:** #157 population + stream  
**Squad:** THE BEASTIE BOYZ / Blam  
**EXTERNAL-clean.**

---

## Diagnostic priority (highest → lowest)

| Rank | Metric | Role |
|-----:|--------|------|
| 1 | **σ_Δ flip** | Core signal hunter — structural drift detection |
| 2 | **batch minimax** | Stress limiter — poison-pattern residual cost |
| 3 | stream minimax | Baseline fluidity (solved — hold) |
| 4 | classical match | Sanity constraint (hold) |

---

## Modulations

**A. Phase-lock volatility (σ_Δ)**  
Mild multi-scale population σ_Δ: amplitude × log-ISI weight (dense bursts weigh more). Momentum floor from recent |Δσ| windows — front-run volatility without exploding scale.

**B. Structural bypass (batch)**  
If window spike count thin or inter-channel rate coherence low → classical-only. Trims dual overhead on incoherent/poison micro-batches.

---

## Results vs baseline (#157)

| Metric | Baseline | Modulated | Δ |
|--------|--------:|----------:|--:|
| classical match | 72/72 | **72/72** | 0 |
| **σ_Δ flip** | 54/72 | **63/72** | **+9** |
| dual_differs | 20/72 | 21/72 | +1 |
| batch minimax wins | 48/72 | 46/72 | −2 |
| stream minimax wins | 72/72 | **72/72** | 0 |

Priority #1 improved. Priority #3–4 held. Priority #2 paid a small tax (−2). Aggressive multi-scale (un-normalized 1/ISI) maxed flip to 72/72 but collapsed batch/stream — rejected; mild log-ISI is the locked balance.

---

## Non-claims

Synthetic suite only. Cost model synthetic. Not a claim on real neuromorphic silicon.

---

## Reproduce

```bash
cd primitive/dual_evidence
python3 demo_neuro_modulated.py
# DELTA: flip +9 · batch_wins -2 · stream +0 · match +0
```

---

**Sensitivity is the weapon. Priority ranking is the scope.**
