#pragma once
/*
 * pure_residual_menu_u32 — minimal Wave 2 uint32 pure residual entry
 *
 * Protects i64 path completely. Parallel specialization.
 * Phase A: FEW_WIDE residual + STRUCTURE + temporary std::sort fallback.
 * Expand residual coverage in follow-ups.
 *
 * EXTERNAL-clean. Not field-level.
 * THE BEASTIE BOYZ — Wave 2 multi-type 2026-08-12
 */
#include <cstdint>
#include <algorithm>
#include "residual_few_wide_u32.hpp"

namespace pure_residual {

/** Minimal uint32 pure residual entry. Returns 0 on success. */
inline int sort_u32(uint32_t *a, size_t n) {
    if (n < 2) return 0;

    // Constant probe
    {
        const size_t S = n < 128 ? n : 128;
        size_t st = n / S; if (st < 1) st = 1;
        uint32_t v0 = a[0];
        bool maybe = true;
        for (size_t i = 0, c = 0; i < n && c < S; i += st, ++c) {
            if (a[i] != v0) { maybe = false; break; }
        }
        if (maybe) {
            bool all = true;
            for (size_t i = 1; i < n; ++i) {
                if (a[i] != v0) { all = false; break; }
            }
            if (all) return 0;
        }
    }

    // Early FEW_WIDE
    if (n >= 64 && residual_few_wide_u32::should_try_few_wide(a, n)) {
        if (residual_few_wide_u32::residual_few_wide_u32(a, n)) return 0;
    }

    // STRUCTURE
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

    if (residual_few_wide_u32::should_try_few_wide(a, n)) {
        if (residual_few_wide_u32::residual_few_wide_u32(a, n)) return 0;
    }

    // Temporary fallback
    std::sort(a, a + n);
    return 0;
}

} // namespace pure_residual
