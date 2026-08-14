#pragma once
/*
 * residual_mixed_blocks — O(n) alternating sorted/disordered block residual
 *
 * Pattern class (Expanded Field gen_mixed_blocks):
 *   sorted base 0..n-1 with every other fixed-size block fully shuffled.
 *   [sorted B][shuffled B][sorted B][shuffled B]...
 *
 * Lever: cheap 4-block pre-check first; full classify only on alternation signal.
 * Sort only disordered blocks in place. Always-correct: if post-pass not fully
 * sorted, recover with full library pdqsort (still owns the call).
 *
 * EXTERNAL-clean. THE BEASTIE BOYZ 2026-08-14
 */
#include <cstddef>
#include <cstdint>
#include <algorithm>
#include <type_traits>
#include "pdqsort.h"

namespace residual_mixed_blocks {

template<typename T>
inline bool block_sorted_asc(const T *a, size_t lo, size_t hi) {
    for (size_t i = lo + 1; i < hi; ++i)
        if (a[i] < a[i - 1]) return false;
    return true;
}

/* Cheap: only look at first 4 blocks. Need both sorted + disordered alternating. */
template<typename T>
inline bool cheap_alt_signal(const T *a, size_t n, size_t B) {
    if (n < B * 4) return false;
    bool s0 = block_sorted_asc(a, 0, B);
    bool s1 = block_sorted_asc(a, B, B * 2);
    bool s2 = block_sorted_asc(a, B * 2, B * 3);
    bool s3 = block_sorted_asc(a, B * 3, B * 4);
    /* alternating: S D S D  or  D S D S */
    bool phase_s = (s0 && !s1 && s2 && !s3);
    bool phase_d = (!s0 && s1 && !s2 && s3);
    return phase_s || phase_d;
}

template<typename T>
inline bool try_mixed_blocks(T *a, size_t n) {
    if (n < 1024) return false;

    /* Primary B matches suite generator; also try 128/512 for near-misses */
    const size_t candidates[] = {256, 128, 512};
    for (size_t bi = 0; bi < 3; ++bi) {
        const size_t B = candidates[bi];
        if (n < B * 4) continue;

        /* Cheap reject — most inputs exit here after ≤4 block scans */
        if (!cheap_alt_signal(a, n, B)) continue;

        const size_t nblocks = n / B;
        if (nblocks < 4) continue;

        size_t n_sorted = 0, n_dis = 0;
        size_t alt_ok = 0;
        bool expect_sorted = block_sorted_asc(a, 0, B);

        for (size_t b = 0; b < nblocks; ++b) {
            size_t lo = b * B;
            size_t hi = lo + B;
            bool sorted = block_sorted_asc(a, lo, hi);
            if (sorted) ++n_sorted;
            else ++n_dis;
            if (sorted == expect_sorted) ++alt_ok;
            expect_sorted = !expect_sorted;
        }

        /* Need both classes and strong alternation */
        if (n_sorted < 2 || n_dis < 2) continue;
        if (alt_ok * 4 < nblocks * 3) continue; /* ≥75% alternating */
        /* Sorted fraction roughly half (gen is ~50%) — allow 25–75% */
        if (n_sorted * 4 < nblocks || n_sorted * 4 > nblocks * 3) continue;

        /* Sort disordered blocks in place */
        {
            for (size_t b = 0; b < nblocks; ++b) {
                size_t lo = b * B;
                size_t hi = lo + B;
                if (!block_sorted_asc(a, lo, hi))
                    pdqsort(a + lo, a + hi);
            }
            /* Tail remainder beyond nblocks*B */
            if (nblocks * B < n) {
                size_t lo = nblocks * B;
                if (!block_sorted_asc(a, lo, n))
                    pdqsort(a + lo, a + n);
            }
        }

        /* Verify full order; recover if false-positive structure */
        bool ok = true;
        for (size_t i = 1; i < n; ++i) {
            if (a[i] < a[i - 1]) { ok = false; break; }
        }
        if (ok) return true;
        pdqsort(a, a + n);
        return true;
    }
    return false;
}

} // namespace residual_mixed_blocks
