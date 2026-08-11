# Non-Claims — PhotonicSort

Applies to Residual-Improvement v2.4.1 (current engineering baseline) and Harvest P5.1 / v1.5.1-c (historical freeze).

## Claims (engineering baseline v2.4.1)

* Targeted residual floors closed: two_values / k=2 (0.42–0.47×), reverse_segments_8 (0.85–0.93×).
* 16-pattern post-merge geo pure/pdq **0.648×**; indicative full extended ~0.55–0.67×.
* EXTERNAL-clean; no library dispatch on pure residual path; worst-case O(n log n).

## Claims (historical Harvest P5.1)

* Locked 11-pattern suite: geo pure/pdq **0.553×**.
* Full 38-pattern extended suite: geo **0.500×**.
* Zipf, constant, push_middle/push_front floors closed by pure techniques only.

## Non-claims

1. **Not a field-level breakthrough.**
2. **Not a novel adaptive complexity theorem.**
3. **Not multi-architecture / multi-type.** Sequential int64 only.
4. **Not zero residual floors.** Documented limits remain:
   - few_unique_k16 ≈ 1.0–1.2× (residual quality limit)
   - Gaussian HE ≈ 1.1–1.2× full-menu (residual quality / path limit; isolated MSD competitive)
5. **Not independent reproduction yet.**
6. **Not a universal replacement for library pdq/ska.**

Always surface the residual limits when summarizing. Honesty primary.

THE BEASTIE BOYZ · 2026-08-11
