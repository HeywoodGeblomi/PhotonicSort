# Vector-2 Claim

**Status: Criteria MET** on the locked Domination Suite v0.1 (2026-08-09)  
**Team:** THE BEASTIE BOYZ

## Primary document

- [VECTOR2_CLAIM_v0.1.md](./VECTOR2_CLAIM_v0.1.md) — full locked claim, measured numbers, scope language, and non-claims.

## What was measured

Photonic hybrid residual configuration:

- Structured / patterned / low-cardinality paths: pure-C PhotonicSort (probe → STRUCTURE early-exit with mandatory O(n) verify, LOW_CARD, LOW_DISORDER, PATTERNED).
- High-entropy (random, adversarial): correct residual *selection* from visible probe metrics + dispatch to library-strength pdqsort / ska_sort.

This is valid engineering (selection + strong residual). It is **not** a claim that Photonic’s pure-C residual alone beats pdqsort or ska_sort on pure random / adversarial.

## Key numbers (n=1e6)

| Criterion | Measured |
|-----------|----------|
| Geo mean vs `std::sort` | **7.08×** |
| Geo mean vs best specialized | **1.94×** |
| Min vs `std::sort` | **1.32×** |
| Major regressions vs specialized | **0** |

## Scope language (mandatory)

> On Domination Suite v0.1, the locked Vector-2 criteria are met (geo vs std ≈ 7.1×, geo vs best specialized ≈ 1.9×, min ≥ 1.3×, zero major regressions). Structured / patterned / low-card wins remain pure-C Photonic. High-entropy residual uses library-strength pdqsort / ska_sort after correct selection from visible probe metrics. Result is scoped to this suite; broader evaluation may differ. No asymptotic claim.

Honesty primary. No asymptotic claim.
