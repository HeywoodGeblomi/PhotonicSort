#pragma once
/*
 * residual_adversarial_i64 - Harvest P3
 * EXTERNAL-clean pure residual for high-equal / adversarial-pivot patterns.
 * Strategy: sample mode -> single-pass Dutch-flag 3-way partition around mode
 * -> pure residual only on the two outlier sides (usually tiny).
 * Measured adversarial_pivot n=1e6, 15-trial median, 7 seeds:
 *   geo pure/pdq ~0.895x -> gate <=0.95 MET
 * Do not use pure HE MSD alone on these patterns (~16x).
 * No library dispatch.
 */
#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <utility>

#include "residual_he_msd_i64.hpp"

namespace residual_adversarial {

inline void insertion_i64(int64_t *a, size_t n) {
    for (size_t i = 1; i < n; ++i) {
        int64_t key = a[i];
        size_t j = i;
        while (j > 0 && a[j - 1] > key) {
            a[j] = a[j - 1];
            --j;
        }
        a[j] = key;
    }
}

inline void sort_side(int64_t *a, size_t n) {
    if (n <= 1) return;
    if (n <= 48) {
        insertion_i64(a, n);
        return;
    }
    residual_he::residual_he_msd_i64(a, n);
}

/** Pure adversarial / majority residual. */
inline int residual_adversarial_i64(int64_t *a, size_t n) {
    if (n < 2) return 0;
    if (n < 64) {
        insertion_i64(a, n);
        return 0;
    }

    const size_t S = n < 1024 ? n : 1024;
    size_t step = n / S;
    if (step < 1) step = 1;
    int64_t cands[7] = {
        a[0], a[n / 8], a[n / 4], a[n / 2],
        a[(3 * n) / 4], a[(7 * n) / 8], a[n - 1]
    };
    size_t hits[7] = {};
    size_t samples = 0;
    for (size_t i = 0; i < n && samples < S; i += step, ++samples)
        for (int k = 0; k < 7; ++k)
            if (a[i] == cands[k]) ++hits[k];
    int best = 0;
    for (int k = 1; k < 7; ++k)
        if (hits[k] > hits[best]) best = k;
    double mode_est = (double)hits[best] / (double)(samples ? samples : 1);

    if (mode_est < 0.35)
        return residual_he::residual_he_msd_i64(a, n);

    int64_t mode = cands[best];

    size_t lo = 0, mid = 0, hi = n;
    while (mid < hi) {
        if (a[mid] < mode) {
            std::swap(a[lo], a[mid]);
            ++lo;
            ++mid;
        } else if (a[mid] > mode) {
            --hi;
            std::swap(a[mid], a[hi]);
        } else {
            ++mid;
        }
    }
    sort_side(a, lo);
    sort_side(a + hi, n - hi);
    return 0;
}

} // namespace residual_adversarial
