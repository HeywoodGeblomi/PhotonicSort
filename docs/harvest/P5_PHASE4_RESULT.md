# P5 Phase 4 — Localized Disorder Residual

**Status:** Phase 4 COMPLETE  
**Date:** 2026-08-11  
**Baseline:** Harvest P5 / v1.5.0-c freeze

## Target results

| Pattern | P5 freeze | Post-P4 | Status |
|---------|----------:|--------:|:------:|
| push_middle_100 | 1.568× | **0.354×** | **CLOSED** |
| push_middle_1000 | 1.491× | **0.287×** | **CLOSED** |
| push_front_100 | ~0.81× | **0.223×** | improved |
| push_front_1000 | ~0.80× | **0.198×** | improved |
| gaussian | 1.100× | 1.102× | residual floor (unchanged) |

## Method

`try_push_middle` rewritten as a **3-run merge residual**:

1. Detect long sorted prefix + long sorted suffix + small disordered island.
2. Sort the island (insertion if ≤64, else HE MSD).
3. 3-way merge (prefix | sorted-island | suffix) into a temp buffer, copy back.

Required because push_middle inserts the largest *k* keys into the middle of a sorted body — sorting the island in place leaves values globally out of order. Merge cost is O(n + k log k).

## Suite impact (n=1e6, median-of-7)

| Group | P5 freeze | Post-P4 |
|-------|----------:|--------:|
| Full extended geo pure/pdq | 0.558× | **0.500×** |
| Locked geo | 0.568× | **0.553×** |
| High-priority geo | 0.497× | 0.571× |

## Non-claims unchanged

Not field-level. EXTERNAL-clean. Pure residual only. Sequential int64.

**THE BEASTIE BOYZ**
