#pragma once
/*
 * pure_residual_menu_i32 — Wave 2 int32 pure residual entry
 *
 * Menu: constant → FEW_WIDE → STRUCTURE → late high-disorder short-circuit →
 *       counting → consecutive_perm → push_middle → low_disorder → residual_pdqsort HE
 *
 * pipe_sparse kill: late inv≥0.5 → residual_pdqsort; HE fallback residual_pdqsort (full orlp).
 * EXTERNAL-clean. Not field-level. i64 path protected.
 * THE BEASTIE BOYZ — pipe_sparse kill 2026-08-12
 */
#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <algorithm>
#include "residual_few_wide_i32.hpp"
#include "residual_consecutive_perm_i32.hpp"
#include "residual_push_middle_i32.hpp"
#include "residual_low_disorder_i32.hpp"
#include "pdqsort_residual.h"

namespace pure_residual {

namespace residual_msd_i32 {

inline void msd_pass(int32_t *a, size_t n, int shift, int32_t *tmp) {
    if (n < 64 || shift < 0) {
        for (size_t i = 1; i < n; ++i) {
            int32_t key = a[i]; size_t j = i;
            while (j > 0 && a[j-1] > key) { a[j] = a[j-1]; --j; }
            a[j] = key;
        }
        return;
    }
    size_t cnt[256] = {};
    for (size_t i = 0; i < n; ++i) {
        uint32_t key = (uint32_t)a[i] ^ 0x80000000u;
        cnt[(key >> shift) & 0xff]++;
    }
    size_t sum = 0;
    size_t offsets[256];
    for (int i = 0; i < 256; ++i) { offsets[i] = sum; sum += cnt[i]; }
    for (size_t i = 0; i < n; ++i) {
        uint32_t key = (uint32_t)a[i] ^ 0x80000000u;
        tmp[offsets[(key >> shift) & 0xff]++] = a[i];
    }
    std::memcpy(a, tmp, n * sizeof(int32_t));
    size_t pos = 0;
    for (int i = 0; i < 256; ++i) {
        if (cnt[i] > 1) msd_pass(a + pos, cnt[i], shift - 8, tmp);
        pos += cnt[i];
    }
}

inline int residual_msd_i32(int32_t *a, size_t n) {
    if (n < 2) return 0;
    int32_t *tmp = (int32_t *)std::malloc(n * sizeof(int32_t));
    if (!tmp) { std::sort(a, a + n); return 0; }
    msd_pass(a, n, 24, tmp);
    std::free(tmp);
    return 0;
}

} // namespace residual_msd_i32

inline bool try_counting_i32(int32_t *a, size_t n) {
    if (n < 2) return true;
    int32_t amin = a[0], amax = a[0];
    for (size_t i = 1; i < n; ++i) {
        if (a[i] < amin) amin = a[i];
        if (a[i] > amax) amax = a[i];
    }
    if (amin == amax) return true;
    uint64_t range = (uint64_t)((int64_t)amax - (int64_t)amin);
    if (range >= (1ull << 20) || range >= (uint64_t)n) return false;
    if (range >= (uint64_t)(n * 3 / 4)) return false;
    size_t *cnt = (size_t *)std::calloc((size_t)range + 1, sizeof(size_t));
    if (!cnt) return false;
    for (size_t i = 0; i < n; ++i)
        cnt[(size_t)((int64_t)a[i] - (int64_t)amin)]++;
    size_t p = 0;
    for (uint64_t v = 0; v <= range; ++v)
        for (size_t c = cnt[v]; c; --c)
            a[p++] = (int32_t)((int64_t)v + (int64_t)amin);
    std::free(cnt);
    return true;
}

inline int sort_i32(int32_t *a, size_t n) {
    if (n < 2) return 0;

    {
        const size_t S = n < 128 ? n : 128;
        size_t st = n / S; if (st < 1) st = 1;
        int32_t v0 = a[0];
        bool maybe = true;
        for (size_t i = 0, c = 0; i < n && c < S; i += st, ++c)
            if (a[i] != v0) { maybe = false; break; }
        if (maybe) {
            bool all = true;
            for (size_t i = 1; i < n; ++i) if (a[i] != v0) { all = false; break; }
            if (all) return 0;
        }
    }

    if (n >= 64 && residual_few_wide_i32::should_try_few_wide(a, n))
        if (residual_few_wide_i32::residual_few_wide_i32(a, n)) return 0;

    // STRUCTURE v2: asc-first tight scan
    {
        bool asc = true;
        for (size_t i = 1; i < n; ++i) {
            if (a[i] < a[i - 1]) { asc = false; break; }
        }
        if (asc) return 0;
    }
    {
        bool desc = true;
        for (size_t i = 1; i < n; ++i) {
            if (a[i] > a[i - 1]) { desc = false; break; }
        }
        if (desc) { std::reverse(a, a + n); return 0; }
    }

    // Late high-disorder short-circuit (after STRUCTURE): skip specialized probes
    // Closes pipe_sparse without stealing organpipe structured paths.
    if (n >= 256) {
        const size_t S = 512;
        size_t inv = 0;
        for (size_t c = 0; c < S; ++c) {
            size_t i = 1 + (c * (n - 1)) / S;
            if (a[i] < a[i - 1]) ++inv;
        }
        if (inv * 2 >= S) {
            residual_pdqsort(a, a + n);
            return 0;
        }
    }

    if (residual_few_wide_i32::should_try_few_wide(a, n))
        if (residual_few_wide_i32::residual_few_wide_i32(a, n)) return 0;

    if (try_counting_i32(a, n)) return 0;

    if (residual_consecutive_perm_i32::try_consecutive_perm(a, n)) return 0;

    if (residual_push_middle_i32::try_push_middle(a, n)) return 0;

    if (residual_low_disorder_i32::should_try_low_disorder(a, n))
        if (residual_low_disorder_i32::residual_low_disorder_i32(a, n)) return 0;

    // HE fallback: residual_pdqsort (closes pipe_sparse; full orlp quality)
    residual_pdqsort(a, a + n);
    return 0;
}

} // namespace pure_residual
