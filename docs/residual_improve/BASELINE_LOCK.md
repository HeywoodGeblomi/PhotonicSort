# Residual-Improvement Baseline Lock — pure residual v2.4.1

**Status:** LOCKED 2026-08-11  
**Codename:** Post-Residual-Improvement Baseline  
**Menu version:** pure residual v2.4.1 (FEW_WIDE + try_reverse_runs)  
**Main tree SHA at lock:** `040667bb0c28f265aabc7d0537451f0c9b5da122`  
**Historical reference unchanged:** Phase 0 / Harvest P5.1 / v1.5.1-c (`docs/phase0/`)

## Purpose

Freeze the pure residual menu and measured residual floors after the residual-improvement phase (Attack 3 floors + FEW_WIDE v2.3/v2.4.1 + equal_heavy correctness hotfix).

This is an **engineering baseline**. It does **not** replace or rewrite the Phase 0 historical freeze.

Gaussian HE phase (2026-08-11) closed with the floor documented as a residual quality / path limit — see [`GAUSSIAN_HE_PHASE_CLOSE.md`](./GAUSSIAN_HE_PHASE_CLOSE.md).

## What is frozen

| Artifact | Role | Blob SHA |
|----------|------|----------|
| `residual/pure_residual_menu.hpp` | Unified pure residual entry | `61cc5c6c1c18a219e966c388daf94d500b18c637` |
| `residual/residual_few_wide_i64.hpp` | FEW_WIDE v2.4.1 (HCAP=128, k=2 dual-count, equal_heavy verify) | `2cca4a9881ae7eda2c10f42da2d45434838b9250` |
| `residual/residual_he_msd_i64.hpp` | HE MSD residual | `809d8097bef3b4b1528850f6d04de536ce563f86` |
| `residual/residual_sparse_i64.hpp` | Sparse residual | `cc0928b088443833630b2734a4e7cf48f880dcc7` |
| `residual/residual_adversarial_i64.hpp` | Majority / Dutch-flag residual | `aa22c55bf5aaeb1402d34fded2aab5d339cf0405` |
| `docs/residual_improve/ATTACK_3_FLOORS_RESULT.md` | Attack floor ledger | `61d5b3883a6dd0f9245c009e4c7fc2e917b3a588` |
| `docs/residual_improve/POST_MERGE_SUITE_RESULT.md` | Post-merge 16-pattern scoreboard | `0bf8a79eab629e60b99876ba9a9747a6a72593ec` |
| `docs/residual_improve/GAUSSIAN_HE_PHASE_CLOSE.md` | Gaussian HE phase close | on main |

## Residual menu order (v2.4.1)

1. Stratified constant probe  
2. Early FEW_WIDE (`should_try_few_wide` + residual)  
3. STRUCTURE (sorted / reverse)  
4. `try_reverse_runs` (descending-run reverse + alternating-buffer pairwise merge)  
5. FEW_WIDE  
6. Expanded counting  
7. Majority / adversarial (equal-rate ≥ 0.35)  
8. `try_push_middle` (3-run merge)  
9. Sparse MSD  
10. Identity-almost  
11. HE MSD  

Key techniques landed in residual-improvement:

- FEW_WIDE v2.3: dual branchless count + sequential fill for k=2  
- FEW_WIDE v2.4: HCAP 64 → 128  
- FEW_WIDE v2.4.1: full O(n) verify on sample-all-equal path (equal_heavy correctness)  
- `try_reverse_runs`: alternating source/dest buffers for overlapping reverse segments  

## Measured surface (n=1e6, vs pdqsort)

### Post-merge 16-pattern scoreboard

(from `POST_MERGE_SUITE_RESULT.md`, median-of-7, commit f9e1432 / PR #38)

| Pattern | menu/pdq | ok |
|---------|---------:|:--:|
| sorted | 0.844× | 1 |
| reverse | 0.219× | 1 |
| organpipe | 0.535× | 1 |
| sawtooth | 0.422× | 1 |
| almost_sorted | 0.515× | 1 |
| uniform_i64 | 0.908× | 1 |
| gauss_nearflat | 1.195× | 1 |
| two_values_50_50 | **0.227×** | 1 |
| few_k4_wide | **0.633×** | 1 |
| few_k16_uniform | 1.208× | 1 |
| reverse_segments_8 | **0.772×** | 1 |
| few_unique_k8 | 0.706× | 1 |
| zipf_k16 | 1.068× | 1 |
| push_middle | 0.521× | 1 |
| equal_heavy | 1.457× | 1 |
| adversarial_pipe | 0.539× | 1 |

**Geo-mean (16 patterns): 0.648×** pure/pdq. All ok=1.

### Attack floors ledger

| Floor | Result | Status |
|-------|--------|--------|
| two_values / k=2 | 0.42–0.47× (focused); 0.227× post-merge | **CLOSED** |
| reverse_segments_8 | 0.85–0.93× (focused); 0.772× post-merge | **CLOSED** |
| few_unique_heavy_k4 | ~0.8–0.9× / 0.633× | held |
| few_unique_k16 (uniform) | ~1.0–1.2× | residual quality limit |
| Gaussian HE (gauss_nearflat) | ~1.1–1.2× full-menu (isolated MSD competitive) | **residual quality / path limit** |

Indicative full-extended 38-pattern surface after residual-improvement (phase notes / project memory; no dedicated locked CSV in this freeze): **~0.55–0.67×** pure/pdq. Locked + high groups correspondingly ~0.58–0.75× range. A future full 38-pattern re-lock may be appended without invalidating this baseline.

## Explicit residual limits

- **few_unique_k16** (balanced uniform / high-skew Zipf k≈8–16): residual quality of hash-rank+count vs pdq equals-friendly partition. Path fires correctly; not a routing miss. Documented limit.
- **Gaussian HE**: full-menu ratio on continuous near-uniform remains ~1.1–1.2×. Isolated classical MSD residual is competitive with pdq; the gap is probe / path overhead under the pure residual contract. Prior pure MSD micros exhausted. Documented residual quality / path limit (Gaussian HE phase closed 2026-08-11). See [`GAUSSIAN_HE_PHASE_CLOSE.md`](./GAUSSIAN_HE_PHASE_CLOSE.md).
- equal_heavy shows variance (majority residual); sample-all-equal correctness path fixed in v2.4.1.

## Rules held

- EXTERNAL-clean (visible probe metrics only; no χ / internal irreversible state)
- Pure residual path only (no library pdq/ska dispatch on the pure menu)
- Worst-case O(n log n)
- Honesty primary
- **Not a field-level breakthrough**
- Phase 0 / Harvest P5.1 remains the historical frozen reference
- Controlled suite only

## Non-claims

See root [`NON_CLAIMS.md`](../../NON_CLAIMS.md) and prior phase non-claims. This freeze adds no new performance claim beyond the residual floors ledger and the published 16-pattern surface. Gaussian HE and moderate-k few-unique remain documented residual quality limits.

## Citation

Any subsequent residual work that builds on this menu should reference:

```
Residual-Improvement Baseline Lock — pure residual v2.4.1 (2026-08-11)
docs/residual_improve/BASELINE_LOCK.md
```

Phase 0 pointer (unchanged historical reference):

```
Phase 0 Baseline Lock — Harvest P5.1 / v1.5.1-c (2026-08-11)
docs/phase0/
```

**THE BEASTIE BOYZ**
