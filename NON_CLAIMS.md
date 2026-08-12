# Non-Claims — PhotonicSort

Applies to Residual-Improvement baseline on main (post A2 HCAP + formal scope) and historical Harvest freezes.

## Claims (current engineering baseline)

* Claim-surface soft=0 + CI95 upper <1.0 on i32/u32/i64/f64 × x86_64 + aarch64.
* Expanded Field Suite (P01–P23) primary geo ≤0.90× vs best specialized on x86_64 (i32 0.44, u32 0.46, i64 0.71).
* In-scope Expanded Suite soft_count = 0 after formal scope of residual-quality / HE path limits.
* EXTERNAL-clean pure residual; no χ; worst-case O(n log n).

## Formal scope-outs (not residual bugs)

| Pattern | Type(s) | Rationale |
|---------|---------|-----------|
| equal_heavy | i32/u32 | residual_pdqsort quality vs library pdq |
| random / gaussianish / uniform_u32 | i64 (u32 near) | ska/radix wins pure HE |

## Non-claims

1. **Not a field-level breakthrough** until multi-arch Expanded Suite + n-scale + claim package hold.
2. **Not a novel adaptive complexity theorem.**
3. **Not a universal replacement for library pdq/ska** on pure HE or equal-heavy residual_pdq quality.
4. **Not independent Expanded Suite multi-arch verification yet** (claim-surface multi-arch is done; Expanded Suite aarch64 pending).
5. Formal scope is honest path-limit documentation, not a hide inside geo mean.

Always surface residual limits when summarizing. Honesty primary.

THE BEASTIE BOYZ · 2026-08-12
