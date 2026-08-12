#pragma once
/*
 * pure_residual_menu_u32 — Wave 2 uint32 pure residual entry (expanded + low_disorder)
 *
 * Protects i64 path completely. Parallel specialization.
 * Menu: constant → FEW_WIDE → STRUCTURE → counting → low_disorder → MSD HE
 * No std::sort fallback on specialized residual paths (except gated low_disorder body).
 *
 * EXTERNAL-clean. Not field-level. Sequential POD uint32 only.
 * THE BEASTIE BOYZ — Wave 2 multi-type residual polish 2026-08-12
 */
#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <algorithm>
#include "residual_few_wide_u32.hpp"
#include "residual_low_disorder_u32.hpp"

namespace pure_residual {

namespace residual_msd_u32 {

inline void msd_pass(uint32_t *a, size_t n, int shift, uint32_t *tmp) {
    if (n < 64 || shift < 0) {
        for (size_t i = 1; i < n; ++i) {
            uint32_t key = a[i]; size_t j = i;
            while (j > 0 && a[j-1] > key) { a[j] = a[j-1]; --j; }
            a[j] = key;
        }
        return;
    }
    size_t cnt[256] = {};
    for (size_t i = 0; i < n; ++i)
        cnt[(a[i] >> shift) & 0xff]++;
    size_t sum = 0;
    size_t offsets[256];
    for (int i = 0; i < 256; ++i) { offsets[i] = sum; sum += cnt[i]; }
    for (size_t i = 0; i < n; ++i) {
        uint32_t key = a[i];
        tmp[offsets[(key >> shift) & 0xff]++] = key;
    }
    std::memcpy(a, tmp, n * sizeof(uint32_t));
    size_t pos = 0;
    for (int i = 0; i < 256; ++i) {
        if (cnt[i] > 1) msd_pass(a + pos, cnt[i], shift - 8, tmp);
        pos += cnt[i];
    }
}

inline int residual_msd_u32(uint32_t *a, size_t n) {
    if (n < 2) return 0;
    uint32_t *tmp = (uint32_t *)std::malloc(n * sizeof(uint32_t));
    if (!tmp) {
        for (size_t i = 1; i < n; ++i) {
            uint32_t key = a[i]; size_t j = i;
            while (j > 0 && a[j-1] > key) { a[j] = a[j-1]; --j; }
            a[j] = key;
        }
        return 0;
    }
    msd_pass(a, n, 24, tmp);
    std::free(tmp);
    return 0;
}

} // namespace residual_msd_u32

inline bool try_counting_u32(uint32_t *a, size_t n) {
    if (n < 2) return true;
    size_t S = n < 4096 ? n : 4096;
    size_t st = n / S; if (st < 1) st = 1;
    uint32_t sample_vals[4096];
    size_t ns = 0;
    uint32_t smin = a[0], smax = a[0];
    for (size_t i = 0; i < n && ns < S; i += st) {
        uint32_t v = a[i];
        sample_vals[ns++] = v;
        if (v < smin) smin = v;
        if (v > smax) smax = v;
    }
    uint64_t srange = (uint64_t)smax - (uint64_t)smin;
    if (srange >= (1ull << 20)) return false;
    if (srange >= (uint64_t)(n * 3 / 4)) return false;

    std::sort(sample_vals, sample_vals + ns);
    size_t sample_u = 1;
    for (size_t i = 1; i < ns; ++i)
        if (sample_vals[i] != sample_vals[i - 1]) ++sample_u;
    if (sample_u > 4096) return false;
    if (sample_u > 1024 && srange > 262144ull) return false;

    uint32_t amin = smin, amax = smax;
    for (size_t i = 0; i < n; ++i) {
        if (a[i] < amin) amin = a[i];
        if (a[i] > amax) amax = a[i];
    }
    if (amin == amax) return true;
    uint64_t range = (uint64_t)amax - (uint64_t)amin;
    if (range >= (1ull << 20)) return false;
    if (range >= (uint64_t)n) return false;
    if (range >= (uint64_t)(n * 3 / 4)) return false;

    size_t *cnt = (size_t *)std::calloc((size_t)range + 1, sizeof(size_t));
    if (!cnt) return false;
    for (size_t i = 0; i < n; ++i)
        cnt[(size_t)((uint64_t)a[i] - (uint64_t)amin)]++;
    size_t p = 0;
    for (uint64_t v = 0; v <= range; ++v)
        for (size_t c = cnt[v]; c; --c)
            a[p++] = (uint32_t)(v + (uint64_t)amin);
    std::free(cnt);
    return true;
}

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

    // Second FEW_WIDE
    if (residual_few_wide_u32::should_try_few_wide(a, n)) {
        if (residual_few_wide_u32::residual_few_wide_u32(a, n)) return 0;
    }

    // Counting
    if (try_counting_u32(a, n)) return 0;

    // Low-disorder (almost_sorted / near-monotonic / light db_pk)
    if (residual_low_disorder_u32::should_try_low_disorder(a, n)) {
        if (residual_low_disorder_u32::residual_low_disorder_u32(a, n)) return 0;
    }

    // HE residual (32-bit MSD radix) — no std::sort fallback
    return residual_msd_u32::residual_msd_u32(a, n);
}

} // namespace pure_residual
