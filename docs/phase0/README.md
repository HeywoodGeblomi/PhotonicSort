# Phase 0 Baseline — Harvest P5.1 / v1.5.1-c

**Locked 2026-08-11.** All subsequent claims must cite this baseline.

## Contents

| File | Purpose |
|------|---------|
| `PHASE0_BASELINE_LOCK.md` | Authoritative freeze record |
| `RED_GATE_STATUS.md` | Floor ledger + regression rule |
| `SHA256SUMS.txt` | Source integrity hashes |
| `pure_residual_menu_v1.5.1-c.hpp` + residual_*.hpp | Pure residual menu |
| `generators_extended.py` | 38-pattern suite |
| `p5_extended_harness.cpp` | Measurement harness source |
| `P5_1_FINAL.csv` | Locked numbers (n=1e6, median-of-7) |
| `Dockerfile` | One-command rebuild skeleton |

## Quick verify

```bash
sha256sum -c SHA256SUMS.txt
```

## Docker skeleton

```bash
docker build -t photonic-p5.1-baseline .
docker run --rm photonic-p5.1-baseline
```

Wall-clock times are host-dependent. Ratios vs pdqsort/ska on the same host are the meaningful comparison. Absolute milliseconds from the original measurement machine are not portable.

## Public claim (unchanged)

Not a field-level breakthrough beyond the published suite.

**THE BEASTIE BOYZ**
