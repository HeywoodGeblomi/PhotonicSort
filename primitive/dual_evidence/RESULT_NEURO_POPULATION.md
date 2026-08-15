# Neural Expansion — Population σ_Δ + Streaming Residual

**Date:** 2026-08-14  
**Status:** **GREEN**  
**Base:** spike-native #152 · non-stationary Claim B #153  
**Squad:** THE BEASTIE BOYZ / Blam  
**EXTERNAL-clean.**

---

## Expansion axes

| Axis | Detail |
|------|--------|
| **Population** | N channels (4–12); pooled odd-spike amplitude σ_Δ |
| **Streaming** | Online residual commits per time window (not batch-only) |
| **Sensitivity** | Dual splits residual routes; classical matched |

---

## Results (72 trials)

| Metric | Value |
|--------|------:|
| classical rate/CV match | **72/72** |
| σ_Δ polarity flip | **54/72** |
| batch dual_differs | **20/72** |
| batch minimax wins | **48/72** |
| **stream minimax wins** | **72/72** |

Streaming residual under dual-evidence won every phase-pair minimax trial in this suite.

---

## Non-claims

1. Synthetic population — not real DVS / Allen / OpenSpike traces.
2. Cost model synthetic (CHEAP=1, FULL=8).
3. Not a claim that neuromorphic chips must ship this.
4. EXTERNAL-clean. No χ.

---

## Reproduce

```bash
cd primitive/dual_evidence
python3 demo_neuro_population.py
# NEURAL EXPANSION GREEN
```

---

**Back to the neural action. Expanded.**
