# PO3 Multi-Arch Gate — MET

**Date:** 2026-08-12  
**Team:** THE BEASTIE BOYZ  
**CI run:** https://github.com/HeywoodGeblomi/PhotonicSort/actions/runs/31561435057  
**Status:** **MULTI-ARCH GATE MET** · still **NOT field-level**

---

## Gate criteria

Measured ratios on **≥2 distinct ISAs**, same suite, same baselines, all controlled patterns ≤1.15×, all ok=1.

| Arch | ISA | Runner | Status |
|------|-----|--------|--------|
| Arch-1 | x86_64 | `ubuntu-24.04` (AMD EPYC 9V74) | **GREEN** |
| Arch-2 | aarch64 | `ubuntu-24.04-arm` (CPU part 0xd49 / Neoverse-class) | **GREEN** |

---

## Ratios (n=1e6, median of 7)

| pattern | x86_64 | arm64 |
|---------|-------:|------:|
| pipe_sparse i32 | 0.69× | 0.76× |
| pipe_sparse u32 | 0.76× | 0.74× |
| almost_sorted | 0.16× | 0.20× |
| timestamps | 0.43× | 0.62× |
| few_k16_wide | 0.90× | 0.80× |
| db_pk | 0.82× | 0.88× |
| random | 0.55× | 0.56× |
| gaussianish | 0.46× | 0.49× |
| organpipe | 0.07× | 0.09× |
| sawtooth | 0.13× | 0.12× |

All ok=1. Max ratio on either arch: **0.90×** (no soft spots).

---

## Work-around used

Local ARM hardware was unavailable. Gate closed via public GitHub Actions native runners (`ubuntu-24.04-arm`, free for public repos). Motif from APS DOI 10.1103/4k13-sdjh: the richer local resource is not required when a correctly structured measurement protocol produces equivalent predictions under the right conditions.

**Honesty:** QEMU would not have counted. Only native runner numbers count. Both jobs are native.

---

## Non-claims

- Multi-arch gate MET is **not** field-level.
- i64 gap map (PO5), full Phase 0 suite, statistical CI still open.
- Sequential POD int32/uint32 only in this measurement.

**THE BEASTIE BOYZ**
