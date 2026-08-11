# Phase 1 Re-measure against FEW_WIDE (v2)

**Date:** 2026-08-11  
**Menu:** pure residual + FEW_WIDE v2 (hashed collect/rank)  
**Baseline:** Phase 0 frozen reference; Phase 1 ledgers pre-FEW_WIDE  

## Critical few-unique surface (n=1e6, median-of-11, vs pdqsort)

| Case | Pre-FEW_WIDE | After v2 | Status |
|------|-------------:|---------:|--------|
| few_unique_heavy_k4 (α=1.5) | **5.694×** | **0.53–0.98×** | **CLOSED** |
| few_k4 α=1.2 wide | ~5.5× | 1.10× | near-parity |
| few_k4 α=2.0 wide | — | 0.70× | win |
| few_k2 α=1.5 wide | — | 1.43× | residual floor |
| few_k8 α=1.5 wide | — | 1.34× | residual floor |
| few_k16 α=1.5 wide | — | 0.91× | win |
| few_k4 compact | already win | 0.26× | held |
| few_k32 (no FEW_WIDE) | — | 3.13× | expected (k>16) |
| two_values_split | 1.431× | **1.63×** | still floor |

## Other Phase 1 floors (vs pdqsort, indicative)

| Case | Pre | After | Notes |
|------|----:|------:|-------|
| gauss_skew_strong_pos | 1.479× | ~1.1–1.4× | HE path; FEW_WIDE not selected |
| reverse_segments_8 | 2.565× | ~1.8–2.9× | still floor (structured reverse) |
| inv_density_05 | 1.851× | ~1.0–1.2× | improved / near-parity |
| range_power_bits_24 | 1.102× | 0.72× | closed |

## FEW_WIDE v2 change
- Open-addressed hash collect + rank (O(n) expected) replaces O(n·k) linear scans
- Direct residual ~4–7 ms on k=4 wide (was ~16–60 ms v1)
- Trigger unchanged: sample unique ≤16 ∧ wide range

## New residual floors introduced by FEW_WIDE?
- **No major new floors** on STRUCTURE / sorted / equal / compact counting paths
- `two_values_split` and some k=2/8 wide cases remain above 1.0× vs pdqsort (pdq’s 2-value partition is extremely strong)
- `few_k32` correctly bypasses FEW_WIDE and shows the pre-existing HE lag

## Recommendation
1. Ship FEW_WIDE v2 to main (hash optimization)
2. Accept critical k=4 wide floor as closed
3. Next residual target: `two_values_split` / k=2 specialized path (or majority tuning), then `reverse_segments_8`

Phase 0 baseline remains frozen reference. EXTERNAL-clean. Not field-level.

**THE BEASTIE BOYZ**
