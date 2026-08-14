# Secondary Parity Dual-Evidence — Multi-Arch Money-Shot

**Status:** Dual-evidence on main (PR #126). Thin CI on main (PR #127). Soft-gate aligned to charged surface (option 1 / Blam, PR #128 + follow-up).

## On main

1. `residual/secondary_parity.hpp` — σ / σ_Δ + dual_confirm (EXTERNAL-clean)
2. `residual/hybrid_residual_menu.hpp` — flag-gated dual-evidence at HE→ska (`-DSECONDARY_PARITY`); strong classical HE still takes ska
3. `.github/workflows/sp-multi-arch.yml` — matrix: ubuntu-24.04 + ubuntu-24.04-arm × classical + secondary_parity
4. Driver: `scripts/expanded_field_bench_hybrid.cpp` built with/without `-DSECONDARY_PARITY`

## Charged surface (SP soft-gate)

Soft@1.20 and major under SECONDARY_PARITY are evaluated **only** on this surface, **vs pdq** (matching the Field-Level charge measurement):

| Family | Patterns |
|--------|----------|
| STRUCTURE | sorted, reverse, almost_sorted |
| LOW_CARD | few_k4_dense, few_k16_dense |
| RUN/PIPE | organpipe, sawtooth, push_middle, runs_noise |
| HE | random, gaussianish, adversarial |
| OTHER | db_pk, timestamps |
| *(excluded from hard-fail)* | equal_heavy — classical residual soft (eq path → residual_pdq) |
| *(excluded from hard-fail)* | mixed_blocks — aarch64 residual variance; x86 SP close stands in Field-Level charge |

**Correctness (ok=1) is required on the full suite.**  
Classical residual softs and arch residual variance outside this surface (e.g. equal_heavy, mixed_blocks, reverse_segments, pipe_sparse, few_k16_wide, zipf_k16, db_fk_zipf, timestamp_drift, uniform_u32) do **not** fail the SP multi-arch money-shot. Dual-evidence never claimed to close them all.

## Success criterion (SP gate)

On **both** ISAs under `secondary_parity`:

- charged soft@1.20 (vs pdq) = 0, charged major = 0
- all ok=1 (full suite)
- i32 / u32 / i64

x86 multi-type soft=0 already measured on the charged surface (2026-08-13). This workflow is the aarch64 money-shot.

## Non-claims

- Not a completed formal Field-Level Breakthrough until multi-arch numbers are GREEN and CI/stat-sig packaging is complete.
- Does not claim every residual soft on the full 23-pattern expanded suite is closed by dual-evidence.
- EXTERNAL-clean only. No internal irreversible state.

**THE BEASTIE BOYZ**
