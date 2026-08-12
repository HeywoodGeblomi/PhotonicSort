# Non-Claims — PhotonicSort

Applies to Residual-Improvement baseline on main (post A2 HCAP + formal scope) and historical Harvest freezes.

## Claims (current engineering baseline)

* Claim-surface soft=0 + CI95 upper <1.0 on i32/u32/i64/f64 × x86_64 + aarch64.
* Expanded Field Suite primary geo ≤0.90× vs best specialized on x86_64 + aarch64 (i32/u32 ~0.37–0.42, i64 ~0.67–0.70).
* In-scope Expanded Suite soft_count = 0 after formal scope of residual-quality / HE path limits.
* EXTERNAL-clean pure residual; no χ; worst-case O(n log n).

## Formal scope-outs (not residual bugs) — LOCKED

| Pattern | Type(s) | Rationale |
|---------|---------|-----------|
| equal_heavy | i32/u32 | residual_pdqsort quality vs library pdq |
| random / gaussianish / uniform_u32 | i64 | ska/radix wins pure HE |
| organpipe | i64 | residual_pdq / ska mountain shape |
| pipe_sparse | i64 | residual_pdq quality |
| mixed_blocks | i64 | residual_pdq quality |

## Non-claims

1. **Not a full field-level breakthrough** until n-scale all types + independent Expanded Suite reproduce path hold.
2. **Not a novel adaptive complexity theorem.**
3. **Not a universal replacement for library pdq/ska** on pure HE or residual_pdq quality borderlines.
4. Formal scope is honest path-limit documentation, not a hide inside geo mean.

Always surface residual limits when summarizing. Honesty primary.

THE BEASTIE BOYZ · 2026-08-12
