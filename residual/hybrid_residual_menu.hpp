#pragma once
/*
 * hybrid_residual_menu v24 — full Soft attack
 * v23 domain PURE held (db_fk_zipf dead).
 * Soft kills:
 *   equal_heavy → always PDQ first (before domain) — pure was 1.35–1.48× on eq-heavy
 *   mixed_blocks i64 → SKA (was PDQ-only for 8B; 1.73× soft)
 * STRUCTURE → O(n)
 * domain ≤65536 → PURE (db_fk_zipf, enums)
 * low-Înv dense_inv verify; HE → ska
 * EXTERNAL-clean. THE BEASTIE BOYZ 2026-08-12
 */
#include <cstdint>
#include <cstddef>
#include <algorithm>
#include <type_traits>
#include "pure_residual_menu.hpp"
#include "pure_residual_menu_i32.hpp"
#include "pure_residual_menu_u32.hpp"
#include "pdqsort_residual.h"
#include "ska_sort.hpp"

namespace hybrid_residual {

template<typename T>
inline bool is_sorted_asc(const T *a, size_t n) {
    for (size_t i = 1; i < n; ++i) if (a[i] < a[i - 1]) return false;
    return true;
}
template<typename T>
inline bool is_sorted_desc(const T *a, size_t n) {
    for (size_t i = 1; i < n; ++i) if (a[i] > a[i - 1]) return false;
    return true;
}

template<typename T>
inline void sample_full(const T *a, size_t n, size_t &inv, size_t &eq, size_t &u,
                        size_t &desc_runs, T &mn, T &mx) {
    const size_t S = 512;
    inv = eq = desc_runs = 0;
    T samp[512];
    mn = mx = a[0];
    bool in_desc = false;
    for (size_t c = 0; c < S; ++c) {
        size_t i = 1 + (c * (n - 1)) / S;
        size_t j = (i + 1 < n) ? i + 1 : i;
        if (a[i] < a[i - 1]) { ++inv; if (!in_desc) { ++desc_runs; in_desc = true; } }
        else in_desc = false;
        if (a[i] == a[j]) ++eq;
        if (a[i] < mn) mn = a[i];
        if (a[i] > mx) mx = a[i];
        samp[c] = a[i];
    }
    std::sort(samp, samp + S);
    u = 1;
    for (size_t c = 1; c < S; ++c) if (samp[c] != samp[c - 1]) ++u;
}

template<typename T>
inline size_t dense_inv(const T *a, size_t n) {
    size_t inv = 0;
    size_t step = n > 8192 ? n / 8192 : 1;
    for (size_t i = 0; i + 1 < n; i += step)
        if (a[i] > a[i + 1]) ++inv;
    return inv;
}

inline size_t dense_inv_samples(size_t n) {
    size_t step = n > 8192 ? n / 8192 : 1;
    return n > 0 ? (n - 1) / step : 0;
}

template<typename T>
inline uint64_t domain_of(T mn, T mx) {
    if constexpr (std::is_unsigned<T>::value)
        return (uint64_t)mx - (uint64_t)mn;
    else
        return (uint64_t)((int64_t)mx - (int64_t)mn);
}

template<typename T, typename PureFn>
inline int dispatch(T *a, size_t n, PureFn pure_fn) {
    if (n < 2) return 0;
    if (is_sorted_asc(a, n)) return 0;
    if (is_sorted_desc(a, n)) { std::reverse(a, a + n); return 0; }

    size_t inv, eq, u, desc_runs;
    T mn, mx;
    sample_full(a, n, inv, eq, u, desc_runs, mn, mx);
    const size_t S = 512;
    uint64_t dom = domain_of(mn, mx);

    /* Soft kill equal_heavy: PDQ first (before domain→PURE). */
    if (eq * 4 >= S * 3) {
        residual_pdqsort(a, a + n);
        return 0;
    }

    /* domain-aware PURE: db_fk_zipf, small enums, mid-card Zipf */
    if (dom <= 65536ull) return pure_fn(a, n);

    if (u <= 32) return pure_fn(a, n);
    if (u <= 128 && inv * 2 >= S && inv * 2 <= S) return pure_fn(a, n);
    if (desc_runs >= 3 && inv * 5 >= S * 3) return pure_fn(a, n);

    /* low-Înv: dense_inv verify */
    if (inv * 20 <= S) {
        if (dom <= (uint64_t)n * 2ull) {
            size_t dinv = dense_inv(a, n);
            size_t ds = dense_inv_samples(n);
            if (ds == 0 || dinv * 20 <= ds) return pure_fn(a, n);
            residual_pdqsort(a, a + n);
            return 0;
        }
        residual_pdqsort(a, a + n);
        return 0;
    }

    /* HE */
    if (u >= (S * 75) / 100 && inv * 5 >= S * 2) {
        ska_sort(a, a + n);
        return 0;
    }

    /* mixed_blocks / consecutive dense-inv: SKA for all key widths when û high */
    {
        size_t dinv = dense_inv(a, n);
        if (dom <= (uint64_t)n * 2ull && dinv >= 100) {
            if (u >= (S * 70) / 100) {
                ska_sort(a, a + n);
            } else {
                residual_pdqsort(a, a + n);
            }
            return 0;
        }
    }

    if (u <= 32) return pure_fn(a, n);
    residual_pdqsort(a, a + n);
    return 0;
}

inline int sort_i64(int64_t *a, size_t n) {
    return dispatch(a, n, [](int64_t *p, size_t nn) { return pure_residual::sort_i64(p, nn); });
}
inline int sort_i32(int32_t *a, size_t n) {
    return dispatch(a, n, [](int32_t *p, size_t nn) { return pure_residual::sort_i32(p, nn); });
}
inline int sort_u32(uint32_t *a, size_t n) {
    return dispatch(a, n, [](uint32_t *p, size_t nn) { return pure_residual::sort_u32(p, nn); });
}

} // namespace hybrid_residual
