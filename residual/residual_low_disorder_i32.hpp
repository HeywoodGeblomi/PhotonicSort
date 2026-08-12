#pragma once
/*
 * residual_low_disorder_i32 — pure residual for near-monotonic / low-inversion int32
 * Adapted from residual_low_disorder_i64 (Phase 1).
 * Ultra-low inv → insertion; moderate → pattern-defeating introsort.
 * EXTERNAL-clean. THE BEASTIE BOYZ — Wave 2 multi-type Phase A 2026-08-12
 */
#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <algorithm>
#include <cmath>

namespace residual_low_disorder_i32 {

inline double sample_inv_ratio(const int32_t *a, size_t n) {
    if (n < 2) return 0.0;
    size_t S = n < 4096 ? n - 1 : 4096;
    size_t st = (n - 1) / S; if (st < 1) st = 1;
    size_t inv = 0, checked = 0;
    for (size_t i = 0; i + 1 < n && checked < S; i += st, ++checked)
        if (a[i] > a[i + 1]) ++inv;
    return checked ? (double)inv / (double)checked : 0.0;
}

inline bool should_try_low_disorder(const int32_t *a, size_t n) {
    if (n < 64) return false;
    // Reject high-card / compact / push_middle-shape / consecutive-domain
    size_t S = n < 1024 ? n : 1024;
    size_t st = n / S; if (st < 1) st = 1;
    uint32_t seen[128] = {};
    size_t distinct = 0, samples = 0;
    int32_t amin = a[0], amax = a[0];
    for (size_t i = 0; i < n && samples < S; i += st, ++samples) {
        uint32_t h = (uint32_t)a[i] * 0x9E3779B9u;
        h ^= h >> 16;
        size_t slot = (h >> 25) & 127;
        uint32_t tag = (h >> 4) | 1;
        if (seen[slot] != tag) {
            if (seen[slot] == 0) ++distinct;
            seen[slot] = tag;
        }
        if (a[i] < amin) amin = a[i];
        if (a[i] > amax) amax = a[i];
    }
    uint64_t span = (uint64_t)((int64_t)amax - (int64_t)amin);
    if (distinct > 64 && span > 64ull * (uint64_t)n) return false;
    if (span < (uint64_t)n && span < 100000ull) return false;
    // Full-span stratified inv sample
    double inv = sample_inv_ratio(a, n);
    if (inv > 0.15) return false;
    // Reject push_middle-ish shapes (long sorted prefix+suffix)
    size_t prefix_end = 1;
    while (prefix_end < n && a[prefix_end] >= a[prefix_end - 1]) ++prefix_end;
    size_t suffix_start = n - 1;
    while (suffix_start > 0 && a[suffix_start] >= a[suffix_start - 1]) --suffix_start;
    size_t mid_n = (prefix_end < suffix_start) ? (suffix_start - prefix_end) : n;
    if (prefix_end >= n / 5 && (n - suffix_start) >= n / 5 && mid_n <= n / 4)
        return false;
    // Reject consecutive-domain identity-almost
    if (span + 1 >= (uint64_t)n - (uint64_t)n / 50 && span <= (uint64_t)n + (uint64_t)n / 50)
        return false;
    return inv <= 0.08;
}

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
    int32_t *i = lo - 1;
    int32_t *j = hi;
    while (true) {
        do { ++i; } while (*i < pivot);
        do { --j; } while (*j > pivot);
        if (i >= j) return j + 1;
        std::swap(*i, *j);
    }
}

inline void introsort(int32_t *lo, int32_t *hi, int depth) {
    while ((size_t)(hi - lo) > 16) {
        if (depth == 0) { heapsort(lo, hi); return; }
        --depth;
        int32_t pivot = *ninther(lo, hi);
        int32_t *mid = partition_hoare(lo, hi, pivot);
        // Already-partitioned check
        int32_t *left = lo;
        while (left < mid && *left <= pivot) ++left;
        if (left == mid) { lo = mid; continue; }
        introsort(lo, mid, depth);
        lo = mid;
    }
    insertion_sort(lo, hi);
}

inline bool try_insertion_ultralow(int32_t *a, size_t n) {
    // Ultra-low inversion: full insertion
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
    double inv = sample_inv_ratio(a, n);
    if (inv <= 0.005) return try_insertion_ultralow(a, n);
    // Moderate low-disorder: pattern-defeating introsort
    int depth = 0;
    size_t m = n;
    while (m > 1) { m >>= 1; ++depth; }
    depth = 2 * depth + 2;
    introsort(a, a + n, depth);
    return true;
}

} // namespace residual_low_disorder_i32
