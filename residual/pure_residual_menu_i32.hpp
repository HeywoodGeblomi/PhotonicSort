#pragma once
/*
 * pure_residual_menu_i32 — Wave 2 int32 pure residual entry (expanded + low_disorder)
 *
 * Protects i64 path completely. Parallel specialization.
 * Menu: constant → FEW_WIDE → STRUCTURE → counting → low_disorder → MSD HE
 *
 * EXTERNAL-clean. Not field-level.
 * THE BEASTIE BOYZ — Wave 2 multi-type residual polish 2026-08-12
 */
#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <algorithm>
#include "residual_few_wide_i32.hpp"
#include "residual_low_disorder_i32.hpp"

namespace pure_residual {

namespace residual_he_i32 {

inline void insertion(int32_t *a, size_t n) {
    for (size_t i = 1; i < n; ++i) {
        int32_t key = a[i];
        size_t j = i;
        while (j > 0 && a[j - 1] > key) { a[j] = a[j - 1]; --j; }
        a[j] = key;
    }
}

inline void heapsort(int32_t *a, size_t n) {
    auto sift = [&](size_t i, size_t n_) {
        for (;;) {
            size_t l = 2 * i + 1;
            if (l >= n_) break;
            size_t r = l + 1;
            size_t m = (r < n_ && a[r] > a[l]) ? r : l;
            if (a[i] >= a[m]) break;
            std::swap(a[i], a[m]);
            i = m;
        }
    };
    for (size_t i = n / 2; i-- > 0;) sift(i, n);
    for (size_t i = n; i-- > 1;) {
        std::swap(a[0], a[i]);
        sift(0, i);
    }
}

// Branchless 3-way partition with ninther
inline void he_introsort(int32_t *a, size_t n, int depth) {
    while (n > 32) {
        if (depth <= 0) { heapsort(a, n); return; }
        --depth;

        // ninther pivot
        size_t s = n / 3;
        auto med3 = [](int32_t x, int32_t y, int32_t z) -> int32_t {
            if (x < y) { if (y < z) return y; return x < z ? z : x; }
            if (x < z) return x; return y < z ? z : y;
        };
        int32_t pivot = med3(
            med3(a[0], a[s/2], a[s]),
            med3(a[s], a[s+s/2], a[2*s]),
            med3(a[2*s], a[2*s+s/2], a[n-1]));

        // Dutch-national-flag 3-way
        size_t lo = 0, mid = 0, hi = n;
        while (mid < hi) {
            if (a[mid] < pivot) { std::swap(a[lo], a[mid]); ++lo; ++mid; }
            else if (a[mid] > pivot) { --hi; std::swap(a[mid], a[hi]); }
            else ++mid;
        }
        he_introsort(a, lo, depth);
        a += mid;
        n -= mid;
    }
    insertion(a, n);
}

inline int residual_he_i32(int32_t *a, size_t n) {
    int depth = 2;
    for (size_t m = n; m > 1; m >>= 1) depth += 2;
    he_introsort(a, n, depth);
    return 0;
}

} // namespace residual_he_i32

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
        uint32_t key = (uint32_t)a[i] ^ 0x80000000u; // signed order
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
    if (!tmp) {
        for (size_t i = 1; i < n; ++i) {
            int32_t key = a[i]; size_t j = i;
            while (j > 0 && a[j-1] > key) { a[j] = a[j-1]; --j; }
            a[j] = key;
        }
        return 0;
    }
    msd_pass(a, n, 24, tmp);
    std::free(tmp);
    return 0;
}

} // namespace residual_msd_i32

/** Counting residual for compact-range int32. */
inline bool try_counting_i32(int32_t *a, size_t n) {
    if (n < 2) return true;
    size_t S = n < 4096 ? n : 4096;
    size_t st = n / S; if (st < 1) st = 1;
    int32_t sample_vals[4096];
    size_t ns = 0;
    int32_t smin = a[0], smax = a[0];
    for (size_t i = 0; i < n && ns < S; i += st) {
        int32_t v = a[i];
        sample_vals[ns++] = v;
        if (v < smin) smin = v;
        if (v > smax) smax = v;
    }
    uint32_t srange = (uint32_t)(smax - smin);
    if (srange >= (1u << 20)) return false;
    if (srange >= (uint32_t)(n * 3 / 4)) return false;

    std::sort(sample_vals, sample_vals + ns);
    size_t sample_u = 1;
    for (size_t i = 1; i < ns; ++i)
        if (sample_vals[i] != sample_vals[i - 1]) ++sample_u;
    if (sample_u > 4096) return false;
    if (sample_u > 1024 && srange > 262144u) return false;

    int32_t amin = smin, amax = smax;
    for (size_t i = 0; i < n; ++i) {
        if (a[i] < amin) amin = a[i];
        if (a[i] > amax) amax = a[i];
    }
    if (amin == amax) return true;
    uint32_t range = (uint32_t)(amax - amin);
    if (range >= (1u << 20)) return false;
    if (range >= (uint32_t)n) return false;
    if (range >= (uint32_t)(n * 3 / 4)) return false;

    size_t *cnt = (size_t *)std::calloc((size_t)range + 1, sizeof(size_t));
    if (!cnt) return false;
    for (size_t i = 0; i < n; ++i)
        cnt[(uint32_t)(a[i] - amin)]++;
    size_t p = 0;
    for (uint32_t v = 0; v <= range; ++v)
        for (size_t c = cnt[v]; c; --c)
            a[p++] = (int32_t)v + amin;
    std::free(cnt);
    return true;
}

/** Unified int32 pure residual entry. Returns 0 on success. */
inline int sort_i32(int32_t *a, size_t n) {
    if (n < 2) return 0;

    // Constant probe
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
            for (size_t i = 1; i < n; ++i) {
                if (a[i] != v0) { all = false; break; }
            }
            if (all) return 0;
        }
    }

    // Early FEW_WIDE (including non-wide k≤4)
    if (n >= 64 && residual_few_wide_i32::should_try_few_wide(a, n)) {
        if (residual_few_wide_i32::residual_few_wide_i32(a, n)) return 0;
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

    // Second FEW_WIDE attempt
    if (residual_few_wide_i32::should_try_few_wide(a, n)) {
        if (residual_few_wide_i32::residual_few_wide_i32(a, n)) return 0;
    }

    // Counting (compact range)
    if (try_counting_i32(a, n)) return 0;

    // Low-disorder (almost_sorted / near-monotonic / light db_pk)
    if (residual_low_disorder_i32::should_try_low_disorder(a, n)) {
        if (residual_low_disorder_i32::residual_low_disorder_i32(a, n)) return 0;
    }

    // HE residual (32-bit MSD radix) — no std::sort fallback
    return residual_msd_i32::residual_msd_i32(a, n);
}

} // namespace pure_residual
