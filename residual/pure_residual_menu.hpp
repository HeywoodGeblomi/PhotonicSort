#pragma once
/*
 * pure_residual_menu — residual-improve attack (post v1.5.1-c)
 * EXTERNAL-clean pure residual path. No library pdq/ska/std::sort dispatch
 *   (identity-almost uses std::sort only on tiny displacement index arrays).
 *
 * Menu order:
 *   constant probe → early FEW_WIDE → STRUCTURE v2 → late high-disorder gate →
 *   reverse-runs → FEW_WIDE → counting → majority → 3-run merge → low_disorder →
 *   sparse → identity-almost → HE MSD
 *
 * Phase 1 (2026-08-11): residual_low_disorder closes db_pk / timestamps.
 * PO5 (2026-08-12): STRUCTURE v2 + late pipe_sparse gate (compact+not-full-unique → residual_pdqsort).
 * Phase 0 baseline remains frozen historical reference. Not field-level.
 * THE BEASTIE BOYZ
 */
#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <algorithm>

#include "residual_he_msd_i64.hpp"
#include "residual_sparse_i64.hpp"
#include "residual_adversarial_i64.hpp"
#include "residual_few_wide_i64.hpp"
#include "residual_low_disorder_i64.hpp"
#include "pdqsort_residual.h"

namespace pure_residual {

inline bool try_identity_almost(int64_t *a, size_t n) {
    if (n < 16) return false;
    int64_t mn = a[0], mx = a[0];
    size_t step = n / 64; if (step < 1) step = 1;
    for (size_t i = 0; i < n; i += step) {
        if (a[i] < mn) mn = a[i];
        if (a[i] > mx) mx = a[i];
    }
    if ((uint64_t)(mx - mn) + 1 != n && (uint64_t)(mx - mn) + 1 != n - 1) {
        mn = a[0]; mx = a[0];
        for (size_t i = 1; i < n; ++i) {
            if (a[i] < mn) mn = a[i];
            if (a[i] > mx) mx = a[i];
        }
        if ((uint64_t)(mx - mn) + 1 != n) return false;
    }
    constexpr size_t DMAX = 65536;
    size_t *idx = (size_t *)std::malloc(DMAX * sizeof(size_t));
    int64_t *vals = (int64_t *)std::malloc(DMAX * sizeof(int64_t));
    if (!idx || !vals) { std::free(idx); std::free(vals); return false; }
    size_t early = n < 4096 ? n : 4096, early_mis = 0;
    for (size_t i = 0; i < early; ++i)
        if (a[i] != mn + (int64_t)i) ++early_mis;
    if (early_mis > early / 4) { std::free(idx); std::free(vals); return false; }
    size_t d = 0; bool ok = true;
    for (size_t i = 0; i < n; ++i) {
        if (a[i] != mn + (int64_t)i) {
            if (d >= DMAX) { ok = false; break; }
            idx[d] = i; vals[d] = a[i]; ++d;
        }
    }
    if (!ok || d == 0) { std::free(idx); std::free(vals); return d == 0; }
    std::sort(vals, vals + d);
    std::sort(idx, idx + d);
    for (size_t k = 0; k < d; ++k) {
        if (vals[k] != mn + (int64_t)idx[k]) {
            std::free(idx); std::free(vals); return false;
        }
    }
    for (size_t k = 0; k < d; ++k) a[idx[k]] = vals[k];
    std::free(idx); std::free(vals);
    return true;
}

inline bool try_counting(int64_t *a, size_t n) {
    if (n < 2) return true;
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
    if (srange >= (1ull << 20)) return false;
    if (srange >= (uint64_t)(n * 3 / 4)) return false;
    std::sort(sample_vals, sample_vals + ns);
    size_t sample_u = 1;
    for (size_t i = 1; i < ns; ++i)
        if (sample_vals[i] != sample_vals[i - 1]) ++sample_u;
    if (sample_u > 4096) return false;
    if (sample_u > 1024 && srange > 262144ull) return false;
    int64_t amin = smin, amax = smax;
    for (size_t i = 0; i < n; ++i) {
        if (a[i] < amin) amin = a[i];
        if (a[i] > amax) amax = a[i];
    }
    if (amin == amax) return true;
    uint64_t range = (uint64_t)(amax - amin);
    if (range >= (1ull << 20)) return false;
    if (range >= (uint64_t)n) return false;
    if (range >= (uint64_t)(n * 3 / 4)) return false;
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

inline bool try_push_middle(int64_t *a, size_t n) {
    if (n < 64) return false;
    size_t prefix_end = 1;
    while (prefix_end < n && a[prefix_end] >= a[prefix_end - 1]) ++prefix_end;
    size_t suffix_start = n - 1;
    while (suffix_start > 0 && a[suffix_start] >= a[suffix_start - 1]) --suffix_start;
    if (prefix_end >= suffix_start) return false;
    size_t mid_lo = prefix_end, mid_hi = suffix_start, mid_n = mid_hi - mid_lo;
    size_t left_run = prefix_end, right_run = n - suffix_start;
    if (left_run + right_run < (n * 2) / 3 || mid_n > n / 4) return false;
    if (mid_n < 1) return true;
    bool push_front_shape = (left_run < n / 20 && right_run >= n / 2 && mid_n <= n / 4);
    if (!push_front_shape && left_run < n / 5) return false;
    if (mid_n <= 64) {
        for (size_t i = mid_lo + 1; i < mid_hi; ++i) {
            int64_t key = a[i]; size_t j = i;
            while (j > mid_lo && a[j - 1] > key) { a[j] = a[j - 1]; --j; }
            a[j] = key;
        }
    } else residual_he::residual_he_msd_i64(a + mid_lo, mid_n);
    int64_t *tmp = (int64_t *)std::malloc(n * sizeof(int64_t));
    if (!tmp) return false;
    size_t i = 0, j = mid_lo, k = mid_hi, p = 0;
    while (i < mid_lo && j < mid_hi && k < n) {
        int64_t x = a[i], y = a[j], z = a[k];
        if (x <= y && x <= z) { tmp[p++] = x; ++i; }
        else if (y <= z) { tmp[p++] = y; ++j; }
        else { tmp[p++] = z; ++k; }
    }
    while (i < mid_lo && j < mid_hi) { if (a[i] <= a[j]) tmp[p++] = a[i++]; else tmp[p++] = a[j++]; }
    while (i < mid_lo && k < n) { if (a[i] <= a[k]) tmp[p++] = a[i++]; else tmp[p++] = a[k++]; }
    while (j < mid_hi && k < n) { if (a[j] <= a[k]) tmp[p++] = a[j++]; else tmp[p++] = a[k++]; }
    while (i < mid_lo) tmp[p++] = a[i++];
    while (j < mid_hi) tmp[p++] = a[j++];
    while (k < n) tmp[p++] = a[k++];
    std::memcpy(a, tmp, n * sizeof(int64_t)); std::free(tmp);
    for (size_t t = 1; t < n; ++t) if (a[t] < a[t - 1]) return false;
    return true;
}

inline bool try_reverse_runs(int64_t *a, size_t n) {
    if (n < 32) return false;
    size_t S = n < 8192 ? n - 1 : 8192;
    size_t st = (n - 1) / S; if (st < 1) st = 1;
    size_t sample_inv = 0, sample_desc_runs = 0, checked = 0;
    bool in_desc = false;
    for (size_t i = 0; i + 1 < n && checked < S; i += st, ++checked) {
        if (a[i] > a[i + 1]) {
            ++sample_inv;
            if (!in_desc) { ++sample_desc_runs; in_desc = true; }
        } else in_desc = false;
    }
    if (sample_desc_runs < 2 || sample_inv < 8) return false;
    if (checked > 0 && sample_inv * 2 > checked) return false;
    size_t i = 0, n_rev = 0;
    while (i + 1 < n) {
        if (a[i] > a[i + 1]) {
            size_t j = i + 1;
            while (j + 1 < n && a[j] > a[j + 1]) ++j;
            if (j - i + 1 >= 4) { std::reverse(a + i, a + j + 1); ++n_rev; }
            i = j + 1;
        } else ++i;
    }
    if (n_rev == 0) return false;
    constexpr size_t MAX_RUNS = 128;
    size_t run_end[MAX_RUNS];
    int nr = 0;
    for (size_t k = 1; k < n; ++k) {
        if (a[k] < a[k - 1]) {
            if (nr >= (int)MAX_RUNS - 1) return false;
            run_end[nr++] = k;
        }
    }
    run_end[nr++] = n;
    if (nr == 1) return true;
    if (nr > 32) return false;
    int64_t *tmp = (int64_t *)std::malloc(n * sizeof(int64_t));
    if (!tmp) return false;
    size_t ends_buf[MAX_RUNS];
    for (int r = 0; r < nr; ++r) ends_buf[r] = run_end[r];
    int nends = nr;
    int64_t *src = a;
    int64_t *dst = tmp;
    while (nends > 1) {
        size_t new_ends[MAX_RUNS];
        int nnew = 0;
        int idx = 0;
        size_t cursor = 0;
        while (idx < nends) {
            if (idx + 1 >= nends) {
                std::memcpy(dst + cursor, src + cursor, (ends_buf[idx] - cursor) * sizeof(int64_t));
                new_ends[nnew++] = ends_buf[idx];
                break;
            }
            size_t s1 = cursor, e1 = ends_buf[idx], s2 = ends_buf[idx], e2 = ends_buf[idx + 1];
            size_t p = s1, i1 = s1, i2 = s2;
            while (i1 < e1 && i2 < e2) {
                if (src[i1] <= src[i2]) dst[p++] = src[i1++];
                else dst[p++] = src[i2++];
            }
            while (i1 < e1) dst[p++] = src[i1++];
            while (i2 < e2) dst[p++] = src[i2++];
            new_ends[nnew++] = e2;
            cursor = e2;
            idx += 2;
        }
        for (int r = 0; r < nnew; ++r) ends_buf[r] = new_ends[r];
        nends = nnew;
        int64_t *t = src; src = dst; dst = t;
    }
    if (src != a) std::memcpy(a, src, n * sizeof(int64_t));
    std::free(tmp);
    for (size_t k = 1; k < n; ++k)
        if (a[k] < a[k - 1]) return false;
    return true;
}

inline int sort_i64(int64_t *a, size_t n) {
    if (n < 2) return 0;

    {
        const size_t S = n < 128 ? n : 128;
        size_t st = n / S; if (st < 1) st = 1;
        int64_t v0 = a[0];
        bool maybe = true;
        for (size_t i = 0, c = 0; i < n && c < S; i += st, ++c) {
            if (a[i] != v0) { maybe = false; break; }
        }
        if (maybe) {
            bool all = true;
            for (size_t i = 1; i < n; ++i) {
                if (a[i] != v0) { all = false; break; }
            }
            if (all) return 0;
        }
    }

    if (n >= 64 && residual_few_wide::should_try_few_wide(a, n)) {
        if (residual_few_wide::residual_few_wide_i64(a, n)) return 0;
    }

    // STRUCTURE v2: asc-first tight scan
    {
        bool asc = true;
        for (size_t i = 1; i < n; ++i) {
            if (a[i] < a[i - 1]) { asc = false; break; }
        }
        if (asc) return 0;
    }
    {
        bool desc = true;
        for (size_t i = 1; i < n; ++i) {
            if (a[i] > a[i - 1]) { desc = false; break; }
        }
        if (desc) { std::reverse(a, a + n); return 0; }
    }

    // Late: high-disorder + compact domain + not near-full-unique → residual_pdqsort
    // Closes pipe_sparse on i64 (same gate as i32/u32). Protects HE MSD path.
    if (n >= 256) {
        const size_t S = 512;
        size_t inv = 0;
        int64_t mn = a[0], mx = a[0];
        int64_t samp[512];
        for (size_t c = 0; c < S; ++c) {
            size_t i = 1 + (c * (n - 1)) / S;
            if (a[i] < a[i - 1]) ++inv;
            if (a[i] < mn) mn = a[i];
            if (a[i] > mx) mx = a[i];
            samp[c] = a[i];
        }
        uint64_t dom = (uint64_t)(mx - mn);
        bool compact = dom <= (uint64_t)n * 4ull;
        std::sort(samp, samp + S);
        size_t u = 1;
        for (size_t c = 1; c < S; ++c) if (samp[c] != samp[c-1]) ++u;
        bool not_full_unique = u < (S * 9) / 10;
        if (inv * 2 >= S && compact && not_full_unique) {
            residual_pdqsort(a, a + n);
            return 0;
        }
    }

    if (try_reverse_runs(a, n)) return 0;

    if (residual_few_wide::should_try_few_wide(a, n)) {
        if (residual_few_wide::residual_few_wide_i64(a, n)) return 0;
    }

    if (try_counting(a, n)) return 0;

    {
        size_t sample = n < 1024 ? n - 1 : 1024;
        size_t step = (n - 1) / sample; if (step < 1) step = 1;
        size_t eq = 0, checked = 0;
        for (size_t i = 0; i + step < n && checked < sample; i += step, ++checked)
            if (a[i] == a[i + step]) ++eq;
        if ((double)eq / (double)(checked ? checked : 1) >= 0.35)
            return residual_adversarial::residual_adversarial_i64(a, n);
    }

    {
        size_t prefix_end = 1;
        while (prefix_end < n && a[prefix_end] >= a[prefix_end - 1]) ++prefix_end;
        size_t suffix_start = n - 1;
        while (suffix_start > 0 && a[suffix_start] >= a[suffix_start - 1]) --suffix_start;
        size_t mid_n = (prefix_end < suffix_start) ? (suffix_start - prefix_end) : n;
        size_t right_run = n - suffix_start;
        bool middle_shape = (prefix_end >= n / 5 && right_run >= n / 5 && mid_n <= n / 4);
        bool front_shape  = (prefix_end < n / 20 && right_run >= n / 2 && mid_n <= n / 4);
        if (middle_shape || front_shape) {
            if (try_push_middle(a, n)) return 0;
        }
    }

    if (residual_low_disorder::should_try_low_disorder(a, n)) {
        if (residual_low_disorder::residual_low_disorder_i64(a, n)) return 0;
    }

    {
        size_t S = n < 1024 ? n : 1024;
        size_t st = n / S; if (st < 1) st = 1;
        uint8_t seen[256] = {};
        size_t distinct = 0, samples = 0;
        int64_t amin = a[0], amax = a[0];
        for (size_t i = 0; i < n && samples < S; i += st, ++samples) {
            uint64_t u = (uint64_t)a[i] ^ (1ull << 63);
            uint8_t tb = (uint8_t)(u >> 56);
            if (!seen[tb]) { seen[tb] = 1; ++distinct; }
            if (a[i] < amin) amin = a[i];
            if (a[i] > amax) amax = a[i];
        }
        uint64_t span = (uint64_t)(amax - amin);
        if ((distinct <= 64 && span > 64ull * (uint64_t)n) ||
            (distinct <= 32 && span > 16ull * (uint64_t)n))
            return residual_sparse::residual_sparse_i64(a, n);
    }

    if (try_identity_almost(a, n)) return 0;
    return residual_he::residual_he_msd_i64(a, n);
}

} // namespace pure_residual
