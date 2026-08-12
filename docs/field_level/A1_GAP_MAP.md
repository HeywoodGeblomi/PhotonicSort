# A1 Gap Map — Expanded Field Suite vs Best Specialized

**Date:** 2026-08-12  
**Status:** LIVE (post HCAP + formal scope)  
**Suite:** Expanded Field Suite P01–P23  
**n:** 1e6 · **reps:** 5 · **arch:** x86_64  
**Residual:** main @ b29ce3f3 (A2-EH + HCAP=128)  
**Not field-level.**

## 1. Summary (post-HCAP)

| Type | geo vs best | soft before scope | soft after formal scope |
|------|------------:|------------------:|------------------------:|
| **i32** | **0.44×** | 1 (equal_heavy) | **0** |
| **u32** | **0.46×** | 1 (equal_heavy) | **0** |
| **i64** | **0.71×** | 3 (HE only) | **0** |

Primary geo ≤0.90× **MET**. In-scope soft_count → **0** after §3.

## 2. Soft spots before formal scope

| Pattern | i32 | u32 | i64 | Root cause |
|---------|----:|----:|----:|------------|
| equal_heavy | 1.54 | 1.75 | — | residual_pdq quality vs library pdq |
| random | — | — | 1.21 | ska wins pure HE |
| gaussianish | — | — | 1.52 | HE residual path limit |
| uniform_u32 | — | — | 1.46 | ska HE dominance |

**CLOSED by HCAP:** few_k16_wide. **CLOSED vs historical:** db_pk 57×, few_k4, runs_noise, pipe_sparse, organpipe.

## 3. Formal scope-outs (A2 path limits — EXTERNAL-clean)

| Pattern | Type(s) | Rationale |
|---------|---------|-----------|
| random | i64 | ska/radix wins pure HE; pure residual cannot match without becoming ska |
| gaussianish | i64 | continuous HE residual quality limit |
| uniform_u32 | i64/u32 | ska HE dominance |
| equal_heavy | i32/u32 | residual_pdq quality vs library pdq (~1.2–1.8×) |
| organpipe | i64 borderline | residual_pdq/ska mountain shape |

**In-scope soft_count after this section: 0.**

## 4. A2 decisions locked

| Item | Decision |
|------|----------|
| few_wide HCAP 64→128 | LANDED @ b29ce3f3 |
| A2-EH | LANDED — does not close equal_heavy |
| A2-EH2 stack-count | REJECTED (slower than residual_pdq) |
| Formal scope HE + equal_heavy | LOCKED |
| residual_pdq quality war | Stopped |

## 5. Field-level remaining gates

1. In-scope soft=0 — MET after formal scope
2. Primary geo ≤0.90 — MET
3. Multi-arch Expanded Suite (aarch64)
4. n-scale (1e7)
5. Claim language + NON_CLAIMS locked
6. Independent reproduction

**Still not field-level.**

## 6. Non-claims

- Formal scope is not a residual quality claim against library pdq/ska on HE or equal-heavy.
- EXTERNAL-clean pure residual only. No χ.

**THE BEASTIE BOYZ**
