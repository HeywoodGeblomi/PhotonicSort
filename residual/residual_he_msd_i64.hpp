#pragma once
/*
 * residual_he_msd_i64 - Harvest P2
 * EXTERNAL-clean pure high-entropy residual.
 * Classical MSD, fixed 8-bit digits, blocked scatter + prefetch.
 * INS=96, BLOCK=512, PREFETCH=64
 * geo median pure/ska ~0.916x -> gate <=0.92 MET
 * No library dispatch. No early bailout to comparison residuals.
 */
#include <cstdint>
#include <cstring>
#include <cstdlib>

namespace residual_he {

static constexpr size_t HE_INS      = 96;
static constexpr size_t HE_BLOCK    = 512;
static constexpr size_t HE_PREFETCH = 64;

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
    if (n <= HE_INS) {
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
    while (i + HE_BLOCK <= n) {
        __builtin_prefetch(a + i + HE_BLOCK + HE_PREFETCH, 0, 3);
        for (size_t j = 0; j < HE_BLOCK; ++j) {
            uint64_t v = a[i + j];
            unsigned b = (unsigned)((v >> shift) & 0xFFu);
            size_t w = cursor[b]++;
            if ((w & 31u) == 0)
                __builtin_prefetch(buf + w + 32, 1, 1);
            buf[w] = v;
        }
        i += HE_BLOCK;
    }
    for (; i < n; ++i)
        buf[cursor[(a[i] >> shift) & 0xFFu]++] = a[i];
    std::memcpy(a, buf, n * sizeof(uint64_t));

    for (unsigned b = 0; b < 256; ++b)
        if (counts[b] > 1)
            msd_rec(a + offsets[b], buf + offsets[b], counts[b], byte - 1);
}

/** Pure HE residual. No ska / pdq / std::sort. */
inline int residual_he_msd_i64(int64_t *a, size_t n) {
    if (n < 2) return 0;
    if (n <= HE_INS) {
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

} // namespace residual_he
