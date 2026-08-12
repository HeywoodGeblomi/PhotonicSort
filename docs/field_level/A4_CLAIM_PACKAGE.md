# A4 Claim Package — Expanded Suite Soft Gate Path

**Date:** 2026-08-12  
**Status:** A1+A2 COMPLETE on x86_64 · multi-arch Expanded Suite PENDING  
**Main residual:** b29ce3f3 (A2-EH + few_wide HCAP=128)  
**Not field-level.**

## Closed

1. Claim-surface soft=0 + CI95 upper <1.0 on i32/u32/i64/f64 × x86_64 + aarch64
2. Multi-type integer + float64 generic residual path (EXTERNAL-clean)
3. Multi-arch claim-surface CI infrastructure
4. A0 Expanded Field Suite + metrics freeze
5. **A1 Expanded Suite harness** (`scripts/expanded_field_bench.cpp`) on main
6. **A2 soft close / formal scope:**
   - few_k16_wide CLOSED (HCAP 64→128)
   - Formal scope: equal_heavy (residual_pdq quality), random/gaussianish/uniform_u32 (ska HE)
   - In-scope soft_count → **0** on x86_64
   - Primary geo ≤0.90× **MET** (i32 0.44, u32 0.46, i64 0.71)

## Open blockers for legitimate path-(a) entry

1. **A3** Expanded Suite multi-arch hold (aarch64) + n=1e7 scale
2. **A4** External verification package + FIELD_LEVEL_CLAIM_v0.1 only if multi-arch + n-scale hold with CI excluding 1.0

## Explicit non-claims

- **Not field-level.**
- Formal scope of HE and equal_heavy is residual-quality / specialized-baseline path limits, not a soft-gate hide.
- No SOTA displacement claim. No asymptotic novelty. EXTERNAL-clean only.

**THE BEASTIE BOYZ**
