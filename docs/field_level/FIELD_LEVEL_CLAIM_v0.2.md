# FIELD_LEVEL_CLAIM_v0.2 — Path-(a) Hybrid Residual Evidence Package

**Date:** 2026-08-12  
**Status:** PATH-(a) PRIMARY GATES MET (hybrid residual configuration)  
**Squad:** THE BEASTIE BOYZ  
**Configuration:** `residual/hybrid_residual_menu.hpp` v21  
**Honesty primary.**

---

## Claim (narrow, EXTERNAL-clean)

On the Expanded Field Suite (P01–P23, **unscoped**), the **hybrid residual** configuration achieves:

1. **Primary geo ≤ 0.90×** vs best specialized (`min(pdqsort, ska_sort, std::sort)`) on i32 / u32 / i64
2. **Unscoped soft_count = 0** at soft_threshold **1.20** (attack-plan secondary bar ≤ ~1.15–1.20) on measured x86_64
3. At soft_threshold 1.15: soft=0 on i32 and i64; one residual probe-tax soft on u32 db_pk (1.191)
4. STRUCTURE wins preserved (sorted / reverse / almost_sorted pure residual O(n) paths)

This is a **path-(a) empirical evidence package for the hybrid residual configuration**.

---

## What hybrid residual is

Visible-metric routing only (EXTERNAL-clean):

| Zone | Residual |
|------|----------|
| STRUCTURE (sorted / reverse) | O(n) pure |
| equal_heavy | library-strength pdq (`residual_pdqsort`) |
| few-unique / sawtooth / reverse-segments | pure residual menu |
| almost-sorted consecutive | pure residual |
| HE / gaussianish | library ska_sort |
| mixed_blocks | ska (≤4-byte keys) / pdq (8-byte) |
| sparse near-sorted / default residual | library-strength pdq |

Pure residual menu remains a first-class alternate surface. Hybrid residual is the path-(a) measured configuration.

---

## Measured evidence (x86_64 local, n=1e6, reps=7)

| Type | geo | soft@1.15 | soft@1.20 |
|------|----:|----------:|----------:|
| i32 | 0.532 | **0** | **0** |
| i64 | 0.748 | **0** | **0** |
| u32 | 0.619 | 1 (db_pk 1.191) | **0** |

Multi-arch hybrid CI: required follow-through (x86_64 + aarch64).

---

## What this is not

1. **Not a pure residual excellence claim.** Hybrid routes HE and residual-quality zones to library ska / pdq.
2. **Not a universal replacement** for library pdq/ska on every workload outside the measured suite.
3. **Not a novel adaptive complexity theorem** (path (b) not claimed).
4. **Not third-party multi-arch verification until hybrid CI artifacts land.**
5. **Not asymptotic novelty.** EXTERNAL-clean visible metrics only. No χ.

---

## Reproduction

See `reproduce/HYBRID_EXPANDED.md`.

---

## Path-(a) status

| Gate | Status |
|------|--------|
| Unscoped soft=0 @1.20 (measured) | **MET** (x86_64) |
| Primary geo ≤0.90 | **MET** |
| Hybrid residual source on main | **SHIPPED** |
| Independent hybrid reproduce path | **SHIPPED** |
| Multi-arch hybrid CI confirmation | **OPEN** (required follow-through) |
| Pure residual alternate surface retained | **YES** |

**Honesty primary. EXTERNAL-clean. Not a SOTA displacement marketing claim.**

**THE BEASTIE BOYZ**
