#pragma once
/*
 * residual_push_middle_u32 — 3-run merge residual for sorted-prefix + island + sorted-suffix
 * Adapted from pure_residual_menu try_push_middle (i64).
 * EXTERNAL-clean. O(n + k log k) with k = island size.
 * THE BEASTIE BOYZ — Wave 2 residual polish 2026-08-12
 */
#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <algorithm>

namespace residual_push_middle_u32 {

inline bool try_push_middle(uint32_t *a, size_t n) {
    if (n < 64) return false;

    size_t prefix_end = 1;
    while (prefix_end < n && a[prefix_end] >= a[prefix_end - 1]) ++prefix_end;

    size_t suffix_start = n - 1;
    while (suffix_start > 0 && a[suffix_start] >= a[suffix_start - 1]) --suffix_start;
    if (prefix_end >= suffix_start) return false;

    size_t mid_lo = prefix_end, mid_hi = suffix_start, mid_n = mid_hi - mid_lo;
    size_t left_run = prefix_end, right_run = n - suffix_start;
    if (left_run + right_run < (n * 2) / 3 || mid_n > n / 4) return false;
    if (mid_n < 1) return true;

    bool push_front_shape = (left_run < n / 20 && right_run >= n / 2 && mid_n <= n / 4);
    if (!push_front_shape && left_run < n / 5) return false;

    if (mid_n <= 64) {
        for (size_t i = mid_lo + 1; i < mid_hi; ++i) {
            uint32_t key = a[i]; size_t j = i;
            while (j > mid_lo && a[j - 1] > key) { a[j] = a[j - 1]; --j; }
            a[j] = key;
        }
    } else {
        std::sort(a + mid_lo, a + mid_hi);
    }

    uint32_t *tmp = (uint32_t *)std::malloc(n * sizeof(uint32_t));
    if (!tmp) return false;
    size_t i = 0, j = mid_lo, k = mid_hi, p = 0;
    while (i < mid_lo && j < mid_hi && k < n) {
        uint32_t x = a[i], y = a[j], z = a[k];
        if (x <= y && x <= z) { tmp[p++] = x; ++i; }
        else if (y <= z) { tmp[p++] = y; ++j; }
        else { tmp[p++] = z; ++k; }
    }
    while (i < mid_lo && j < mid_hi) {
        if (a[i] <= a[j]) tmp[p++] = a[i++]; else tmp[p++] = a[j++];
    }
    while (i < mid_lo && k < n) {
        if (a[i] <= a[k]) tmp[p++] = a[i++]; else tmp[p++] = a[k++];
    }
    while (j < mid_hi && k < n) {
        if (a[j] <= a[k]) tmp[p++] = a[j++]; else tmp[p++] = a[k++];
    }
    while (i < mid_lo) tmp[p++] = a[i++];
    while (j < mid_hi) tmp[p++] = a[j++];
    while (k < n) tmp[p++] = a[k++];
    std::memcpy(a, tmp, n * sizeof(uint32_t));
    std::free(tmp);

    for (size_t t = 1; t < n; ++t) if (a[t] < a[t - 1]) return false;
    return true;
}

} // namespace residual_push_middle_u32
