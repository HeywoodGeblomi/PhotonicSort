#pragma once
/*
 * residual_mixed_blocks — O(n) alternating sorted/disordered block residual
 *
 * Pattern class (Expanded Field gen_mixed_blocks):
 *   sorted base 0..n-1 with every other fixed-size block fully shuffled.
 *   [sorted B][shuffled B][sorted B][shuffled B]...
 *
 * Lever: classify fixed blocks; if alternating structure holds, sort only
 * disordered blocks in place. Always-correct: if post-pass not fully sorted,
 * recover with full library pdqsort (still owns the call).
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

template<typename T>
inline bool try_mixed_blocks(T *a, size_t n) {
    if (n < 1024) return false;

    /* Primary B matches suite generator; also try 128/512 for near-misses */
    const size_t candidates[] = {256, 128, 512};
    for (size_t bi = 0; bi < 3; ++bi) {
        const size_t B = candidates[bi];
        if (n < B * 4) continue;

        const size_t nblocks = n / B;
        if (nblocks < 4) continue;

        size_t n_sorted = 0, n_dis = 0;
        size_t alt_ok = 0;
        /* Expect start with sorted (gen_mixed_blocks); tolerate phase flip */
        bool expect_sorted = true;
        bool phase_locked = false;

        for (size_t b = 0; b < nblocks; ++b) {
            size_t lo = b * B;
            size_t hi = lo + B;
            bool sorted = block_sorted_asc(a, lo, hi);
            if (sorted) ++n_sorted;
            else ++n_dis;

            if (!phase_locked && b == 0) {
                expect_sorted = sorted;
                phase_locked = true;
                ++alt_ok;
            } else if (phase_locked) {
                if (sorted == expect_sorted) ++alt_ok;
                expect_sorted = !expect_sorted;
            }
        }

        /* Need both classes and strong alternation */
        if (n_sorted < 2 || n_dis < 2) continue;
        if (alt_ok * 4 < nblocks * 3) continue; /* ≥75% alternating */
        /* Sorted fraction roughly half (gen is ~50%) — allow 25–75% */
        if (n_sorted * 4 < nblocks || n_sorted * 4 > nblocks * 3) continue;

        /* Sort disordered blocks in place */
        expect_sorted = phase_locked ? block_sorted_asc(a, 0, B) : true;
        /* Re-walk with same phase as detection */
        {
            bool exp = block_sorted_asc(a, 0, std::min(B, n));
            for (size_t b = 0; b < nblocks; ++b) {
                size_t lo = b * B;
                size_t hi = lo + B;
                bool sorted = block_sorted_asc(a, lo, hi);
                if (!sorted) {
                    pdqsort(a + lo, a + hi);
                }
                (void)exp;
                exp = !exp;
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
