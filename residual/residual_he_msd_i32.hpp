#pragma once
/*
 * residual_he_msd_i32 — classical MSD residual for int32 high-entropy
 * Adapted from residual_he_msd_i64. Fixed 8-bit digits, 4-byte keys.
 * Sign-bit flip for unsigned radix order.
 * EXTERNAL-clean. THE BEASTIE BOYZ — Wave 2 multi-type Phase A 2026-08-12
 */
#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <algorithm>

namespace residual_he_i32 {

inline void insertion_u32(uint32_t *a, size_t n) {
    for (size_t i = 1; i < n; ++i) {
        uint32_t key = a[i];
        size_t j = i;
        while (j > 0 && a[j - 1] > key) {
            a[j] = a[j - 1];
            --j;
        }
        a[j] = key;
    }
}

inline int top_byte(const uint32_t *a, size_t n) {
    uint32_t amin = a[0], amax = a[0];
    for (size_t i = 1; i < n; ++i) {
        if (a[i] < amin) amin = a[i];
        if (a[i] > amax) amax = a[i];
    }
    uint32_t diff = amin ^ amax;
    if (diff == 0) return -1;
    return (int)((31 - __builtin_clz(diff)) >> 3);
}

inline void msd_rec(uint32_t *a, uint32_t *buf, size_t n, int byte) {
    if (n < 32) { insertion_u32(a, n); return; }
    if (byte < 0) return;
    unsigned shift = (unsigned)byte * 8;
    size_t counts[256] = {};
    for (size_t i = 0; i < n; ++i)
        counts[(a[i] >> shift) & 0xFFu]++;
    size_t offsets[256];
    size_t sum = 0;
    for (int b = 0; b < 256; ++b) {
        offsets[b] = sum;
        sum += counts[b];
    }
    int nonempty = 0;
    for (int b = 0; b < 256; ++b) if (counts[b]) ++nonempty;
    if (nonempty == 1) {
        msd_rec(a, buf, n, byte - 1);
        return;
    }
    size_t cursor[256];
    for (int b = 0; b < 256; ++b) cursor[b] = offsets[b];
    for (size_t i = 0; i < n; ++i) {
        uint32_t v = a[i];
        unsigned b = (unsigned)((v >> shift) & 0xFFu);
        buf[cursor[b]++] = v;
    }
    std::memcpy(a, buf, n * sizeof(uint32_t));
    for (int b = 0; b < 256; ++b) {
        if (counts[b] > 1)
            msd_rec(a + offsets[b], buf + offsets[b], counts[b], byte - 1);
    }
}

inline int residual_he_msd_i32(int32_t *a, size_t n) {
    if (n < 2) return 0;
    if (n < 32) {
        for (size_t i = 1; i < n; ++i) {
            int32_t key = a[i];
            size_t j = i;
            while (j > 0 && a[j - 1] > key) { a[j] = a[j - 1]; --j; }
            a[j] = key;
        }
        return 0;
    }
    uint32_t *u = reinterpret_cast<uint32_t *>(a);
    for (size_t i = 0; i < n; ++i)
        u[i] ^= (uint32_t)1 << 31;
    uint32_t *buf = static_cast<uint32_t *>(std::malloc(n * sizeof(uint32_t)));
    if (!buf) {
        for (size_t i = 0; i < n; ++i) u[i] ^= (uint32_t)1 << 31;
        std::sort(a, a + n);
        return 0;
    }
    int tb = top_byte(u, n);
    if (tb >= 0) msd_rec(u, buf, n, tb);
    else insertion_u32(u, n);
    std::free(buf);
    for (size_t i = 0; i < n; ++i)
        u[i] ^= (uint32_t)1 << 31;
    return 0;
}

} // namespace residual_he_i32
