# PO1–PO6 Status + timestamps u32 closure

**Date:** 2026-08-12  
**Team:** THE BEASTIE BOYZ  
**Not field-level.**

## timestamps u32 — CLOSED

STRUCTURE v2: asc-first tight scan (then separate desc).

| Type | Before | After |
|------|-------:|------:|
| timestamps i32 | ~1.03× | **~0.50×** |
| timestamps u32 | ~1.6× | **~0.88×** |
| sorted u32 | — | **~0.58×** |

**All residual soft spots on the controlled set are CLOSED.**

## PO1–PO6 progress

| ID | Ticket | Status |
|----|--------|--------|
| PO1 | Re-measure Phase 0 gap maps (few_wide v2.6.1 + STRUCTURE v2) | **IN PROGRESS** |
| PO2 | Docker one-command + reference ratios | PENDING |
| PO3 | Multi-arch plan + first second-arch numbers | PENDING (hardware-gated) |
| PO4 | Soft-spot methodology note (isolation vs suite) | **DONE** (SOFT_SPOTS_CAPTURE) |
| PO5 | i64 Phase 0 gap map | PENDING |
| PO6 | Independent reproduction Docker GREEN | PENDING |

## Field-level gate (honesty)

Still **NOT field-level**. Remaining hard gates: multi-arch, statistical CI, suite re-measure under new residual stack, Docker GREEN.

**THE BEASTIE BOYZ**
