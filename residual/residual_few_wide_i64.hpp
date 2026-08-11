#pragma once
/*
 * residual_few_wide_i64 — FEW_WIDE pure residual
 *
 * Target: low cardinality (k ≤ 16) over a wide numeric range where classic
 * counting rejects on srange. Value-preserving: collect uniques → sort →
 * rank map → count via ranks → emit original values in sorted rank order.
 *
 * Pure residual only. EXTERNAL-clean. Fixed-size tables (no unbounded alloc).
 * Fallback if k exceeds 16 mid-collection.
 *
 * THE BEASTIE BOYZ — residual-improvement phase 2026-08-11
 */
#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <algorithm>

namespace residual_few_wide {

static constexpr size_t KMAX = 16;

/** Collect up to KMAX distinct values. Returns k, or 0 on overflow / empty. */
inline size_t collect_uniques(const int64_t *a, size_t n, int64_t *uniq_out) {
    size_t k = 0;
    for (size_t i = 0; i < n; ++i) {
        int64_t v = a[i];
        bool found = false;
        for (size_t j = 0; j < k; ++j) {
            if (uniq_out[j] == v) { found = true; break; }
        }
        if (!found) {
            if (k >= KMAX) return 0; // overflow → caller falls through
            uniq_out[k++] = v;
        }
    }
    return k;
}

/** Value-preserving FEW_WIDE residual.
 *  Returns true on success; false if k > KMAX or empty (caller falls through). */
inline bool residual_few_wide_i64(int64_t *a, size_t n) {
    if (n < 2) return true;

    int64_t uniq[KMAX];
    size_t k = collect_uniques(a, n, uniq);
    if (k == 0) return false; // overflow or no values
    if (k == 1) return true;  // already "sorted"

    // Sort the k uniques
    for (size_t i = 1; i < k; ++i) {
        int64_t key = uniq[i];
        size_t j = i;
        while (j > 0 && uniq[j - 1] > key) {
            uniq[j] = uniq[j - 1];
            --j;
        }
        uniq[j] = key;
    }

    // Count via rank (linear scan of k uniques per element — fine for k≤16)
    size_t cnt[KMAX] = {};
    for (size_t i = 0; i < n; ++i) {
        int64_t v = a[i];
        size_t r = 0;
        while (r < k && uniq[r] != v) ++r;
        if (r < k) ++cnt[r];
    }

    // Emit original values in sorted rank order
    size_t p = 0;
    for (size_t r = 0; r < k; ++r) {
        int64_t v = uniq[r];
        for (size_t c = cnt[r]; c; --c)
            a[p++] = v;
    }
    return true;
}

/**
 * Probe-visible trigger: sample suggests few uniques AND wide range
 * (classic counting would reject on srange).
 */
inline bool should_try_few_wide(const int64_t *a, size_t n) {
    if (n < 2) return false;
    size_t S = n < 4096 ? n : 4096;
    size_t st = n / S; if (st < 1) st = 1;
    int64_t sample_vals[4096];
    size_t ns = 0;
    int64_t smin = a[0], smax = a[0];
    for (size_t i = 0; i < n && ns < S; i += st) {
        int64_t v = a[i];
        sample_vals[ns++] = v;
        if (v < smin) smin = v;
        if (v > smax) smax = v;
    }
    uint64_t srange = (uint64_t)(smax - smin);
    bool wide = (srange >= (1ull << 20)) || (srange >= (uint64_t)(n * 3 / 4));
    if (!wide) return false;

    std::sort(sample_vals, sample_vals + ns);
    size_t sample_u = 1;
    for (size_t i = 1; i < ns; ++i)
        if (sample_vals[i] != sample_vals[i - 1]) ++sample_u;

    return sample_u <= KMAX + 2;
}

} // namespace residual_few_wide
