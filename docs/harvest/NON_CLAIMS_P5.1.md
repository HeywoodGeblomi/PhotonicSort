# NON_CLAIMS — Harvest P5.1 / v1.5.1-c

**Date:** 2026-08-11  
**Scope:** Pure residual adaptive int64 path after extended-suite evaluation + Phase 4 3-run merge residual

## What we claim

* On the **locked 11-pattern suite**: pure residual geo pure/pdq **0.553×**; every pattern < 1.0× pdqsort.
* On the **high-priority extended group**: geo **0.571×**; only gaussian sits at ≈1.10×.
* On the **full 38-pattern extended suite**: geo pure/pdq **0.500×**.
* **Zipf**, **constant**, and **push_middle / push_front** floors closed by pure techniques only.
* Path is EXTERNAL-clean: visible probe metrics only, no library dispatch to pdqsort/ska_sort/std::sort on the pure residual path.
* Worst-case complexity remains O(n log n).

## What we do **not** claim

1. **Not a field-level breakthrough.**
2. **Not a novel adaptive complexity theorem.**
3. **Not multi-architecture / multi-type.** Sequential int64 only.
4. **Not zero residual floors.** gaussian ≈ 1.10× remains a documented HE residual quality floor.
5. **Not independent reproduction yet.**
6. **Not a replacement for library pdq/ska in every context.**

## Language rules

* Prefer “pure residual menu competitive on expanded suite (geo 0.50×)” over “dominates” or “breakthrough.”
* Always pair geo numbers with the suite name (locked 11 / high-priority 13 / full 38).
* Always surface the **gaussian ≈ 1.10× floor** when summarizing P5.1.

**THE BEASTIE BOYZ**
