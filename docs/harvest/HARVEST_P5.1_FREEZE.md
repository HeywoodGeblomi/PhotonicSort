# Harvest P5.1 / v1.5.1-c — Pure Residual Freeze

**Status:** FROZEN 2026-08-11  
**Codename:** Harvest P5.1  
**Version tag candidate:** `v1.5.1-c`  
**Supersedes:** Harvest P5 / v1.5.0-c

## Why P5.1

Residual menu changed: Phase 4 added a live **3-run merge** residual for push-middle / push-front. Locked geo recovered; full extended geo improved. Clean freeze is clearer than additive notes on P5.

## Locked numbers (n=1e6, median-of-7)

| Group | geo pure/pdq | Gate |
|-------|-------------:|:-----|
| Full extended (38) | **0.500×** | ≤0.65 MET |
| High priority (13) | **0.571×** | ≤0.75 MET |
| Locked original 11 | **0.553×** | all patterns < 1.0× |
| Max high-priority pattern | **1.102×** (gaussian) | documented HE floor |

### Key pattern ratios

| Pattern | pure/pdq |
|---------|---------:|
| push_middle_100 | **0.354×** |
| push_middle_1000 | **0.287×** |
| push_front_100 | **0.223×** |
| push_front_1000 | **0.198×** |
| zipf_1.2 / 1.5 / 2.0 | 0.31–0.65× |
| constant_0 / 42 | 0.85–0.88× |
| gaussian | 1.102× |

## Residual menu (frozen order)

1. STRUCTURE (sorted / reverse)
2. Stratified constant probe
3. Expanded counting (sample-first; moderate card + compact range) — Zipf kill
4. Majority / Dutch-flag (equal-rate ≥ 0.35)
5. **3-run merge** (push-middle / push-front) — sort island, merge prefix|island|suffix
6. Sparse MSD
7. Identity-almost
8. HE MSD

## Changes vs P5 (v1.5.0-c)

| Item | P5 | P5.1 |
|------|----|------|
| push_middle residual | heuristic (open 1.49–1.57×) | **3-run merge CLOSED 0.29–0.35×** |
| Full extended geo | 0.558× | **0.500×** |
| Locked geo | 0.568× | **0.553×** |
| gaussian | 1.100× | 1.102× (same floor) |

## Explicit open item

* **gaussian ≈ 1.10×** — HE residual quality floor on near-uniform continuous data. Accepted.

## Non-claims

See `NON_CLAIMS_P5.1.md`. Not field-level. EXTERNAL-clean. Pure residual only. Independent reproduction required before stronger claims.

**THE BEASTIE BOYZ**
