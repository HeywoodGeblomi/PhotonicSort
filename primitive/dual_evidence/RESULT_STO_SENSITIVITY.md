# STO Sensitivity Pressure — Residual Minimax under Polarity Cost

**Date:** 2026-08-14  
**Status:** **GREEN**  
**Weapon:** Sensitivity  
**Squad:** THE BEASTIE BOYZ / Blam  
**EXTERNAL-clean.**

---

## NON-CLAIMS

1. **NOT** P=NP. **NOT** an NP-complete breakthrough.
2. Synthetic polarity-sensitive residual cost model (FULL_CORRECT=2, FULL_WRONG=12, CHEAP=1).
3. Process-class incompleteness of classical C + measured minimax gap under that cost model — not a universal practical claim.

---

## One line

Sensitivity to path-probe polarity is the residual decision instrument: classical policy is identical across phase pairs; dual splits; minimax residual cost favors dual when FULL is polarity-correct.

---

## Results (k=5..21, 17 trials)

| Metric | Value |
|--------|------:|
| sigma_flip | **11/17** |
| dual_differs | **6/17** |
| polarity sensitivity | **6/17** |
| **minimax wins** (μ_dual < 0.98·μ_classical) | **13/17** |
| median residual ratio | **0.167** |
| min ratio | 0.083 |
| max ratio | 1.000 |

Classical residual policy never splits phase pairs. Dual does. When FULL is expensive on the wrong polarity, dual's sensitivity is the minimax advantage.

---

## Reproduce

```bash
cd primitive/dual_evidence
python3 harness_sto_sensitivity.py
# SENSITIVITY PRESSURE GREEN
```

---

**Sensitivity is the weapon. Honesty is the saving grace.**
