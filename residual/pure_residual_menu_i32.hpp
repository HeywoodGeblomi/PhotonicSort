#pragma once
/*
 * pure_residual_menu_i32 — Wave 2 Phase A pure residual entry for int32
 *
 * Protects i64 excellence: this is a parallel specialization only.
 * Menu:
 *   constant probe → FEW_WIDE (incl. non-wide low-card) → STRUCTURE →
 *   counting (compact range) → low_disorder → HE MSD
 *
 * EXTERNAL-clean. Not field-level. Sequential POD int32 only.
 * THE BEASTIE BOYZ — Wave 2 multi-type Phase A 2026-08-12
 */
#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <algorithm>
#include "residual_few_wide_i32.hpp"
#include "residual_low_disorder_i32.hpp"
#include "residual_he_msd_i32.hpp"

namespace pure_residual {

inline bool try_counting_i32(int32_t *a, size_t n) {
    if (n < 2) return true;
    size_t S = n < 4096 ? n : 4096;
    size_t st = n / S; if (st < 1) st = 1;
    int32_t smin = a[0], smax = a[0];
    for (size_t i = 0; i < n && S; i += st, --S) {
        if (a[i] < smin) smin = a[i];
        if (a[i] > smax) smax = a[i];
    }
    int32_t amin = smin, amax = smax;
    for (size_t i = 0; i < n; ++i) {
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

/** Unified pure residual entry for int32. Returns 0 on success. */
inline int sort_i32(int32_t *a, size_t n) {
    if (n < 2) return 0;

    // Constant / all-equal probe
    {
        const size_t S = n < 128 ? n : 128;
        size_t st = n / S; if (st < 1) st = 1;
        int32_t v0 = a[0];
        bool maybe = true;
        for (size_t i = 0, c = 0; i < n && c < S; i += st, ++c) {
            if (a[i] != v0) { maybe = false; break; }
        }
        if (maybe) {
            bool all = true;
            for (size_t i = 1; i < n; ++i) if (a[i] != v0) { all = false; break; }
            if (all) return 0;
        }
    }

    // FEW_WIDE (includes non-wide low-card via sample_u ≤ 4 gate)
    if (n >= 64 && residual_few_wide_i32::should_try_few_wide(a, n)) {
        if (residual_few_wide_i32::residual_few_wide_i32(a, n)) return 0;
    }

    // STRUCTURE (sorted / reverse)
    {
        bool asc = true, desc = true;
        for (size_t i = 1; i < n; ++i) {
            if (a[i] < a[i - 1]) asc = false;
            if (a[i] > a[i - 1]) desc = false;
            if (!asc && !desc) break;
        }
        if (asc) return 0;
        if (desc) { std::reverse(a, a + n); return 0; }
    }

    // Counting (compact range)
    if (try_counting_i32(a, n)) return 0;

    // Low-disorder
    if (residual_low_disorder_i32::should_try_low_disorder(a, n)) {
        if (residual_low_disorder_i32::residual_low_disorder_i32(a, n)) return 0;
    }

    // HE residual (classical MSD for int32)
    return residual_he_i32::residual_he_msd_i32(a, n);
}

} // namespace pure_residual
