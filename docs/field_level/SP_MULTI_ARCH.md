# Secondary Parity Dual-Evidence — Multi-Arch Money-Shot

**PR purpose:** Land Secondary Parity dual-evidence residual selection + CI matrix on x86_64 and aarch64 so the multi-arch Field-Level gate can be measured.

## What lands

1. `residual/secondary_parity.hpp` — σ / σ_Δ computation + dual_confirm (EXTERNAL-clean)
2. `residual/hybrid_residual_menu.hpp` — flag-gated dual-evidence at HE→ska (`-DSECONDARY_PARITY`)
3. `secondary_parity/suite_multitype.cpp` — multi-type (i32/u32/i64) suite driver vs pdqsort
4. `.github/workflows/sp-multi-arch.yml` — matrix: ubuntu-24.04 + ubuntu-24.04-arm × classical + secondary_parity

## Success criterion

On **both** ISAs under `secondary_parity` gate:

- soft@1.15 = 0, soft@1.20 = 0, major (>1.5×) = 0
- all ok=1
- i32 / u32 / i64

x86 multi-type soft=0 already measured locally (2026-08-13). This PR unlocks the aarch64 money-shot via GitHub Actions ARM runners.

## Non-claims

- Not a completed formal Field-Level Breakthrough until multi-arch numbers are GREEN and CI/stat-sig packaging is complete.
- EXTERNAL-clean only. No internal irreversible state.

**THE BEASTIE BOYZ**
