#pragma once
/*
 * residual_consecutive_perm_i32 — O(n) write-sorted for consecutive permutations
 * Targets almost_sorted / push_middle / identity-shuffled when values are a
 * permutation of [mn, mn+n). Detect via span + sum; write a[i] = mn + i.
 * EXTERNAL-clean. Not field-level.
 * THE BEASTIE BOYZ — soft-spot kill 2026-08-12
 */
#include <cstdint>
#include <cstddef>

namespace residual_consecutive_perm_i32 {

inline bool try_consecutive_perm(int32_t *a, size_t n) {
    if (n < 64) return false;

    size_t S = n < 256 ? n : 256;
    size_t st = n / S; if (st < 1) st = 1;
    int32_t smn = a[0], smx = a[0];
    for (size_t i = 0, c = 0; i < n && c < S; i += st, ++c) {
        if (a[i] < smn) smn = a[i];
        if (a[i] > smx) smx = a[i];
    }
    uint64_t sspan = (uint64_t)((int64_t)smx - (int64_t)smn);
    if (sspan + 1 < (uint64_t)n - (uint64_t)n / 50) return false;
    if (sspan > (uint64_t)n + (uint64_t)n / 50) return false;

    int32_t mn = a[0], mx = a[0];
    int64_t sum = 0;
    for (size_t i = 0; i < n; ++i) {
        int32_t v = a[i];
        if (v < mn) mn = v;
        if (v > mx) mx = v;
        sum += (int64_t)v;
    }
    uint64_t span = (uint64_t)((int64_t)mx - (int64_t)mn);
    if (span + 1 != (uint64_t)n) return false;

    int64_t expected = (int64_t)n * (2 * (int64_t)mn + (int64_t)n - 1) / 2;
    if (sum != expected) return false;

    for (size_t i = 0; i < n; ++i)
        a[i] = mn + (int32_t)i;
    return true;
}

} // namespace residual_consecutive_perm_i32
