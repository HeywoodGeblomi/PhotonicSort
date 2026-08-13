# Secondary Parity Dual-Evidence — Multi-Arch Money-Shot

**Status:** Code on main (PR #126). Thin CI workflow lands here so aarch64 fires automatically.

## On main

1. `residual/secondary_parity.hpp` — σ / σ_Δ + dual_confirm (EXTERNAL-clean)
2. `residual/hybrid_residual_menu.hpp` — flag-gated dual-evidence at HE→ska (`-DSECONDARY_PARITY`)
3. `.github/workflows/sp-multi-arch.yml` — matrix: ubuntu-24.04 + ubuntu-24.04-arm × classical + secondary_parity
4. Driver: existing `scripts/expanded_field_bench_hybrid.cpp` built with/without `-DSECONDARY_PARITY`

## Success criterion (SP gate)

On **both** ISAs under `secondary_parity`:

- soft@1.20 = 0, major (>1.5×) = 0
- all ok=1
- i32 / u32 / i64

x86 multi-type soft=0 already measured locally (2026-08-13). This workflow unlocks the aarch64 money-shot via GitHub Actions ARM runners.

## Non-claims

- Not a completed formal Field-Level Breakthrough until multi-arch numbers are GREEN and CI/stat-sig packaging is complete.
- EXTERNAL-clean only. No internal irreversible state.

**THE BEASTIE BOYZ**
