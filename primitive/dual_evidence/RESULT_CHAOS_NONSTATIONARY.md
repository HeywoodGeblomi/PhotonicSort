# Chaos Non-Stationary Expansion — Dual-Evidence

**Date:** 2026-08-14  
**Status:** **GREEN — 0 collapses / 200**  
**Base:** Claim B non-stationary #153  
**Squad:** THE BEASTIE BOYZ / Blam  
**EXTERNAL-clean.**

---

## New families

| Family | Chaos mode |
|--------|------------|
| telegraph | rapid alternating density |
| pink_burst | heavy-tailed inter-burst gaps |
| adversarial_match | rate-matched scrambled distractor |
| multi_scale_flip | telegraph + late flood |
| dropout | random silence windows (partial obs) |

---

## Results (40 seeds × 5 families)

| Family | σ_Δ flip | dual_differs | wins | collapses | median ratio |
|--------|--------:|-------------:|-----:|----------:|-------------:|
| telegraph | 33/40 | 19/40 | 18/40 | **0** | 1.000 |
| pink_burst | 30/40 | 16/40 | 22/40 | **0** | 0.125 |
| adversarial_match | 32/40 | 15/40 | 25/40 | **0** | 0.125 |
| multi_scale_flip | 29/40 | 16/40 | 20/40 | **0** | 1.000 |
| dropout | 27/40 | 16/40 | 25/40 | **0** | 0.942 |

**TOTAL: wins=110 · collapses=0 · 200 trials.**

---

## Reproduce

```bash
python3 stress_chaos_nonstationary.py
# VERDICT: Claim B gap SURVIVES chaos families (0 collapses)
```

---

**Chaos expanded. Gap holds.**
