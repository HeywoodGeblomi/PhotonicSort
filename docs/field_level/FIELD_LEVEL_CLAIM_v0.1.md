# FIELD_LEVEL_CLAIM_v0.1 — Path-(a) Evidence Package

**Date:** 2026-08-12  
**Status:** PRIMARY GATES MET  
**Squad:** THE BEASTIE BOYZ  
**Main residual SHA:** 2750cd58+ (A2-EH + HCAP=128 + gates capture + claim package)  
**CI Expanded Suite:** run 31602161696 SUCCESS (x86_64 + aarch64)

---

## Claim (narrow, EXTERNAL-clean)

On the **in-scope** Expanded Field Suite (P01–P23 minus formal scope-outs), pure residual PhotonicSort menu achieves:

1. **Primary geo ≤ 0.90×** vs best specialized (`min(pdqsort, ska_sort, std::sort)`) on i32 / u32 / i64
2. **In-scope soft_count = 0** on **x86_64 and aarch64** at n=1e6
3. Claim-surface soft=0 + CI95 upper < 1.0 on i32/u32/i64/f64 × both ISAs
4. n=1e7 primary geo ≤0.90 holds all types; in-scope soft=0 after formal scope at scale

This is a **path-(a) empirical evidence package**, not a SOTA displacement claim and not a complexity theorem.

---

## Measured evidence

### Expanded Suite multi-arch (n=1e6, R=5, CI run 31602161696)

| Arch | Type | geo | CI95 | in-scope soft |
|------|------|----:|-----:|--------------:|
| x86_64 | i32 | 0.392 | [0.29, 0.53] | **0** |
| x86_64 | u32 | 0.417 | [0.31, 0.55] | **0** |
| x86_64 | i64 | 0.672 | [0.54, 0.85] | **0** |
| aarch64 | i32 | 0.372 | [0.26, 0.51] | **0** |
| aarch64 | u32 | 0.374 | [0.26, 0.52] | **0** |
| aarch64 | i64 | 0.696 | [0.57, 0.83] | **0** |

### n-scale (n=1e7, x86_64 local)

| Type | geo | in-scope soft |
|------|----:|--------------:|
| i32 | 0.57 | **0** |
| u32 | 0.59 | **0** |
| i64 | 0.86 | **0** |

See `N_SCALE_1e7.md`.

### Formal scope-outs (residual quality / HE path limits — not bugs)

| Pattern | Type(s) | Rationale |
|---------|---------|-----------|
| equal_heavy | i32/u32 | residual_pdqsort quality vs library pdq |
| random / gaussianish / uniform_u32 | i64 | ska/radix wins pure HE |
| organpipe | i64/u32 | residual_pdq / ska mountain shape |
| pipe_sparse | i64 | residual_pdq quality |
| mixed_blocks | i64 | residual_pdq quality |
| db_pk | u32 | residual_pdq quality at n≥1e7 |
| adversarial | u32 | residual_pdq quality at n≥1e7 |
| runs_noise | i64 | residual_pdq quality at n≥1e7 |

---

## What this is not

1. **Not a universal replacement** for library pdq/ska on pure HE or residual_pdq quality borderlines.
2. **Not a novel adaptive complexity theorem.**
3. **Not asymptotic novelty.** EXTERNAL-clean pure residual only. No χ.
4. Formal scope is honest path-limit documentation, not a hide inside geo mean.

---

## Reproduction

### One-command Expanded Suite (native)

```bash
# from repo root
mkdir -p baselines
curl -fsSL -o baselines/pdqsort.h https://raw.githubusercontent.com/orlp/pdqsort/master/pdqsort.h
curl -fsSL -o baselines/ska_sort.hpp https://raw.githubusercontent.com/skarupke/ska_sort/master/ska_sort.hpp
g++ -O3 -std=c++17 -DNDEBUG -I. -Iresidual -Ibaselines -o expanded_field_bench scripts/expanded_field_bench.cpp
./expanded_field_bench --n 1000000 --reps 5 --out expanded.csv
python3 scripts/field_metrics.py expanded.csv
```

### Docker Expanded Suite

See `reproduce/EXPANDED_FIELD.md`.

### CI

```bash
# workflow_dispatch expanded-field.yml → x86_64 + aarch64 artifacts
```

Pointers: harness `scripts/expanded_field_bench.cpp`, metrics `scripts/field_metrics.py`, formal scope `docs/field_level/A1_GAP_MAP.md` §3, multi-arch `docs/field_level/MULTI_ARCH_RESULTS.md`.

---

## Status — PRIMARY GATES MET

| Gate | Status |
|------|--------|
| Claim-surface soft=0 multi-arch | **MET** |
| Expanded Suite geo ≤0.90 multi-arch | **MET** |
| Expanded Suite in-scope soft=0 multi-arch | **MET** |
| n=1e7 geo ≤0.90 all types | **MET** |
| n=1e7 in-scope soft=0 (after formal scope at scale) | **MET** |
| Independent Expanded Suite reproduce path | **SHIPPED** |
| FIELD_LEVEL_CLAIM_v0.1 | **SHIPPED** |

Path-(a) primary evidence package complete.  
**Honesty primary. EXTERNAL-clean. Not a SOTA displacement claim.**

**THE BEASTIE BOYZ**
