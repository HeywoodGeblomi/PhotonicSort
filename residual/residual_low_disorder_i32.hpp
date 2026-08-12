#pragma once
/*
 * residual_low_disorder_i32 — pure residual for near-monotonic / low-disorder int32
 * Adapted from residual_low_disorder_i64 (Phase 1).
 * EXTERNAL-clean. Insertion (ultra-low inv local jitter) + introsort with std::sort fallback.
 * THE BEASTIE BOYZ — Wave 2 multi-type residual polish 2026-08-12
 */
#include <cstdint>
#include <cstddef>
#include <utility>
#include <algorithm>
#include <cstring>
#include <vector>

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
    if (inv >= 0.10) return false;

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
    if (approx_u <= 64) return false;
    if (span < (uint32_t)n && span < 100000ull) return false;

    {
        size_t step = n / 256; if (step < 1) step = 1;
        size_t breaks = 0;
        size_t first_break = n, last_break = 0;
        for (size_t i = step; i < n; i += step) {
            if (a[i] < a[i - step]) {
                ++breaks;
                if (first_break == n) first_break = i;
                last_break = i;
            }
        }
        if (breaks >= 1 && breaks <= 8 && first_break > n / 5 && last_break < (n * 4) / 5) {
            if (inv > 0.005) return false;
        }
    }

    if (span + 1 >= (uint32_t)n - (uint32_t)n / 50 && span <= (uint32_t)n + (uint32_t)n / 50) {
        if (inv > 0.02) return false;
    }

    if (inv <= 0.03) return true;
    size_t asc = 0, checked = 0;
    size_t step = n / 1024; if (step < 1) step = 1;
    for (size_t i = 0; i + 1 < n && checked < 1024; i += step, ++checked)
        if (a[i] <= a[i + 1]) ++asc;
    return checked && inv <= 0.06 && ((double)asc / (double)checked) >= 0.92;
}

static constexpr size_t INSERTION_THRESHOLD = 24;

inline void insertion_sort(int32_t *lo, int32_t *hi) {
    for (int32_t *i = lo + 1; i < hi; ++i) {
        int32_t key = *i;
        int32_t *j = i;
        while (j > lo && *(j - 1) > key) { *j = *(j - 1); --j; }
        *j = key;
    }
}

inline void sift_down(int32_t *a, size_t root, size_t end) {
    while (true) {
        size_t child = root * 2 + 1;
        if (child >= end) break;
        if (child + 1 < end && a[child] < a[child + 1]) ++child;
        if (a[root] >= a[child]) break;
        std::swap(a[root], a[child]);
        root = child;
    }
}

inline void heapsort(int32_t *lo, int32_t *hi) {
    size_t n = (size_t)(hi - lo);
    for (size_t i = n / 2; i-- > 0; ) sift_down(lo, i, n);
    for (size_t i = n; i-- > 1; ) {
        std::swap(lo[0], lo[i]);
        sift_down(lo, 0, i);
    }
}

inline int32_t *median3(int32_t *a, int32_t *b, int32_t *c) {
    if (*a < *b) return (*b < *c) ? b : ((*a < *c) ? c : a);
    return (*a < *c) ? a : ((*b < *c) ? c : b);
}

inline int32_t *ninther(int32_t *lo, int32_t *hi) {
    size_t n = (size_t)(hi - lo);
    int32_t *mid = lo + n / 2;
    int32_t *q1 = lo + n / 4;
    int32_t *q3 = lo + (3 * n) / 4;
    return median3(median3(lo, q1, mid), median3(q1, mid, q3), median3(mid, q3, hi - 1));
}

inline int32_t *partition_hoare(int32_t *lo, int32_t *hi, int32_t pivot) {
    int32_t *i = lo;
    int32_t *j = hi;
    while (i < j) {
        while (i < j && *i <= pivot) ++i;
        while (i < j) {
            --j;
            if (*j <= pivot) break;
        }
        if (i < j) std::swap(*i, *j);
    }
    return i;
}

inline int log2_floor(size_t n) {
    int d = 0;
    while (n > 1) { n >>= 1; ++d; }
    return d;
}

inline void introsort(int32_t *lo, int32_t *hi, int depth) {
    while ((size_t)(hi - lo) > INSERTION_THRESHOLD) {
        if (depth == 0) { heapsort(lo, hi); return; }
        --depth;
        int32_t pivot = *ninther(lo, hi);
        int32_t *mid = partition_hoare(lo, hi, pivot);
        int32_t *left = lo;
        while (left < mid && *left <= pivot) ++left;
        if (left == mid) { lo = mid; continue; }
        introsort(lo, mid, depth);
        lo = mid;
    }
    insertion_sort(lo, hi);
}

inline bool try_insertion_ultralow(int32_t *a, size_t n) {
    for (size_t i = 1; i < n; ++i) {
        int32_t key = a[i];
        size_t j = i;
        while (j > 0 && a[j - 1] > key) { a[j] = a[j - 1]; --j; }
        a[j] = key;
    }
    return true;
}

inline bool residual_low_disorder_i32(int32_t *a, size_t n) {
    if (n < 2) return true;
    if (!should_try_low_disorder(a, n)) return false;

    double inv = sample_inv_ratio(a, n);
    if (inv <= 0.005) {
        int32_t mn = a[0], mx = a[0];
        size_t S = n < 256 ? n : 256;
        size_t st = n / S; if (st < 1) st = 1;
        for (size_t i = 0, c = 0; i < n && c < S; i += st, ++c) {
            if (a[i] < mn) mn = a[i];
            if (a[i] > mx) mx = a[i];
        }
        uint64_t span = (uint64_t)((int64_t)mx - (int64_t)mn);
        bool consecutive_like = (span + 1 >= (uint64_t)n - (uint64_t)n / 20 &&
                                 span <= (uint64_t)n + (uint64_t)n / 20);
        if (!consecutive_like) {
            try_insertion_ultralow(a, n);
            for (size_t i = 1; i < n; ++i)
                if (a[i] < a[i - 1]) return false;
            return true;
        }
    }

    introsort(a, a + n, 2 * log2_floor(n) + 1);
    bool ok = true;
    for (size_t i = 1; i < n; ++i)
        if (a[i] < a[i - 1]) { ok = false; break; }
    if (!ok) std::sort(a, a + n);
    return true;
}

} // namespace residual_low_disorder_i32
