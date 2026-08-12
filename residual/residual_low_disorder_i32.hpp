#pragma once
/*
 * residual_low_disorder_i32 — low-disorder residual with pdq-quality body
 *
 * Gate: sample inv-ratio + cardinality/span rejects.
 * Body: residual_pdqsort (Orson Peters pdqsort, zlib, namespaced).
 * Attribution retained per zlib license.
 *
 * EXTERNAL-clean (visible metrics only). Not field-level.
 * THE BEASTIE BOYZ — path-1 pdq residual 2026-08-12
 */
#include <cstdint>
#include <cstddef>
#include <algorithm>
#include "pdqsort_residual.h"

namespace residual_low_disorder_i32 {

inline double sample_inv_ratio(const int32_t *a, size_t n) {
    if (n < 2) return 0.0;
    size_t inv = 0, checked = 0;
    size_t S = n < 4096 ? (n - 1) : 4096;
    size_t st = (n - 1) / S; if (st < 1) st = 1;
    for (size_t i = 0; i + 1 < n && checked < S; i += st, ++checked)
        if (a[i] > a[i + 1]) ++inv;
    size_t windows[] = {0, n / 4, n / 2, (3 * n) / 4};
    for (size_t w = 0; w < 4; ++w) {
        size_t base = windows[w];
        if (base >= n - 1) continue;
        for (size_t k = 0; k < 64 && base + k + 1 < n; ++k, ++checked)
            if (a[base + k] > a[base + k + 1]) ++inv;
    }
    return checked ? (double)inv / (double)checked : 0.0;
}

inline bool should_try_low_disorder(const int32_t *a, size_t n) {
    if (n < 64) return false;
    double inv = sample_inv_ratio(a, n);
    if (inv >= 0.12) return false;

    size_t S = n < 1024 ? n : 1024;
    size_t st = n / S; if (st < 1) st = 1;
    int32_t mn = a[0], mx = a[0];
    uint32_t seen[128] = {};
    size_t approx_u = 0;
    for (size_t i = 0, c = 0; i < n && c < S; i += st, ++c) {
        if (a[i] < mn) mn = a[i];
        if (a[i] > mx) mx = a[i];
        uint32_t h = (uint32_t)a[i] * 0x9E3779B9u;
        h ^= h >> 16;
        size_t slot = (h >> 25) & 127;
        uint32_t tag = (h >> 4) | 1;
        if (seen[slot] != tag) {
            if (seen[slot] == 0) ++approx_u;
            seen[slot] = tag;
        }
    }
    uint32_t span = (uint32_t)(mx - mn);
    if (approx_u <= 32) return false;
    if (span < (uint32_t)n && span < 100000ull) return false;

    if (inv <= 0.05) return true;
    size_t asc = 0, checked = 0;
    size_t step = n / 1024; if (step < 1) step = 1;
    for (size_t i = 0; i + 1 < n && checked < 1024; i += step, ++checked)
        if (a[i] <= a[i + 1]) ++asc;
    return checked && inv <= 0.10 && ((double)asc / (double)checked) >= 0.88;
}

inline bool residual_low_disorder_i32(int32_t *a, size_t n) {
    if (n < 2) return true;
    if (!should_try_low_disorder(a, n)) return false;
    residual_pdqsort(a, a + n);
    return true;
}

} // namespace residual_low_disorder_i32
