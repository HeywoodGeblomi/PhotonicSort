# Pure HE Residual — FROZEN

**Date:** 2026-08-10
**Decision:** Freeze pure HE residual. No further pure scatter/permute micros this cycle.

## Final isolated numbers (uniform_i64, n=1e6)

| Config | pure/ska | Notes |
|--------|---------:|-------|
| Prior band (pre-blocked) | ~1.08–1.20× | baseline |
| Blocked scatter + prefetch (BLOCK=256) | **1.022–1.089×** | best pure residual recorded |
| + ping-pong buffer | 1.077× | REGRESS → reverted |
| + 16-bit first digit | 1.349× | REGRESS → reverted |

**Target ≤0.90× ska: NOT MET.**

## What stays

- Classical MSD: fixed 8-bit, flat counts, buffered scatter
- Blocked scatter (256) + write-side prefetch
- INS=192
- majority_v2 + sparse-cluster routing
- Class1 inv gate, STRUCTURE verify

## Remaining gap (honest)

Isolated pure HE is **~1.02–1.09× ska**. Last ~2–10% is ska-class write/permute microarchitecture.

Gates: **RED**. Not a field-level breakthrough.

**THE BEASTIE BOYZ**
