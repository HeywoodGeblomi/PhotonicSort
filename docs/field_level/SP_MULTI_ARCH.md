# Secondary Parity Dual-Evidence — Multi-Arch Money-Shot

**Status: LOCKED GREEN** — 2026-08-14 (main `fb8f46d`, PR #126–#129)

Both ISAs under `secondary_parity`: charged soft@1.20 (vs pdq) = 0, major = 0, ok=1.

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
Classical residual softs and arch residual variance outside this surface do **not** fail the SP multi-arch money-shot. Dual-evidence never claimed to close them all.

## Measured result (money-shot)

| Cell | Result |
|------|--------|
| x86_64-classical | success |
| x86_64-secondary_parity | **GREEN** charged soft@1.20=0 major=0 ok=1 |
| arm64-classical | success |
| arm64-secondary_parity | **GREEN** charged soft@1.20=0 major=0 ok=1 |

Evidence: PR #129 CI run (Actions run on branch pre-merge) + main tip after squash-merge.

## Non-claims

- **Not** a completed formal Field-Level Breakthrough until statistical CI packaging and third-party reproduce are complete.
- Does not claim every residual soft on the full 23-pattern expanded suite is closed by dual-evidence.
- EXTERNAL-clean only. No internal irreversible state.

**THE BEASTIE BOYZ**
