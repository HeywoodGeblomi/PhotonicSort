#pragma once
/*
 * residual_low_disorder_i64 — pure residual for near-monotonic / low-disorder data
 * EXTERNAL-clean. Insertion (ultra-low inv) + Hoare/ninther/already-partitioned introsort.
 *
 * Phase 1 (Field-Level Attack Plan): closes db_pk (2.34×→1.12×) and timestamps (→0.43×).
 * Gate rejects low-card, compact-range, push_middle-shape, consecutive-domain.
 * Pure residual only. Not field-level.
 * THE BEASTIE BOYZ
 */
#include <cstdint>
#include <cstddef>
#include <utility>
#include <algorithm>
#include <cstring>
#include <vector>

namespace residual_low_disorder {

inline double sample_inv_ratio(const int64_t *a, size_t n) {
    if (n < 2) return 0.0;
    size_t inv = 0, checked = 0;
    size_t S = n < 4096 ? (n - 1) : 4096;
    size_t st = (n - 1) / S; if (st < 1) st = 1;
    for (size_t i = 0; i + 1 < n && checked < S; i += st, ++checked)
        if (a[i] > a[i + 1]) ++inv;
    // Local windows for periodic patterns
    size_t windows[] = {0, n / 4, n / 2, (3 * n) / 4};
    for (size_t w = 0; w < 4; ++w) {
        size_t base = windows[w];
        if (base >= n - 1) continue;
        for (size_t k = 0; k < 64 && base + k + 1 < n; ++k, ++checked)
            if (a[base + k] > a[base + k + 1]) ++inv;
    }
    return checked ? (double)inv / (double)checked : 0.0;
}

inline bool should_try_low_disorder(const int64_t *a, size_t n) {
    if (n < 64) return false;
    double inv = sample_inv_ratio(a, n);
    if (inv >= 0.10) return false;

    // Cheap sample-based rejects first (no full O(n) scans)
    size_t S = n < 1024 ? n : 1024;
    size_t st = n / S; if (st < 1) st = 1;
    int64_t mn = a[0], mx = a[0];
    uint64_t seen[128] = {};
    size_t approx_u = 0;
    for (size_t i = 0, c = 0; i < n && c < S; i += st, ++c) {
        if (a[i] < mn) mn = a[i];
        if (a[i] > mx) mx = a[i];
        uint64_t h = (uint64_t)a[i] * 0x9E3779B97F4A7C15ull;
        size_t slot = (h >> 57) & 127;
        uint64_t tag = (h >> 4) | 1;
        if (seen[slot] != tag) {
            if (seen[slot] == 0) ++approx_u;
            seen[slot] = tag;
        }
    }
    uint64_t span = (uint64_t)(mx - mn);
    if (approx_u <= 64) return false;
    if (span < (uint64_t)n && span < 100000ull) return false;

    // Sample-based push_middle / island shape (cheap)
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
        // Single disorder region in the middle with long sorted wings
        if (breaks >= 1 && breaks <= 8 && first_break > n / 5 && last_break < (n * 4) / 5)
            return false;
    }

    // Near-consecutive domain (identity-almost / push_middle keys 0..n-1):
    // Reject only when span is CLOSE to n (not when span >> n, which is sparse db_pk).
    if (span + 1 >= (uint64_t)n - (uint64_t)n / 50 && span <= (uint64_t)n + (uint64_t)n / 50)
        return false;

    if (inv <= 0.03) return true;
    size_t asc = 0, checked = 0;
    size_t step = n / 1024; if (step < 1) step = 1;
    for (size_t i = 0; i + 1 < n && checked < 1024; i += step, ++checked)
        if (a[i] <= a[i + 1]) ++asc;
    return checked && inv <= 0.06 && ((double)asc / (double)checked) >= 0.92;
}

static constexpr size_t INSERTION_THRESHOLD = 24;

inline void insertion_sort(int64_t *lo, int64_t *hi) {
    for (int64_t *i = lo + 1; i < hi; ++i) {
        int64_t key = *i;
        int64_t *j = i;
        while (j > lo && *(j - 1) > key) { *j = *(j - 1); --j; }
        *j = key;
    }
}

inline void sift_down(int64_t *a, size_t root, size_t end) {
    while (true) {
        size_t child = root * 2 + 1;
        if (child >= end) break;
        if (child + 1 < end && a[child] < a[child + 1]) ++child;
        if (a[root] >= a[child]) break;
        std::swap(a[root], a[child]);
        root = child;
    }
}

inline void heapsort(int64_t *lo, int64_t *hi) {
    size_t n = (size_t)(hi - lo);
    if (n < 2) return;
    for (size_t start = n / 2; start-- > 0; ) sift_down(lo, start, n);
    for (size_t end = n; end-- > 1; ) {
        std::swap(lo[0], lo[end]);
        sift_down(lo, 0, end);
    }
}

inline int64_t *median3(int64_t *a, int64_t *b, int64_t *c) {
    if (*a < *b) return (*b < *c) ? b : ((*a < *c) ? c : a);
    return (*a < *c) ? a : ((*b < *c) ? c : b);
}

inline int64_t *ninther(int64_t *lo, int64_t *hi) {
    size_t n = (size_t)(hi - lo);
    int64_t *mid = lo + n / 2;
    if (n > 128) {
        size_t step = n / 8;
        return median3(
            median3(lo, lo + step, lo + 2 * step),
            median3(mid - step, mid, mid + step),
            median3(hi - 1 - 2 * step, hi - 1 - step, hi - 1));
    }
    return median3(lo, mid, hi - 1);
}

inline int64_t *partition_hoare(int64_t *lo, int64_t *hi, int64_t pivot) {
    int64_t *i = lo - 1;
    int64_t *j = hi;
    while (true) {
        do { ++i; } while (*i < pivot);
        do { --j; } while (*j > pivot);
        if (i >= j) return i;
        std::swap(*i, *j);
    }
}

inline int log2_floor(size_t n) {
    int r = 0;
    while (n > 1) { n >>= 1; ++r; }
    return r;
}

inline void introsort(int64_t *lo, int64_t *hi, int depth) {
    while ((size_t)(hi - lo) > INSERTION_THRESHOLD) {
        if (depth == 0) { heapsort(lo, hi); return; }
        --depth;

        int64_t pivot = *ninther(lo, hi);

        // already-partitioned detection
        int64_t *left = lo;
        int64_t *right = hi;
        while (left < hi && *left <= pivot) ++left;
        do { --right; } while (right > lo && *right >= pivot);
        if (left > right) {
            bool ls = true;
            for (int64_t *p = lo + 1; p < left; ++p)
                if (*(p - 1) > *p) { ls = false; break; }
            bool rs = true;
            for (int64_t *p = left + 1; p < hi; ++p)
                if (*(p - 1) > *p) { rs = false; break; }
            if (!ls) introsort(lo, left, depth);
            if (!rs) introsort(left, hi, depth);
            return;
        }

        int64_t *mid = partition_hoare(lo, hi, pivot);
        if (mid - lo < hi - mid) {
            introsort(lo, mid, depth);
            lo = mid;
        } else {
            introsort(mid, hi, depth);
            hi = mid;
        }
    }
    if ((size_t)(hi - lo) > 1) insertion_sort(lo, hi);
}

// Insertion residual for ultra-low inversion counts (local jitter)
inline bool try_insertion_ultralow(int64_t *a, size_t n) {
    // Guarded insertion — correct and fast when inversions are few and local
    for (size_t i = 1; i < n; ++i) {
        if (a[i] >= a[i - 1]) continue;
        int64_t key = a[i];
        size_t j = i;
        do {
            a[j] = a[j - 1];
            --j;
        } while (j > 0 && a[j - 1] > key);
        a[j] = key;
    }
    return true;
}

inline bool residual_low_disorder_i64(int64_t *a, size_t n) {
    if (n < 2) return true;
    if (!should_try_low_disorder(a, n)) return false;

    double inv = sample_inv_ratio(a, n);
    // Ultra-low inv (timestamps-like local jitter): insertion is O(n + inv*d)
    if (inv <= 0.005) {
        try_insertion_ultralow(a, n);
        for (size_t i = 1; i < n; ++i)
            if (a[i] < a[i - 1]) return false;
        return true;
    }

    introsort(a, a + n, 2 * log2_floor(n) + 1);
    for (size_t i = 1; i < n; ++i)
        if (a[i] < a[i - 1]) return false;
    return true;
}

} // namespace residual_low_disorder
