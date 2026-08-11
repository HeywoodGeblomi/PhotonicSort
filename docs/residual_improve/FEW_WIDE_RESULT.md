# FEW_WIDE Residual — Residual-Improvement Phase Result

**Date:** 2026-08-11  
**Baseline residual:** Harvest P5.1 / v1.5.1-c (Phase 0 frozen)  
**Change:** New pure residual `residual_few_wide_i64` + trigger before classic counting  
**Contract:** Pure residual only, EXTERNAL-clean, Phase 0 reference unchanged for comparison  

---

## Critical floor — CLOSED

| Case | Phase 1 floor | After FEW_WIDE | Status |
|------|--------------:|---------------:|--------|
| few_unique_heavy_k4 (wide) | **5.694×** | **0.848×** | **CLOSED** |
| few_k4 α=1.2 wide | ~5.5× | 0.833× | CLOSED |
| few_k4 α=2.0 wide | — | 0.685× | win |
| few_k2 / k8 wide | — | 0.59–0.90× | win |
| few_k16 wide | — | 1.008× | parity |
| few_k4 compact | already win | 0.124× | held |

---

## Full extended suite (n=1e6, median-of-7)

| Metric | Phase 0 freeze | After FEW_WIDE |
|--------|---------------:|---------------:|
| Full extended geo pure/pdq | **0.500×** | **0.504×** |
| Locked (11) geo | 0.553× | **0.500×** (locked-only run) |
| High-priority geo | 0.571× | 0.541× |

STRUCTURE / almost / Class-1 / Zipf / push-middle paths held. Correctness ok=1 on all patterns.

---

## Residual design (locked)

- **Trigger:** sample unique ≤ 16 ∧ wide range (srange ≥ 2^20 or ≥ 3n/4)  
- **Algorithm:** collect ≤16 uniques → sort → rank map → count → emit original values in rank order  
- **Fallback:** k overflow → existing residual (majority / HE)  
- **Menu position:** before classic counting  

Files:
- `residual/residual_few_wide_i64.hpp`
- `residual/pure_residual_menu.hpp` (wired)

---

## Non-claims

- Not a field-level breakthrough beyond the published suite.  
- Phase 0 baseline remains the frozen reference.  
- FEW_WIDE closes the documented low-cardinality + wide-range floor; it does not claim domination of all residual soft spots (gaussian ~1.1–1.3× still documented).

**THE BEASTIE BOYZ**
