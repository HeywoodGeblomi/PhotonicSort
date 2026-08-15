# FULL Bind B3+B4 — Residual Core + pybind11

**Status:** **GREEN**  
**Date:** 2026-08-14  
**Squad:** THE BEASTIE BOYZ / Blam  
**EXTERNAL-clean.**

---

## B3 — FULL residual path

`residual_full_path.hpp` — when residual commits FULL, evaluate probe stream with dual-evidence / SP:

| Talent | When |
|--------|------|
| CHEAP | not OWNED |
| FULL_STRUCTURE | OWNED · \|σ_Δ\| > 0.1 |
| FULL_HE | OWNED · high classical score |
| FULL_GENERIC | OWNED · otherwise |

## B4 — pybind11 module `dual_evidence_cpp`

Exports: `sigma_amps`, `compute`, `second_solid`, `classical_choose`, `dual_choose`, `residual_decide`, enums `Action`, `ResidualTalent`.

## Build

```bash
cd primitive/dual_evidence
make all && make test
```

## Receipts

| Check | Result |
|-------|--------|
| pybind vs ctypes dual_choose | **BIT-IDENTICAL** |
| residual_decide | owned/talent/sigma/score |
| make test | **GREEN** |

## Bind ladder

B0→B1→B2→**B3**→**B4** all GREEN.

**One second solid. Python ↔ C++ ↔ residual core. FULL bind.**
