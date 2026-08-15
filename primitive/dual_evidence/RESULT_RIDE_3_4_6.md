# Tracks 3 · 4 · 6 — Threshold Tune · Broader Apps · Claim Surface

**Status:** **GREEN**  
**Date:** 2026-08-14  
**Squad:** THE BEASTIE BOYZ / Blam  
**EXTERNAL-clean.**

---

## Track 3 — Threshold tune

Sweep `classical_threshold × sigma_floor` against synthetic field-like patterns.

| Metric | Locked value |
|--------|-------------:|
| **classical_threshold** | **0.04** |
| **sigma_floor** | **0.005** |
| Best purity | **0.940** |

Per-pattern at best (30 seeds):

| Pattern | Desired-talent hit rate |
|---------|------------------------:|
| structured | 0.97 |
| high_entropy | 0.73 |
| cheap | 1.00 |
| almost_sorted | 1.00 |
| adversarial | 1.00 |

Defaults in `dual_residual.residual_decide` updated to match.

```bash
python3 tune_thresholds.py
```

---

## Track 4 — Broader app (spike + talent)

`demo_spike_agent_talent.py` — dual_choose on structured vs scrambled channels + residual_decide annotation.

| Check | Result |
|-------|--------|
| dual picks structure side | **38/40** |

---

## Track 6 — Claim surface

| Claim | Status |
|-------|--------|
| Field-Level charged soft@1.20 vs pdq | **UNCHANGED** (not re-run; dual residual advisory only) |
| Multi-arch money-shot | **UNCHANGED** |
| Sort path / residual menu production drive | **Not forced** through dual_residual yet |

This plate hardens the **Python dual residual API**, not the C++ charged soft-gate. Geo-mean vs pdq is not claimed improved by this PR.

---

## Reproduce

```bash
cd primitive/dual_evidence
make all && make test
python3 tune_thresholds.py
python3 demo_spike_agent_talent.py
```

**Ride complete for 3·4·6 scope.**
