# A0 Lock — Claim Surface Freeze

**Date:** 2026-08-12  
**Status:** **COMPLETE**  
**Squad:** THE BEASTIE BOYZ

---

## Deliverables

| Artifact | Path | Status |
|----------|------|--------|
| Expanded Field Suite | [`EXPANDED_FIELD_SUITE.md`](./EXPANDED_FIELD_SUITE.md) | **FROZEN** |
| Baselines | [`BASELINES_LOCKED.md`](./BASELINES_LOCKED.md) | **FROZEN** |
| Metrics + kill criterion | [`METRICS_LOCKED.md`](./METRICS_LOCKED.md) | **FROZEN** |
| Entry attack plan | [`FIELD_LEVEL_ENTRY_ATTACK.md`](./FIELD_LEVEL_ENTRY_ATTACK.md) | Parent |

---

## Explicit non-actions (A0)

- **No residual code changes**
- **No field-level claim language**
- **No geo published as “field-level”** under pdq-only comparison

---

## Next: A1

Multi-baseline gap map on Expanded Field Suite:

- Emit ratio vs pdq, ska (where applicable), std::sort, and **best specialized**
- n=1e6 primary, n=1e5 sanity
- Types: i32, u32, i64
- Soft spots redefined vs best specialized (≤1.15×)

---

## One-line

**A0 freezes what we measure and how we score it. A1 reveals where we actually stand against best specialized.**

**THE BEASTIE BOYZ**
