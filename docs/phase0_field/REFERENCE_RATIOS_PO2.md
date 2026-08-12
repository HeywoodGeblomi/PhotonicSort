# PO2 Reference Ratios — Independent Reproduction Target

**Date:** 2026-08-12  
**Team:** THE BEASTIE BOYZ  
**Stack:** post pipe_sparse kill (PR #76)  
**Method:** cold isolation, n=1e6, median of 5–9  
**Status:** **NOT field-level**

---

## How to reproduce

```bash
docker build -f reproduce/Dockerfile -t photonic-po2 .
docker run --rm photonic-po2
```

Compare **ratios** (menu/pdq), not absolute milliseconds.

---

## Reference ratios (local smoke, post-kill)

| type | pattern | menu/pdq | status |
|------|---------|----------:|--------|
| i32 | pipe_sparse | **0.81×** | CLOSED |
| u32 | pipe_sparse | **0.70×** | CLOSED |
| i32 | almost_sorted | **0.27×** | CLOSED |
| i32 | timestamps | **0.57×** | CLOSED |
| i32 | few_k16_wide | **0.84×** | CLOSED |
| i32 | db_pk | **1.11×** | CLOSED |
| i32 | random | **0.67×** | CLOSED |
| i32 | gaussianish | **0.52×** | CLOSED |
| i32 | organpipe | **0.08×** | CLOSED |
| i32 | sawtooth | **0.15×** | CLOSED |

**All residual soft spots CLOSED under cold isolation.**

---

## Acceptance band

Third-party Docker runs should match pattern-for-pattern within ~20% relative noise on ratios.
Soft-spot reopen threshold: any controlled pattern >1.15× under cold isolation.

---

## Non-claims

- Not field-level.
- Sequential POD int32/uint32 only.
- Ratios are the auditable surface; absolute times are host-dependent.
- Multi-arch + statistical CI still required for field-level entry.

**THE BEASTIE BOYZ**
