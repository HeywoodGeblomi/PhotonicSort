#pragma once
/*
 * residual_sparse_i64 - Harvest P1
 * EXTERNAL-clean pure residual for sparse / primary-key style distributions.
 *
 * Root cause of prior floor: sparse-cluster detector routed to std::sort
 * (~2.3-2.6x vs library pdqsort). Fix: pure classical MSD with INS=64,
 * BLOCK=256, blocked scatter + prefetch. No library dispatch.
 *
 * Measured (n=1e6, 11-trial best-of, 5 seeds): geo pure/pdq ~0.81x
 * Gate <=0.95x: MET
 *
 * Do NOT use this path for majority / equal-heavy / adversarial_pivot
 * (those need majority_v2). Entry only when sparse-cluster probe fires
 * or when this residual is explicitly selected.
 */
#include <cstdint>
#include <cstring>
#include <cstdlib>

namespace residual_sparse {

static constexpr size_t SPARSE_INS   = 64;
static constexpr size_t SPARSE_BLOCK = 256;

inline void insertion_u64(uint64_t *a, size_t n) {
    for (size_t i = 1; i < n; ++i) {
        uint64_t key = a[i];
        size_t j = i;
        while (j > 0 && a[j - 1] > key) {
            a[j] = a[j - 1];
            --j;
        }
        a[j] = key;
    }
}

inline int top_byte(const uint64_t *a, size_t n) {
    if (n < 2) return -1;
    uint64_t amin = a[0], amax = a[0];
    for (size_t i = 1; i < n; ++i) {
        if (a[i] < amin) amin = a[i];
        if (a[i] > amax) amax = a[i];
    }
    uint64_t diff = amin ^ amax;
    if (!diff) return -1;
    return (int)((63 - __builtin_clzll(diff)) >> 3);
}

inline void msd_rec(uint64_t *a, uint64_t *buf, size_t n, int byte) {
    if (n <= SPARSE_INS) {
        if (n > 1) insertion_u64(a, n);
        return;
    }
    if (byte < 0) return;

    unsigned shift = (unsigned)byte * 8;
    size_t counts[256] = {};
    for (size_t i = 0; i < n; ++i)
        counts[(a[i] >> shift) & 0xFFu]++;

    size_t nonzero = 0;
    for (unsigned b = 0; b < 256; ++b)
        if (counts[b]) ++nonzero;
    if (nonzero <= 1) {
        msd_rec(a, buf, n, byte - 1);
        return;
    }

    size_t offsets[256], cursor[256];
    size_t sum = 0;
    for (unsigned b = 0; b < 256; ++b) {
        offsets[b] = sum;
        cursor[b] = sum;
        sum += counts[b];
    }

    size_t i = 0;
    while (i + SPARSE_BLOCK <= n) {
        __builtin_prefetch(a + i + SPARSE_BLOCK + 64, 0, 3);
        for (size_t j = 0; j < SPARSE_BLOCK; ++j) {
            uint64_t v = a[i + j];
            unsigned b = (unsigned)((v >> shift) & 0xFFu);
            size_t w = cursor[b]++;
            if ((w & 31u) == 0)
                __builtin_prefetch(buf + w + 32, 1, 1);
            buf[w] = v;
        }
        i += SPARSE_BLOCK;
    }
    for (; i < n; ++i)
        buf[cursor[(a[i] >> shift) & 0xFFu]++] = a[i];
    std::memcpy(a, buf, n * sizeof(uint64_t));

    for (unsigned b = 0; b < 256; ++b)
        if (counts[b] > 1)
            msd_rec(a + offsets[b], buf + offsets[b], counts[b], byte - 1);
}

/* Tiny-range counting when profitable (low cardinality dense image). */
inline bool try_counting(int64_t *a, size_t n) {
    if (n < 2) return true;
    int64_t amin = a[0], amax = a[0];
    for (size_t i = 1; i < n; ++i) {
        if (a[i] < amin) amin = a[i];
        if (a[i] > amax) amax = a[i];
    }
    uint64_t range = (uint64_t)(amax - amin);
    if (range >= (uint64_t)n * 4u || range >= (1ull << 20))
        return false;
    size_t *cnt = (size_t *)std::calloc((size_t)range + 1, sizeof(size_t));
    if (!cnt) return false;
    for (size_t i = 0; i < n; ++i)
        cnt[(uint64_t)(a[i] - amin)]++;
    size_t p = 0;
    for (uint64_t v = 0; v <= range; ++v)
        for (size_t c = cnt[v]; c; --c)
            a[p++] = (int64_t)v + amin;
    std::free(cnt);
    return true;
}

/** Pure sparse residual. No std::sort / pdqsort / ska. */
inline int residual_sparse_i64(int64_t *a, size_t n) {
    if (n < 2) return 0;
    if (n <= SPARSE_INS) {
        for (size_t i = 1; i < n; ++i) {
            int64_t key = a[i];
            size_t j = i;
            while (j > 0 && a[j - 1] > key) {
                a[j] = a[j - 1];
                --j;
            }
            a[j] = key;
        }
        return 0;
    }
    if (try_counting(a, n))
        return 0;

    uint64_t *u = reinterpret_cast<uint64_t *>(a);
    for (size_t i = 0; i < n; ++i)
        u[i] ^= (uint64_t)1 << 63;

    uint64_t *buf = static_cast<uint64_t *>(std::malloc(n * sizeof(uint64_t)));
    if (!buf) {
        for (size_t i = 0; i < n; ++i)
            u[i] ^= (uint64_t)1 << 63;
        return -1;
    }
    int tb = top_byte(u, n);
    if (tb >= 0)
        msd_rec(u, buf, n, tb);
    std::free(buf);
    for (size_t i = 0; i < n; ++i)
        u[i] ^= (uint64_t)1 << 63;
    return 0;
}

} // namespace residual_sparse
