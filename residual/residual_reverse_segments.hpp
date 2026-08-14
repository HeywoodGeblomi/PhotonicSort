#pragma once
/*
 * residual_reverse_segments — O(n) fix for fixed-size descending blocks
 * (expanded-field reverse_segments pattern and kin).
 * Detect uniform descending run length, reverse each block, verify sorted.
 * EXTERNAL-clean. THE BEASTIE BOYZ 2026-08-14
 */
#include <cstddef>
#include <algorithm>
#include <type_traits>

namespace residual_reverse_segments {

template<typename T>
inline bool try_reverse_segments(T *a, size_t n) {
    if (n < 8) return false;
    // First descending run length
    size_t i = 0;
    while (i + 1 < n && a[i] > a[i + 1]) ++i;
    const size_t run0 = i + 1;
    if (run0 < 2 || run0 > 8192) return false;
    if (n < run0 * 2) return false;

    size_t pos = 0;
    int blocks = 0;
    while (pos < n) {
        size_t e = pos + run0;
        if (e > n) e = n;
        for (size_t j = pos; j + 1 < e; ++j)
            if (!(a[j] > a[j + 1])) return false;
        pos = e;
        ++blocks;
    }
    if (blocks < 2) return false;

    pos = 0;
    while (pos < n) {
        size_t e = pos + run0;
        if (e > n) e = n;
        std::reverse(a + pos, a + e);
        pos = e;
    }
    for (size_t j = 1; j < n; ++j)
        if (a[j] < a[j - 1]) return false;
    return true;
}

} // namespace residual_reverse_segments
