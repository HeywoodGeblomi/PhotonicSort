# Baselines Locked — A0

**Date:** 2026-08-12  
**Status:** FROZEN  
**Parent:** [`FIELD_LEVEL_ENTRY_ATTACK.md`](./FIELD_LEVEL_ENTRY_ATTACK.md)

---

## 1. Best specialized definition

For each `(pattern, type, n, arch)`:

```
best_ms = min({ ms(B) | B ∈ applicable_baselines(pattern, type) })
ratio   = pure_residual_ms / best_ms
```

**Primary field-level metric** = geometric mean of `ratio` over the Expanded Field Suite (in-scope patterns only).

---

## 2. Locked baselines

| Baseline | Source | Applicable types | Notes |
|----------|--------|------------------|-------|
| **pdqsort** | orlp/pdqsort (`pdqsort.h`) | All POD types in suite | Always applicable |
| **ska_sort** | ska_sort (unsigned / integer radix where legal) | `uint32_t`, `uint64_t`, and integer keys bitcast-safe only | **Not** applied to signed-negative-heavy or float until verified |
| **std::sort** | libstdc++ / libc++ `std::sort` | All | Always applicable |

Optional later (A3+ only, not required for A0/A1):

| Baseline | When |
|----------|------|
| Timsort / powersort-class | Generic / run-heavy float or comparator surface only |

---

## 3. Applicability rules (honest)

1. **ska_sort** is included in `min()` only when the harness uses a **supported unsigned integer path** (or documented safe bitcast). Do not force ska on signed patterns where the reference implementation is undefined or slower by construction.
2. If ska is inapplicable, `best = min(pdq, std::sort)`.
3. Never compare against a baseline that fails correctness on that pattern.
4. Hybrid “library residual” configurations are **out of pure residual claim** — separate Vector-2 numbers only.

---

## 4. Pinning

- pdqsort: upstream orlp single header; pin commit/tag in harness README when fetching
- ska_sort: pin version/commit in harness
- std::sort: document compiler (`g++ -O3 -std=c++17 -DNDEBUG`) and lib

Compiler flags for all timed code: **`-O3 -std=c++17 -DNDEBUG`**.

---

## 5. Freeze statement

Baselines and applicability rules above are frozen for A1–A4 measurement. Changing the baseline set requires a new A0 revision.

**THE BEASTIE BOYZ**
