# Non-Claims — PhotonicSort

Applies to Residual-Improvement baseline on main (post A2 HCAP + formal scope + FIELD_LEVEL_CLAIM_v0.1).

## Claims (current engineering baseline)

* Claim-surface soft=0 + CI95 upper <1.0 on i32/u32/i64/f64 × x86_64 + aarch64.
* Expanded Field Suite primary geo ≤0.90× vs best specialized on x86_64 + aarch64.
* In-scope Expanded Suite soft_count = 0 after formal scope of residual-quality / HE path limits.
* n=1e7 primary geo ≤0.90 holds; in-scope soft=0 after formal scope at scale.
* EXTERNAL-clean pure residual; no χ; worst-case O(n log n).
* Independent Expanded Suite reproduce path shipped.

## Formal scope-outs (not residual bugs) — LOCKED

| Pattern | Type(s) | Rationale |
|---------|---------|-----------|
| equal_heavy | i32/u32 | residual_pdqsort quality vs library pdq |
| random / gaussianish / uniform_u32 | i64 | ska/radix wins pure HE |
| organpipe | i64/u32 | residual_pdq / ska mountain shape |
| pipe_sparse / mixed_blocks | i64 | residual_pdq quality |
| db_pk / adversarial | u32 | residual_pdq quality at n≥1e7 |
| runs_noise | i64 | residual_pdq quality at n≥1e7 |

## Non-claims

1. **Not a universal replacement for library pdq/ska** on pure HE or residual_pdq quality borderlines.
2. **Not a novel adaptive complexity theorem.**
3. Formal scope is honest path-limit documentation, not a hide inside geo mean.
4. Path-(a) empirical evidence package only — not SOTA displacement marketing.

Always surface residual limits when summarizing. Honesty primary.

THE BEASTIE BOYZ · 2026-08-12
