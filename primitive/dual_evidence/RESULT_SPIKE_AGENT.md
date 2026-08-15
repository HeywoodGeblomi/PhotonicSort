# Closed-Loop Spike Agent — DEATH MATCH

**Date:** 2026-08-14  
**Status:** **GREEN — DUAL OWNS**  
**Squad:** THE BEASTIE BOYZ / Blam  
**EXTERNAL-clean.**

---

## Setup

Two sensory channels (LEFT / RIGHT):

| Channel | Content | Classical sees | Dual sees |
|---------|---------|----------------|-----------|
| Goal beacon | Phase-burst structure | moderate rate | **strong σ_Δ** |
| Distractor | Scrambled high-rate noise | **loud rate** | σ ≈ 0 |

Classical residual: step toward higher rate → **chases distractor**.  
Dual residual: step toward |σ_Δ| OWNED channel → **follows structure**.

---

## Results (40 episodes)

| Mode | Goals | mean progress | mean wrong | mean cost |
|------|------:|--------------:|-----------:|----------:|
| classical | **0/40** | 0.00 | **20.00** | 180.00 |
| dual | **40/40** | 12.00 | **0.00** | **12.11** |

Sanity: goal rate≈92 sig=+0.27 · distractor rate≈164 sig≈0.00

---

## Sample

```
classical structure=RIGHT: progress=0/12  wrong=20  cost=180
dual      structure=RIGHT: progress=12/12 wrong=0   cost=12.2
```

---

## Non-claims

Synthetic beacons. Not real neuromorphic control. Not a claim that dual solves all POMDPs.

---

## Reproduce

```bash
cd primitive/dual_evidence
python3 demo_spike_agent.py
# VERDICT: DUAL OWNS
```

---

**Sensitivity is the weapon. Classical chases noise. Dual owns the beacon.**
