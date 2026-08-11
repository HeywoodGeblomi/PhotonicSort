# Phase 0 — Baseline Lock

**Status:** LOCKED 2026-08-11  
**Codename:** Harvest P5.1 / v1.5.1-c  
**North-Star plan:** Attack plan for broader field-level assertions (honesty-first)

## Purpose

This document freezes the exact pure-residual adaptive int64 sorter, the 38-pattern suite, and the measured numbers that produced the public claim. Every future result that claims improvement or broader applicability must be measured against this baseline and must cite it.

## What is frozen

| Artifact | Role | SHA-256 (see SHA256SUMS.txt) |
|----------|------|------------------------------|
| `pure_residual_menu_v1.5.1-c.hpp` | Unified pure residual entry | 29c720f1… |
| `residual_he_msd_i64.hpp` | Classical MSD HE residual | 67919317… |
| `residual_sparse_i64.hpp` | Sparse MSD residual | b9ae27de… |
| `residual_adversarial_i64.hpp` | Dutch-flag majority residual | dea8de5d… |
| `generators_extended.py` | 38-pattern generator suite | 47da9280… |
| `p5_extended_harness.cpp` | Measurement harness source | f8c4832e… |
| `P5_1_FINAL.csv` | Locked numbers (n=1e6, median-of-7) | 8f29cfaa… |
| `HARVEST_P5.1_FREEZE.md` | Freeze record | 9de74e22… |
| `NON_CLAIMS_P5.1.md` | Mandatory non-claims | f8a7c7dd… |

Full hashes: `SHA256SUMS.txt` in this directory.

## Locked performance (n=1e6, median-of-7)

| Group | geo pure/pdq | Gate |
|-------|-------------:|:-----|
| Full extended (38) | **0.500×** | ≤0.65 MET |
| High priority (13) | **0.571×** | ≤0.75 MET |
| Locked original 11 | **0.553×** | all patterns < 1.0× except equal_heavy 1.027× |

**Documented residual floors:** gaussian **1.102×**, equal_heavy **1.027×**  
See `RED_GATE_STATUS.md`.

## Residual menu (frozen order)

1. STRUCTURE (sorted / reverse)
2. Stratified constant probe
3. Expanded counting (sample-first; moderate card + compact range)
4. Majority / Dutch-flag (equal-rate ≥ 0.35)
5. 3-run merge (push-middle / push-front)
6. Sparse MSD
7. Identity-almost
8. HE MSD

## Rules held

- EXTERNAL-clean (visible probe metrics only)
- Pure residual path only (no library dispatch)
- Worst-case \(O(n \log n)\)
- Honesty primary
- **Not a field-level breakthrough beyond the published suite**

## Phase 0 deliverables checklist

- [x] Archive residual sources + generators + harness source
- [x] SHA-256 manifest
- [x] Red-gate / floor-ledger status
- [x] Baseline lock document (this file)
- [x] One-command Docker or Nix reproduction skeleton
- [x] Public push of Phase 0 package to PhotonicSort

## Citation

Any claim that builds on or supersedes Harvest P5.1 must reference:

```
Phase 0 Baseline Lock — Harvest P5.1 / v1.5.1-c (2026-08-11)
docs/phase0/
```

**THE BEASTIE BOYZ**
