#pragma once
/*
 * residual_few_wide_i32 — FEW_WIDE pure residual for int32_t (Wave 2)
 *
 * Parallel specialization of residual_few_wide_i64 v2.5.
 * Target: k ≤ 16 (wide or dense) for int32_t. Value-preserving.
 * v2.5 gate: should_try accepts sample_u ≤ 4 independent of range.
 *
 * Protects i64 path: this is a separate specialization. Zero change to i64.
 * Pure residual only. EXTERNAL-clean. Fixed-size tables.
 * THE BEASTIE BOYZ — Wave 2 multi-type 2026-08-12
 */
#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <algorithm>
#include <cstddef>

namespace residual_few_wide_i32 {

static constexpr size_t KMAX = 16;
static constexpr size_t HCAP = 128;

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
    size_t c0 = 0, c1 = 0;
    for (size_t i = 0; i < n; ++i) {
        c0 += (size_t)(a[i] == v0);
        c1 += (size_t)(a[i] == v1);
    }
    if (c0 + c1 != n) return false;
    for (size_t i = 0; i < c0; ++i) a[i] = v0;
    for (size_t i = c0; i < n; ++i) a[i] = v1;
    return true;
}

inline size_t collect_uniques(const int32_t *a, size_t n, int32_t *uniq_out) {
    int32_t slot[HCAP];
    uint8_t used[HCAP] = {};
    size_t k = 0;
    for (size_t i = 0; i < n; ++i) {
        int32_t v = a[i];
        uint32_t h = mix32((uint32_t)v) & (HCAP - 1);
        for (;;) {
            if (!used[h]) {
                if (k >= KMAX) return 0;
                used[h] = 1;
                slot[h] = v;
                uniq_out[k++] = v;
                break;
            }
            if (slot[h] == v) break;
            h = (h + 1) & (HCAP - 1);
        }
    }
    return k;
}

inline void build_rank_map(const int32_t *uniq, size_t k,
                           int32_t *slot, uint8_t *used, uint8_t *rank_out) {
    std::memset(used, 0, HCAP);
    for (size_t r = 0; r < k; ++r) {
        uint32_t h = mix32((uint32_t)uniq[r]) & (HCAP - 1);
        while (used[h]) h = (h + 1) & (HCAP - 1);
        used[h] = 1;
        slot[h] = uniq[r];
        rank_out[h] = (uint8_t)r;
    }
}

inline uint8_t lookup_rank(int32_t v, const int32_t *slot, const uint8_t *used, const uint8_t *rank) {
    uint32_t h = mix32((uint32_t)v) & (HCAP - 1);
    while (used[h]) {
        if (slot[h] == v) return rank[h];
        h = (h + 1) & (HCAP - 1);
    }
    return 0;
}

inline bool residual_few_wide_i32(int32_t *a, size_t n) {
    if (n < 2) return true;

    // Sample for k≈2
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
        if (!more && two) {
            if (residual_two_value(a, n)) return true;
        }
        if (!more && !two) {
            bool all = true;
            for (size_t i = 1; i < n; ++i) {
                if (a[i] != s0) { all = false; break; }
            }
            if (all) return true;
        }
    }

    int32_t uniq[KMAX];
    size_t k = collect_uniques(a, n, uniq);
    if (k == 0) return false;
    if (k == 1) return true;
    if (k == 2) {
        if (residual_two_value(a, n)) return true;
        return false;
    }

    for (size_t i = 1; i < k; ++i) {
        int32_t key = uniq[i];
        size_t j = i;
        while (j > 0 && uniq[j - 1] > key) { uniq[j] = uniq[j - 1]; --j; }
        uniq[j] = key;
    }

    int32_t slot[HCAP];
    uint8_t used[HCAP], rank[HCAP];
    build_rank_map(uniq, k, slot, used, rank);

    size_t cnt[KMAX] = {};
    for (size_t i = 0; i < n; ++i)
        ++cnt[lookup_rank(a[i], slot, used, rank)];

    size_t p = 0;
    for (size_t r = 0; r < k; ++r) {
        int32_t v = uniq[r];
        for (size_t c = cnt[r]; c; --c)
            a[p++] = v;
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

    // Always accept very low cardinality (k≤4) regardless of range.
    if (sample_u <= 4) return true;

    // Mid-card (5..KMAX+2) only when range is wide.
    uint32_t srange = (uint32_t)(smax - smin);
    bool wide = (srange >= (1u << 20)) || (srange >= (uint32_t)(n * 3 / 4));
    if (!wide) return false;
    return sample_u <= KMAX + 2;
}

} // namespace residual_few_wide_i32
