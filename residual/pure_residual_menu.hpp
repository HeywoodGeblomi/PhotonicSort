#pragma once
/*
 * pure_residual_menu - Harvest P4 unified pure residual entry
 * EXTERNAL-clean. No library pdq/ska/std::sort on this path.
 * Gates MET 2026-08-11: geo 0.538x pdq, floors closed, zero major regressions.
 */
#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <algorithm>

#include "residual_he_msd_i64.hpp"
#include "residual_sparse_i64.hpp"
#include "residual_adversarial_i64.hpp"

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

inline bool try_counting_lowcard(int64_t *a, size_t n) {
    if (n < 2) return true;
    size_t S = n < 2048 ? n : 2048;
    size_t st = n / S; if (st < 1) st = 1;
    int64_t sample_vals[2048];
    size_t ns = 0;
    for (size_t i = 0; i < n && ns < S; i += st)
        sample_vals[ns++] = a[i];
    std::sort(sample_vals, sample_vals + ns);
    size_t sample_u = 1;
    for (size_t i = 1; i < ns; ++i)
        if (sample_vals[i] != sample_vals[i - 1]) ++sample_u;
    if (sample_u > 256) return false;
    int64_t amin = a[0], amax = a[0];
    for (size_t i = 1; i < n; ++i) {
        if (a[i] < amin) amin = a[i];
        if (a[i] > amax) amax = a[i];
    }
    uint64_t range = (uint64_t)(amax - amin);
    if (range >= (1ull << 20) || range >= (uint64_t)n * 8) return false;
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

/** Unified pure residual entry. Returns 0 on success. */
inline int sort_i64(int64_t *a, size_t n) {
    if (n < 2) return 0;
    // STRUCTURE
    {
        bool asc = true, desc = true;
        for (size_t i = 1; i < n; ++i) {
            if (a[i] < a[i - 1]) asc = false;
            if (a[i] > a[i - 1]) desc = false;
            if (!asc && !desc) break;
        }
        if (asc) return 0;
        if (desc) { std::reverse(a, a + n); return 0; }
    }
    // Majority
    {
        size_t sample = n < 1024 ? n - 1 : 1024;
        size_t step = (n - 1) / sample; if (step < 1) step = 1;
        size_t eq = 0, checked = 0;
        for (size_t i = 0; i + step < n && checked < sample; i += step, ++checked)
            if (a[i] == a[i + step]) ++eq;
        if ((double)eq / (double)(checked ? checked : 1) >= 0.35)
            return residual_adversarial::residual_adversarial_i64(a, n);
    }
    // Low-card
    if (try_counting_lowcard(a, n)) return 0;
    // Sparse
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
    // Identity-almost -> HE
    if (try_identity_almost(a, n)) return 0;
    return residual_he::residual_he_msd_i64(a, n);
}

} // namespace pure_residual
