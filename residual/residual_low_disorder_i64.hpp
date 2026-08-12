#pragma once
/*
 * residual_low_disorder_i64 — pure residual for near-monotonic / low-disorder data
 * A2: ultra-low inv uses residual_pdqsort (insertion pathological on sparse swaps / db_pk).
 * EXTERNAL-clean. Not field-level. THE BEASTIE BOYZ
 */
#include <cstdint>
#include <cstddef>
#include <utility>
#include <algorithm>
#include <cstring>
#include "pdqsort_residual.h"

namespace residual_low_disorder {

inline double sample_inv_ratio(const int64_t *a, size_t n) {
    if (n < 2) return 0.0;
    size_t inv = 0, checked = 0;
    size_t S = n < 4096 ? (n - 1) : 4096;
    size_t st = (n - 1) / S; if (st < 1) st = 1;
    for (size_t i = 0; i + 1 < n && checked < S; i += st, ++checked)
        if (a[i] > a[i + 1]) ++inv;
    size_t windows[] = {0, n / 4, n / 2, (3 * n) / 4};
    for (size_t w = 0; w < 4; ++w) {
        size_t base = windows[w];
        if (base >= n - 1) continue;
        for (size_t k = 0; k < 64 && base + k + 1 < n; ++k, ++checked)
            if (a[base + k] > a[base + k + 1]) ++inv;
    }
    return checked ? (double)inv / (double)checked : 0.0;
}

inline bool should_try_low_disorder(const int64_t *a, size_t n) {
    if (n < 64) return false;
    double inv = sample_inv_ratio(a, n);
    if (inv >= 0.10) return false;
    size_t S = n < 1024 ? n : 1024;
    size_t st = n / S; if (st < 1) st = 1;
    int64_t mn = a[0], mx = a[0];
    uint64_t seen[128] = {};
    size_t approx_u = 0;
    for (size_t i = 0, c = 0; i < n && c < S; i += st, ++c) {
        if (a[i] < mn) mn = a[i];
        if (a[i] > mx) mx = a[i];
        uint64_t h = (uint64_t)a[i] * 0x9E3779B97F4A7C15ull;
        size_t slot = (h >> 57) & 127;
        uint64_t tag = (h >> 4) | 1;
        if (seen[slot] != tag) {
            if (seen[slot] == 0) ++approx_u;
            seen[slot] = tag;
        }
    }
    uint64_t span = (uint64_t)(mx - mn);
    if (approx_u <= 64) return false;
    if (span < (uint64_t)n && span < 100000ull) return false;
    {
        size_t step = n / 256; if (step < 1) step = 1;
        size_t breaks = 0;
        size_t first_break = n, last_break = 0;
        for (size_t i = step; i < n; i += step) {
            if (a[i] < a[i - step]) {
                ++breaks;
                if (first_break == n) first_break = i;
                last_break = i;
            }
        }
        if (breaks >= 1 && breaks <= 8 && first_break > n / 5 && last_break < (n * 4) / 5)
            return false;
    }
    if (span + 1 >= (uint64_t)n - (uint64_t)n / 50 && span <= (uint64_t)n + (uint64_t)n / 50)
        return false;
    if (inv <= 0.03) return true;
    size_t asc = 0, checked = 0;
    size_t step = n / 1024; if (step < 1) step = 1;
    for (size_t i = 0; i + 1 < n && checked < 1024; i += step, ++checked)
        if (a[i] <= a[i + 1]) ++asc;
    return checked && inv <= 0.06 && ((double)asc / (double)checked) >= 0.92;
}

inline bool residual_low_disorder_i64(int64_t *a, size_t n) {
    if (n < 2) return true;
    if (!should_try_low_disorder(a, n)) return false;
    double inv = sample_inv_ratio(a, n);
    // A2: residual_pdqsort for all low-disorder (insertion pathological on sparse swaps)
    residual_pdqsort(a, a + n);
    for (size_t i = 1; i < n; ++i)
        if (a[i] < a[i - 1]) return false;
    return true;
}

} // namespace residual_low_disorder
