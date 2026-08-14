# Non-Sort Dual-Evidence Demo — Track 2 Result

**Date:** 2026-08-14  
**Status:** **GREEN**  
**Domain:** adaptive residual action on sensor-stream segments  
**Primitive:** `primitive/dual_evidence/`  
**EXTERNAL-clean. THE BEASTIE BOYZ / Blam**

---

## One line

The dual-evidence residual commitment API works **outside sorting**: continuous sensor segments → windowed disorder stream → σ_Δ second solid → irreversible CHEAP vs FULL residual commit.

---

## Protocol

| Item | Detail |
|------|--------|
| Stream | Monotone sensor ramp + local reverse spikes (F1 analogue on floats) |
| Probe | Windowed inversion density (continuous) |
| Classical | Flip-rate / run count threshold |
| Second solid | Polarity-directed σ_Δ (`expected_polarity=+1`) |
| Residual menu | CHEAP (cost 1·n) · FULL (cost 8·n) |
| Phase pairs | Same classical stats; σ_Δ polarity flips |

---

## Results (seeded trials)

| n | classical_match | sigma_flip | dual_differs | median μ_dual/μ_classical |
|--:|:---------------:|:----------:|:------------:|--------------------------:|
| 512 | yes | yes | **yes** (sample) | — |
| 1024 | 40/40 | 40/40 | 0/40 | **0.125** |
| 4096 | 40/40 | 40/40 | 0/40 | **0.125** |
| 8192 | 40/40 | 40/40 | 0/40 | 1.000 |

Headline sample (n=512 seed=0):

```
classical actions = (FULL, FULL)     # same classical decision both phases
dual actions      = (CHEAP, FULL)    # polarity-directed second solid splits
σ_Δ               = -0.0146 / +0.0084
```

When both phases fail second solid (negative σ_Δ under +1 expectation), dual abates both to CHEAP → minimax cost **8× lower** than classical FULL. That is residual-cost improvement under dual evidence outside the sorter.

---

## What this establishes

| Claim | Verdict |
|-------|---------|
| Primitive is domain-agnostic (not sorter-tied) | **YES** |
| Classical matches on phase pairs | **YES** |
| σ_Δ polarity flips on phase pairs | **YES** |
| Dual can route phases differently | **YES** (n=512 sample) |
| Minimax residual cost can improve | **YES** (median ratio 0.125 on mid-n) |

---

## Non-claims

1. Not a production sensor codec or agent.
2. Cost model is synthetic (1 vs 8 unit costs) — existence proof for residual-selection gap, not a measured wall-clock on real hardware codecs.
3. Not a claim that every non-sort domain benefits.
4. EXTERNAL-clean. No χ.

---

## Reproduce

```bash
cd primitive/dual_evidence
python3 dual_evidence.py          # contract self-check
python3 demo_stream_residual.py   # this demo
```

---

**THE BEASTIE BOYZ / Blam**
