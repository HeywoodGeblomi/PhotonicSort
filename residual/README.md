# Residual sources (pure path)

## `msd_radix_i64.hpp`

Frozen pure HE residual (2026-08-10):

- Classical MSD, fixed 8-bit digits, flat counts
- Blocked scatter (BLOCK=256) + write-side prefetch
- INS=192
- majority_v2 + sparse-cluster entry routing
- EXTERNAL-clean; no library pdq/ska calls on the HE path

Namespace: `he_residual`. Entry: `he_residual::msd_radix_i64(int64_t*, size_t)`.

See [`docs/phase3/`](../docs/phase3/) for measured baselines and non-claims.
