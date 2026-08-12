# A3 Multi-Type — float64 + generic comparator

**Date:** 2026-08-12  
**Status:** COMPLETE (x86_64 measured)  
**Not field-level** until multi-arch claim surface holds (see MULTI_ARCH.md).

## Results (float64 claim surface)

| n | geo | max | soft | Gate |
|--:|----:|----:|-----:|:----:|
| 1e6 | **0.65×** | 1.07× | 0 | **PASS** |
| 1e7 | **0.82×** | 0.98× | 0 | **PASS** |

Integer claim surface remains PASS (i32/u32/i64).

Deliverable: `residual/pure_residual_menu_generic.hpp` (structure, equal, ultra-low inv, run-gate, discrete float counting, residual_pdqsort).

**THE BEASTIE BOYZ**
