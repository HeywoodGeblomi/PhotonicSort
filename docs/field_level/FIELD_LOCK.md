# FIELD_LOCK — PHO-ACE-001 re-lock

**Date:** 2026-08-21  
**Ticket:** PHO-ACE-001  
**Branch:** pho-ace-001  
**Squad:** THE BEASTIE BOYZ  
**H0:** Sort body (c/photonic_sort.c and residual/*) unchanged. No new residual path. EXTERNAL-clean.

---

## Phase A — C product API oracle

| Item | Result |
|------|--------|
| Harness | `c/tests/oracle_c_api.c` |
| Entry | `photonic_sort_i64` / `_ex` / `photonic_sort` (shipped C API) |
| Reference | libc `qsort` same comparator |
| Patterns | empty · n=1 · sorted · reverse · all_equal · random · organpipe · few_unique_k4 · few_unique_k16 · adversarial · almost_sorted · mode_AGGRESSIVE · mode_FORCE_HOLE · generic |
| N (CI) | 100 000 |
| Local | ALL_CORRECTNESS_OK=1 (n=1e4 and n=1e5) |
| CI | `.github/workflows/c-oracle.yml` hard-fails on non-zero / missing OK line |

**Exit A:** GREEN. No correctness bug; H0 holds (body untouched).

---

## Phase B — Expanded Field re-lock (hybrid residual DEFAULT)

Harness: `scripts/expanded_field_bench_hybrid.cpp` (existing).  
Config: hybrid residual menu (Secondary Parity dual-evidence DEFAULT).  
Host: local x86_64, n=1 000 000, reps=3 (median).  
Date of run: 2026-08-21.

### Locked cells (hold on this run)

All 23 Expanded Field Suite patterns for **i64** on x86_64:

sorted · reverse · almost_sorted · organpipe · sawtooth · reverse_segments · push_middle · runs_noise · few_k4_dense · few_k16_dense · few_k16_wide · equal_heavy · zipf_k16 · db_pk · timestamps · pipe_sparse · adversarial · random · gaussianish · db_fk_zipf · timestamp_drift · mixed_blocks · uniform_u32

| Metric | Value |
|--------|------:|
| soft@1.15 (vs best specialized) | **0** |
| soft@1.20 (vs best specialized) | **0** |
| max ratio | 1.133 (pipe_sparse) |
| geo mean ratio | **0.618** |
| ok | 1 on every cell |

**No flips vs Field-Level Claim v0.5 charged surface.**  
**No cells dropped to UNSTABLE.**

### aarch64

Skipped on this local re-lock (no ARM runner in agent sandbox).  
Prior multi-arch money-shot (CI run 31807222980 and subsequent sp-multi-arch on tip) remains the aarch64 lock.  
If a future agent run on ubuntu-24.04-arm flips any cell, mark that cell UNSTABLE here and drop from locked language.

### Pure residual note

`scripts/expanded_field_bench.cpp` (pure) was also run for completeness.  
Soft spots remain as documented path limits (sorted, organpipe, random, gaussianish, mixed_blocks, uniform under pure residual quality).  
These do **not** affect the hybrid Field-Level charged surface.  
See [`UNSCOPED_BOUNDARIES.md`](../../residual/UNSCOPED_BOUNDARIES.md) and historical pure freezes.

---

## BENCHMARKS.md consistency

- **PRIMARY Expanded Field Suite (Aug-12 pure multi-arch soft=0 under formal scope)** stays as historical PRIMARY table.  
- Field-Level Claim v0.5 (hybrid SP dual-evidence, full charged surface, soft@1.20 vs pdq = 0 both ISAs) is the current charged lock; this document is the 2026-08-21 ACE re-lock of that surface.  
- No contradiction: BENCHMARKS PRIMARY is pure + scoped; FIELD_LOCK + FIELD_LEVEL_CLAIM_v0.5 are hybrid DEFAULT.

---

## Honesty

- NON_CLAIMS.md unchanged.  
- No P=NP. No hardware photons.  
- promote_ready remains false for universal-win.  
- PrymGyroSort / GyroRank / GeblomiSort untouched.  
- No new residual path invented under this ticket.

**A then B complete. Stop. No Phase C.**

THE BEASTIE BOYZ / Blam · 2026-08-21
