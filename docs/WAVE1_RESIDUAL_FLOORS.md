# Wave 1 Residual Floors — Status Lock

**Date:** 2026-08-12  
**Parent:** [`BARRIER_BREAK_PLAN.md`](./BARRIER_BREAK_PLAN.md)  
**Squad:** THE BEASTIE BOYZ

---

## Summary

| Barrier | Verdict | Evidence (n=1e6, official harness, post residual_few_wide v2.5) |
|---------|---------|------------------------------------------------------------------|
| **2 – few_unique_k16** | **CLOSED** | few_k16_wide 0.678–0.686× pdq; two_values routing 0.600–0.883× (was 4.32×); residual quality + routing fixed |
| **1 – Gaussian HE** | Residual quality OK / path-tax | Full menu 1.056–1.097×; isolated HE residual previously competitive (~0.89×). Remaining gap is menu probe tax, not residual quality floor |
| **3 – Suite geo** | Movement noted | Geo 0.877–0.897× vs pdq (improved from pre-v2.5 ~0.93×). In or near ≤0.85–0.90 success band; still dragged by zipf / push_middle / reverse_segments |

All patterns ok=1. EXTERNAL-clean pure residual only. Sequential int64. **Not field-level.**

## Barrier 2 detail

- `residual_few_wide` **v2.5** (PR #51): `should_try_few_wide` accepts `sample_u ≤ 4` independent of range.
- Closes non-wide low-card routing miss (two_values, dense few).
- Wide mid-card path unchanged; counting still owns compact larger-k.
- Attack A (linear-collect) evaluated and **killed** (6–9× slower than hash residual).

| Pattern | vs pdq (sandbox / Harper) | Notes |
|---------|--------------------------:|-------|
| two_values | **0.600× / 0.883×** | routing CLOSED |
| few_k4_wide | 0.824× | CLOSED |
| few_k16_wide | **0.678× / 0.686×** | residual quality CLOSED |
| few_k16_dense | 0.846× / 0.752× | CLOSED |
| zipf_k16 | 1.428× / 1.441× | residual quality limit (high-skew mid-card) — documented |

## Barrier 1 detail

- Isolated classical MSD residual competitive vs pdq/ska on continuous near-uniform.
- Full-menu path tax (~10–20%) leaves gauss_nearflat at ~1.05–1.15× depending on host.
- Verdict: residual quality OK; document permanent path-tax component. No further residual micro-work required for this barrier under current menu.

## Remaining soft spots (not barrier 1/2 success criteria)

- zipf_k16 ~1.4× (high-skew residual quality)
- push_middle ~2.5× (known 3-run merge residual limit)
- reverse_segments_8 ~1.6×
- db_pk ~1.17× (post Phase 1)

## Non-claims

- Not field-level.
- Not multi-type / multi-architecture.
- Not a universal replacement for library pdq / ska / Timsort / std::sort.
- zipf high-skew mid-card remains a residual quality limit.
- Gaussian full-menu residual includes measured path tax.

## Next

Wave 1 residual floors for barriers 1+2 locked. Geo (barrier 3) optional further residual work or freeze specialized residual claim and open **Wave 2 multi-type**.

**THE BEASTIE BOYZ**
