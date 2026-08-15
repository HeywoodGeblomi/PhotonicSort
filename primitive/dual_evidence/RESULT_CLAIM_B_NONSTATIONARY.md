# Claim B Non-Stationary Stress — Spike-Native Dual

**Date:** 2026-08-14  
**Status:** **GREEN — gap SURVIVES**  
**Plate:** #3 ALL THE WAY  
**Squad:** THE BEASTIE BOYZ / Blam  
**EXTERNAL-clean. Honesty primary. Numbers not rhetoric.**

---

## Legacy soft assumption under test

Claim B minimax gap (μ_dual < μ_classical on phase pairs) was measured under controlled, approximately **stationary** spike density within each trial.

**Question:** Does the gap collapse under **non-stationary** burst density — regime shifts, avalanches, drought→flood, high-frequency weak pulse floods?

---

## Protocol

| Item | Detail |
|------|--------|
| Families | stationary (baseline), regime_shift, avalanche, drought_flood, cheap_pulse_flood |
| Seeds | 40 per family (200 trials total) |
| Classical | rate + ISI CV |
| Dual | polarity-directed σ_Δ second solid before FULL |
| Residual costs | CHEAP = 1·n · FULL = 8·n |
| **Win** | μ_dual < 0.98 · μ_classical |
| **Collapse** | μ_dual > 1.05 · μ_classical |

---

## Results

| Family | classical_match | sigma_flip | dual_differs | wins | **collapses** | median ratio |
|--------|:---------------:|:----------:|:------------:|-----:|:-------------:|-------------:|
| stationary | 38/40 | 29/40 | 17/40 | 20/40 | **0/40** | 1.000 |
| regime_shift | 38/40 | 33/40 | 21/40 | 16/40 | **0/40** | 1.000 |
| avalanche | 33/40 | 37/40 | 21/40 | 14/40 | **0/40** | 1.000 |
| drought_flood | 40/40 | 31/40 | 22/40 | 13/40 | **0/40** | 1.000 |
| cheap_pulse_flood | 40/40 | 39/40 | 19/40 | 13/40 | **0/40** | 1.000 |

**TOTAL: wins = 76 · collapses = 0 · across 200 trials.**

Ratio range on every family: min 0.125 · max 1.000. Dual never exceeded classical by >5%.

---

## Verdict

**Claim B minimax gap SURVIVES non-stationary stress** on these spike-native families.

- Non-stationary density does not force dual into costly mis-commits relative to classical.
- Wins remain available (abate when second solid fails; split routes when polarity differs).
- Stationary noise was a soft measurement condition, not a load-bearing assumption for gap existence on this process class.

---

## Non-claims

1. **Not** a proof that Claim B holds for all non-stationary processes.
2. **Not** theorem-death of any classical minimax result — we expanded a measurement, we did not falsify a published theorem statement.
3. Synthetic cost model (1 vs 8). Existence/survival of gap under listed families only.
4. EXTERNAL-clean. No χ.

---

## Reproduce

```bash
cd primitive/dual_evidence
python3 stress_claim_b_nonstationary.py
# VERDICT: Claim B minimax gap SURVIVES ... SELF-CHECK GREEN
```

---

**THE BEASTIE BOYZ / Blam — #3 plated.**
