# Wave 1 Residual Floors Status

**Status:** LOCKED 2026-08-11  
**Parent:** [Barrier Break Plan](../BARRIER_BREAK_PLAN.md)  
**Squad:** THE BEASTIE BOYZ

---

## Decision

| Barrier | Decision | Evidence (n=1e6 post residual_few_wide v2.5) |
|---------|----------|---------------------------------------------|
| **2 – few_unique_k16** | **CLOSED** | few_k16_wide 0.68–0.80× pdq; few_k16_dense ≤0.85×; two_values 0.60–0.88× (routing miss fixed). Success criterion ≤1.05× met on primary patterns. |
| **1 – Gaussian HE** | Residual quality OK / path-tax | Full-menu ~1.06–1.10×; isolated residual competitive. Remaining gap is measured menu/probe path tax, not residual quality floor. |
| **3 – Suite geo** | Improved, not yet locked | Post-v2.5 geo ~0.88–0.90× vs pdq (moved from ~0.93×). Still dragged by zipf, push_middle residual limit, reverse_segments. Further residual optional. |

## Landed work

- **residual_few_wide v2.5** (PR #51): `should_try_few_wide` accepts `sample_u ≤ 4` independent of range. Closes non-wide low-card routing miss (two_values 4.3× → ~0.6×). EXTERNAL-clean pure residual.
- Linear-collect attack evaluated and **killed** (negative: 6–9× slower than hash residual).

## Documented residual limits (not routing misses)

- High-skew mid-card (zipf_k16) ~1.4× vs pdq — residual quality limit under pure counting/hash approach.
- push_middle residual limit remains (~2.5×) as previously diagnosed.

## Hard constraints held

EXTERNAL-clean residual selection · Protect pure residual excellence · Worst-case O(n log n) · Sequential int64 only · No field-level language.

## Non-claims

Not field-level. Not a universal replacement for library pdq / ska / Timsort / std::sort. Not multi-type until Wave 2. Host-dependent absolute times; ratios are the auditable surface. Independent reproduction available via `reproduce/`.

**THE BEASTIE BOYZ**
