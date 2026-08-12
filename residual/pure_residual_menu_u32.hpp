#pragma once
/*
 * pure_residual_menu_u32 — Wave 2 multi-type pure residual menu (uint32_t)
 * Menu: constant → FEW_WIDE → STRUCTURE → counting → consecutive_perm →
 *       push_middle → low_disorder → MSD HE
 * STRUCTURE v2: asc-first tight scan — closes timestamps u32 verify tax.
 * EXTERNAL-clean. Not field-level.
 * THE BEASTIE BOYZ 2026-08-12
 */
#include <cstdint>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include "residual_few_wide_u32.hpp"
#include "residual_consecutive_perm_u32.hpp"
#include "residual_push_middle_u32.hpp"
#include "residual_low_disorder_u32.hpp"

namespace residual_msd_u32 {
inline int residual_msd_u32(uint32_t *a, size_t n);
}

namespace pure_residual {

inline bool try_counting_u32(uint32_t *a, size_t n) {
    if (n < 2) return true;
    uint32_t amin = a[0], amax = a[0];
    for (size_t i = 1; i < n; ++i) {
        if (a[i] < amin) amin = a[i];
        if (a[i] > amax) amax = a[i];
    }
    uint64_t range = (uint64_t)amax - (uint64_t)amin;
    if (range >= (1ull << 20) || range >= (uint64_t)n) return false;
    if (range >= (uint64_t)(n * 3 / 4)) return false;
    size_t *cnt = (size_t *)std::calloc((size_t)range + 1, sizeof(size_t));
    if (!cnt) return false;
    for (size_t i = 0; i < n; ++i)
        cnt[(size_t)(a[i] - amin)]++;
    size_t p = 0;
    for (uint64_t v = 0; v <= range; ++v)
        for (size_t c = cnt[v]; c; --c)
            a[p++] = (uint32_t)(amin + (uint32_t)v);
    std::free(cnt);
    return true;
}

inline int sort_u32(uint32_t *a, size_t n) {
    if (n < 2) return 0;

    {
        const size_t S = n < 128 ? n : 128;
        size_t st = n / S; if (st < 1) st = 1;
        uint32_t v0 = a[0];
        bool maybe = true;
        for (size_t i = 0, c = 0; i < n && c < S; i += st, ++c)
            if (a[i] != v0) { maybe = false; break; }
        if (maybe) {
            bool all = true;
            for (size_t i = 1; i < n; ++i) if (a[i] != v0) { all = false; break; }
            if (all) return 0;
        }
    }

    if (n >= 64 && residual_few_wide_u32::should_try_few_wide(a, n))
        if (residual_few_wide_u32::residual_few_wide_u32(a, n)) return 0;

    // STRUCTURE v2: asc-first tight scan (closes timestamps u32 verify tax)
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

    if (residual_few_wide_u32::should_try_few_wide(a, n))
        if (residual_few_wide_u32::residual_few_wide_u32(a, n)) return 0;

    if (try_counting_u32(a, n)) return 0;

    if (residual_consecutive_perm_u32::try_consecutive_perm(a, n)) return 0;

    if (residual_push_middle_u32::try_push_middle(a, n)) return 0;

    if (residual_low_disorder_u32::should_try_low_disorder(a, n))
        if (residual_low_disorder_u32::residual_low_disorder_u32(a, n)) return 0;

    std::sort(a, a + n);
    return 0;
}

} // namespace pure_residual
