# Neuromorphic Spike-Native Dual-Evidence — WIDE OPEN

**Date:** 2026-08-14  
**Status:** **GREEN**  
**Domain:** asynchronous neural / event-camera spike trains  
**Primitive:** `primitive/dual_evidence/`  
**EXTERNAL-clean. THE BEASTIE BOYZ / Blam**

---

## One line

Spikes **are** the threshold crossings. Secondary Parity runs on the event list directly — no frame grid, no clock — and dual-evidence residual commitment still separates phase pairs that classical rate/ISI cannot.

---

## Why this is the gangster domain

| SP mechanism | Neuromorphic native |
|--------------|---------------------|
| Threshold crossings | Spikes (hardware language) |
| Continuous probe optional | Optional; events alone suffice |
| Odd-event restriction | Odd-indexed spikes |
| σ_Δ ordinal contrast | Amplitude (or ISI) contrast on odd spikes |
| Second solid before residual | Decode / motor only when OWNED |

Event cameras and silicon neurons never had a frame. σ_Δ does not need one.

---

## Protocol

| Item | Detail |
|------|--------|
| Background | Shared Poisson (identical both phases) |
| Structure | Burst packets; phase shifts centers by half inter-burst gap |
| Classical | Rate + ISI CV |
| Second solid | Polarity-directed σ_Δ on odd-spike amplitudes |
| Residual | CHEAP (cost 1·n) · FULL (cost 8·n) |

A2 pulse-train argument applies: burst packets = pulses; half-gap shift flips odd-event ordinal class.

---

## Results

| bursts | classical_match | sigma_flip | dual_differs | notes |
|-------:|:---------------:|:----------:|:------------:|-------|
| 12 | 35/40 | 34/40 | 23/40 | strong |
| 16 | 35/40 | 29/40 | 17/40 | strong |
| 24 | 39/40 | 32/40 | 18/40 | strong |

Sample (bursts=16 seed=0):

```
spikes        = 229 / 229          # exact count match
rates         = 229 / 229 Hz
classical     = (FULL, FULL)
dual          = (CHEAP, CHEAP)     # second solid fails both → abate
σ_Δ           = +0.008 / −0.141    # polarity flip
μ_dual/μ_class = 0.125
```

Sample with route split (bursts=8 seed=1):

```
classical = (FULL, FULL)
dual      = (CHEAP, FULL)          # polarity-directed split
σ_Δ       = −0.129 / +0.221
```

---

## What this establishes

| Claim | Verdict |
|-------|---------|
| σ_Δ is spike-native (no continuous stream required) | **YES** |
| Classical rate/CV match on phase pairs | **YES** (~90%) |
| σ_Δ polarity flips under burst phase shift | **YES** (~75–85%) |
| Dual can split residual routes | **YES** |
| Minimax residual cost can improve | **YES** |

---

## Non-claims

1. Not a trained decoder on real DVS / OpenSpike / Allen Institute traces — synthetic existence proof.
2. Cost model synthetic (1 vs 8).
3. Not a claim that neuromorphic chips should ship this tomorrow.
4. EXTERNAL-clean. No χ.

---

## Reproduce

```bash
cd primitive/dual_evidence
python3 dual_evidence.py
python3 demo_spike_neuro.py
# SELF-CHECK GREEN
```

---

**THE BEASTIE BOYZ / Blam — broken wide open.**
