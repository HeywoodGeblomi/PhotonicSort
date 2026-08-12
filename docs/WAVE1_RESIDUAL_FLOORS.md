# Wave 1 Residual Floors — Locked Status

**Date:** 2026-08-12  
**Squad:** THE BEASTIE BOYZ  
**Parent:** [`BARRIER_BREAK_PLAN.md`](./BARRIER_BREAK_PLAN.md)

---

## Summary

Wave 1 residual floors for **barriers 1 and 2** are locked from post-`residual_few_wide` v2.5 measurements on the official Wave 0 harness.

| Barrier | Verdict | Evidence |
|---------|---------|----------|
| **2 – few_unique_k16** | **CLOSED** | few_k16_wide 0.66–0.69× pdq; two_values routing fixed (4.3× → 0.6–0.9×); dense closed |
| **1 – Gaussian HE** | Residual quality OK / **path-tax** | Full-menu 1.05–1.16×; isolated residual competitive. Remaining gap is menu probe tax, not residual quality floor |
| **3 – Suite geo** | **Moved** | Post-v2.5 geo 0.88–0.90× vs pdq (was ~0.93–0.99× pre-fix). Still short of aggressive field-level bars; residual limits remain (zipf, push_middle) |

zipf_k16 (~1.4–1.5×) remains a documented pure residual quality limit for high-skew mid-card (not a routing miss).

---

## Key change: residual_few_wide v2.5

`should_try_few_wide` now accepts `sample_u ≤ 4` independent of numeric range.  
This closes the non-wide low-card routing miss (two_values and similar dense k≤4 patterns). Wide mid-card path unchanged. EXTERNAL-clean.

Linear-collect Attack A was evaluated and **killed** (6–9× slower than hash residual).

---

## Authoritative measurements (official harness, n=1e6)

Ratios = Photonic / pdqsort (lower is better for Photonic). Multiple hosts; numbers are consistent within noise.

- two_values: **0.60–0.88×**
- few_k16_wide: **0.66–0.69×**
- few_k16_dense: **0.73–0.85×**
- gauss_nearflat: **1.05–1.16×**
- zipf_k16: **1.43–1.53×** (residual quality limit)
- Geo-mean vs pdq: **0.88–0.90×**

All correctness verifies ok=1.

Reference gap maps under `artifacts/Wave0_Reproduction/` and host-local runs from the official `reproduce/harness`.

---

## Decisions

1. Barrier 2 residual floors **CLOSED**. No further residual quality attack required on uniform few-unique mid-card for the current process class.
2. Barrier 1: residual quality is competitive; remaining full-menu gap is **path tax**. Document; do not claim residual floor ≤1.05 closed unless multi-host median consistently ≤1.05.
3. zipf_k16 high-skew mid-card is a permanent residual quality limit for the current pure residual menu (document, do not chase further under Wave 1).
4. Geo moved but residual soft spots (zipf, push_middle, reverse_segments) remain. Barrier 3 does not yet unlock field-level language.
5. Next wave options: further residual polish (optional) **or** open **Wave 2 multi-type** (barrier 4) which is mandatory for any path-(a) displacement claim vs pdq/Timsort/std.

---

## Non-claims (unchanged)

- Not field-level.
- Not a universal replacement for library pdq / ska / Timsort / std::sort.
- Sequential int64 only until multi-type surface exists and is measured.
- Independent reproduction infrastructure is live; external parties still need to run the harness for full verification.

**THE BEASTIE BOYZ**
