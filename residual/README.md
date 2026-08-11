# Pure residual sources (One Single Harvest)

EXTERNAL-clean pure residual menu — **no** library dispatch to pdqsort / ska_sort / std::sort.

| File | Role | Gate |
|------|------|------|
| `pure_residual_menu.hpp` | Unified entry (STRUCTURE → Majority → Low-card → Sparse → Identity-almost → HE) | geo 0.538× pdq |
| `residual_he_msd_i64.hpp` | Classical MSD INS=96 BLOCK=512 | HE ≤0.92× ska MET |
| `residual_sparse_i64.hpp` | Pure MSD INS=64 BLOCK=256 (replaces std::sort sparse path) | db_pk ≤0.95× MET |
| `residual_adversarial_i64.hpp` | Dutch-flag majority + pure sides | adversarial ≤0.95× MET |
| `msd_radix_i64.hpp` | Prior freeze residual (historical) | — |

See [docs/harvest/](../docs/harvest/) for full suite numbers and non-claims.

**THE BEASTIE BOYZ** · 2026-08-11
