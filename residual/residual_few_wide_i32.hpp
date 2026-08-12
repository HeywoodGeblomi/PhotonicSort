#pragma once
/*
 * residual_few_wide_i32 — FEW_WIDE pure residual for int32_t
 * v2.6.1: single-pass open-address hash (key+count), then sort-by-key writeback.
 * Target: k ≤ 16 wide or dense. Value-preserving.
 * EXTERNAL-clean. Not field-level.
 * THE BEASTIE BOYZ — soft-spot kill few_k16_wide 2026-08-12
 */
#include <cstdint>
#include <cstring>
#include <algorithm>
#include <cstddef>

namespace residual_few_wide_i32 {

static constexpr size_t KMAX = 16;
static constexpr size_t HCAP = 64;

inline uint32_t mix32(uint32_t x) {
    x ^= x >> 16; x *= 0x7feb352dU;
    x ^= x >> 15; x *= 0x846ca68bU;
    x ^= x >> 16;
    return x;
}

inline bool residual_two_value(int32_t *a, size_t n) {
    if (n < 2) return true;
    int32_t v0 = a[0], v1 = v0;
    for (size_t i = 1; i < n; ++i) {
        if (a[i] != v0) { v1 = a[i]; break; }
    }
    if (v0 == v1) return true;
    if (v0 > v1) std::swap(v0, v1);
    size_t c0 = 0;
    for (size_t i = 0; i < n; ++i) {
        if (a[i] == v0) ++c0;
        else if (a[i] != v1) return false;
    }
    for (size_t i = 0; i < c0; ++i) a[i] = v0;
    for (size_t i = c0; i < n; ++i) a[i] = v1;
    return true;
}

inline bool residual_few_wide_i32(int32_t *a, size_t n) {
    if (n < 2) return true;

    {
        size_t S = n < 64 ? n : 64;
        size_t st = n / S; if (st < 1) st = 1;
        int32_t s0 = a[0], s1 = s0;
        bool two = false, more = false;
        for (size_t i = 0; i < n && !more; i += st) {
            int32_t v = a[i];
            if (v != s0) {
                if (!two) { s1 = v; two = true; }
                else if (v != s1) more = true;
            }
        }
        if (!more) {
            if (!two) {
                for (size_t i = 1; i < n; ++i)
                    if (a[i] != s0) return residual_two_value(a, n);
                return true;
            }
            return residual_two_value(a, n);
        }
    }

    int32_t keys[HCAP];
    size_t counts[HCAP];
    uint8_t used[HCAP] = {};
    size_t k = 0;

    for (size_t i = 0; i < n; ++i) {
        int32_t v = a[i];
        uint32_t h = mix32((uint32_t)v) & (HCAP - 1);
        for (;;) {
            if (!used[h]) {
                if (k >= KMAX) return false;
                used[h] = 1;
                keys[h] = v;
                counts[h] = 1;
                ++k;
                break;
            }
            if (keys[h] == v) {
                ++counts[h];
                break;
            }
            h = (h + 1) & (HCAP - 1);
        }
    }

    int32_t uniq[KMAX];
    size_t cnt[KMAX];
    size_t u = 0;
    for (size_t h = 0; h < HCAP; ++h) {
        if (!used[h]) continue;
        int32_t key = keys[h];
        size_t c = counts[h];
        size_t j = u;
        while (j > 0 && uniq[j - 1] > key) {
            uniq[j] = uniq[j - 1];
            cnt[j] = cnt[j - 1];
            --j;
        }
        uniq[j] = key;
        cnt[j] = c;
        ++u;
    }

    size_t p = 0;
    for (size_t r = 0; r < u; ++r) {
        int32_t v = uniq[r];
        size_t c = cnt[r];
        while (c >= 8) {
            a[p]=v;a[p+1]=v;a[p+2]=v;a[p+3]=v;
            a[p+4]=v;a[p+5]=v;a[p+6]=v;a[p+7]=v;
            p += 8; c -= 8;
        }
        while (c--) a[p++] = v;
    }
    return true;
}

inline bool should_try_few_wide(const int32_t *a, size_t n) {
    if (n < 2) return false;
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
    std::sort(sample_vals, sample_vals + ns);
    size_t sample_u = 1;
    for (size_t i = 1; i < ns; ++i)
        if (sample_vals[i] != sample_vals[i - 1]) ++sample_u;

    if (sample_u <= 4) return true;

    uint32_t srange = (uint32_t)(smax - smin);
    bool wide = (srange >= (1u << 20)) || (srange >= (uint32_t)(n * 3 / 4));
    if (!wide) return false;
    return sample_u <= KMAX + 2;
}

} // namespace residual_few_wide_i32
