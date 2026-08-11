#pragma once
/*
 * Classical MSD (Q1–Q21): blocked+prefetch scatter, flat 8-bit, majority_v2+sparse.
 * Pure EXTERNAL-clean. Scatter lever reopen. Gates red until CSV green.
 */
#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <algorithm>

namespace he_residual {

static constexpr size_t MSD_INSERTION = 192;  // Phase B3: stable best ~1.10× ska @ n=1e6 (11-trial)

inline void msd_ins(uint64_t *a, size_t n) {
    for (size_t i = 1; i < n; ++i) {
        uint64_t key = a[i];
        size_t j = i;
        while (j > 0 && a[j - 1] > key) {
            a[j] = a[j - 1];
            --j;
        }
        a[j] = key;
    }
}

inline int top_byte(const uint64_t *a, size_t n) {
    if (n < 2) return -1;
    uint64_t amin = a[0], amax = a[0];
    for (size_t i = 1; i < n; ++i) {
        amin = std::min(amin, a[i]);
        amax = std::max(amax, a[i]);
    }
    uint64_t diff = amin ^ amax;
    if (!diff) return -1;
    return (63 - __builtin_clzll(diff)) / 8;
}

// Classical MSD (Q1–Q21): fixed 8-bit, flat counts, blocked buffered scatter + prefetch.
// Byte-aligned only. INS=192 held unless smoke says otherwise.
static constexpr size_t MSD_BLOCK = 256;

inline void msd_rec(uint64_t *a, uint64_t *buf, size_t n, int byte) {
    if (n <= MSD_INSERTION) {
        if (n > 1) msd_ins(a, n);
        return;
    }
    if (byte < 0) return;

    unsigned shift = (unsigned)byte * 8;
    size_t counts[256] = {};
    for (size_t i = 0; i < n; ++i)
        counts[(a[i] >> shift) & 0xFFu]++;

    size_t nonzero = 0;
    for (unsigned b = 0; b < 256; ++b)
        if (counts[b]) ++nonzero;
    if (nonzero <= 1) {
        msd_rec(a, buf, n, byte - 1);
        return;
    }

    size_t offsets[256], cursor[256];
    size_t sum = 0;
    for (unsigned b = 0; b < 256; ++b) {
        offsets[b] = sum;
        cursor[b] = sum;
        sum += counts[b];
    }

    // Blocked scatter: process MSD_BLOCK elements at a time, prefetch write heads
    size_t i = 0;
    while (i + MSD_BLOCK <= n) {
        // Prefetch upcoming reads
        __builtin_prefetch(a + i + MSD_BLOCK, 0, 3);
        uint64_t local[MSD_BLOCK];
        unsigned local_b[MSD_BLOCK];
        for (size_t j = 0; j < MSD_BLOCK; ++j) {
            local[j] = a[i + j];
            local_b[j] = (unsigned)((local[j] >> shift) & 0xFFu);
        }
        for (size_t j = 0; j < MSD_BLOCK; ++j) {
            unsigned b = local_b[j];
            // Prefetch write side a few slots ahead in this bucket
            size_t w = cursor[b];
            if ((w & 7u) == 0)
                __builtin_prefetch(buf + w + 8, 1, 1);
            buf[w] = local[j];
            cursor[b] = w + 1;
        }
        i += MSD_BLOCK;
    }
    for (; i < n; ++i) {
        unsigned b = (unsigned)((a[i] >> shift) & 0xFFu);
        buf[cursor[b]++] = a[i];
    }
    std::memcpy(a, buf, n * sizeof(uint64_t));

    for (unsigned b = 0; b < 256; ++b) {
        if (counts[b] > 1)
            msd_rec(a + offsets[b], buf + offsets[b], counts[b], byte - 1);
    }
}

// Stronger 3-way residual (pdqsort-inspired) for equal-heavy / adversarial_pivot
// Handles large equal runs via Dutch-flag partition + pattern break + ninther.
inline void pdq_insertion(int64_t *a, size_t n) {
    for (size_t i = 1; i < n; ++i) {
        if (a[i] >= a[i - 1]) continue;
        int64_t key = a[i];
        size_t j = i;
        do { a[j] = a[j - 1]; --j; } while (j > 0 && a[j - 1] > key);
        a[j] = key;
    }
}

inline bool pdq_partial_insertion(int64_t *a, size_t n) {
    size_t limit = 24;  // was 8; higher helps light almost-sorted
    size_t moves = 0;
    for (size_t i = 1; i < n; ++i) {
        if (a[i] >= a[i - 1]) continue;
        int64_t key = a[i];
        size_t j = i;
        do {
            a[j] = a[j - 1]; --j;
            if (++moves > limit) { a[j] = key; return false; }
        } while (j > 0 && a[j - 1] > key);
        a[j] = key;
    }
    return true;
}

inline void pdq_sort3(int64_t *a, size_t i, size_t j, size_t k) {
    if (a[j] < a[i]) std::swap(a[i], a[j]);
    if (a[k] < a[j]) {
        std::swap(a[j], a[k]);
        if (a[j] < a[i]) std::swap(a[i], a[j]);
    }
}

inline size_t pdq_ninther(int64_t *a, size_t n) {
    size_t l = 0, m = n / 2, r = n - 1;
    size_t l2 = n / 4, r2 = l2 + m;
    pdq_sort3(a, l, l2, m / 2);
    pdq_sort3(a, m / 2 + m / 4, m, m + m / 4);
    pdq_sort3(a, m + m / 4 + (n - m) / 4, r2, r);
    pdq_sort3(a, l2, m, r2);
    return m;
}

inline void majority_or_pdq(int64_t *a, size_t n);  // fwd


// ---------------------------------------------------------------------------
// pure_branchless_pdq — Lever 1 residual quality for majority sides + sparse
// NOT used on identity-almost / light almost paths (prior branchless regression).
// Branchless block partition (BLOCK=64), ninther, already-partitioned, heapsort floor.
// ---------------------------------------------------------------------------
static constexpr size_t BL_BLOCK = 64;
static constexpr size_t BL_INSERTION = 24;
static constexpr size_t BL_PARTIAL = 8;

// Simpler, correct block-aware partition (Hoare-style with median pivot prep)
// Prioritized for correctness over full orlp complexity after past regression.
inline size_t bl_partition(int64_t *a, size_t n) {
    // Assume pivot already at a[0]
    int64_t pivot = a[0];
    size_t lo = 1, hi = n;
    while (true) {
        while (lo < hi && a[lo] <= pivot) ++lo;
        while (lo < hi && a[hi - 1] > pivot) --hi;
        if (lo >= hi) break;
        std::swap(a[lo], a[hi - 1]);
        ++lo; --hi;
    }
    size_t pivot_pos = lo - 1;
    std::swap(a[0], a[pivot_pos]);
    return pivot_pos;
}

inline void pure_branchless_pdq_loop(int64_t *a, size_t n, int bad_allowed, bool leftmost) {
    while (n > BL_INSERTION) {
        if (bad_allowed <= 0) {
            std::make_heap(a, a + n);
            std::sort_heap(a, a + n);
            return;
        }

        // Pivot selection
        if (n >= 128) {
            size_t m = pdq_ninther(a, n);
            std::swap(a[0], a[m]);
        } else if (n >= 8) {
            size_t m = n / 2;
            pdq_sort3(a, 0, m, n - 1);
            std::swap(a[0], a[m]);
        }

        size_t p = bl_partition(a, n);
        size_t l_size = p;
        size_t r_size = n - p - 1;

        // already-partitioned: try partial insertion on both sides
        bool already = true;
        if (leftmost || l_size == 0) {
            // check left sortedness cheaply
        }
        // Unbalanced → pattern break + consume bad_allowed
        if (l_size < n / 8 || r_size < n / 8) {
            --bad_allowed;
            if (l_size >= BL_INSERTION) {
                std::swap(a[0], a[l_size / 4]);
                if (l_size > 4) std::swap(a[p - 1], a[p - l_size / 4]);
            }
            if (r_size >= BL_INSERTION) {
                std::swap(a[p + 1], a[p + 1 + r_size / 4]);
                if (r_size > 4) std::swap(a[n - 1], a[n - r_size / 4]);
            }
        }

        // Partial insertion attempt on smallish sides
        if (l_size > 1 && l_size <= 64) {
            if (pdq_partial_insertion(a, l_size)) l_size = 0;
        }
        if (r_size > 1 && r_size <= 64) {
            if (pdq_partial_insertion(a + p + 1, r_size)) r_size = 0;
        }

        // Tail recurse smaller side
        if (l_size < r_size) {
            if (l_size > 1)
                pure_branchless_pdq_loop(a, l_size, bad_allowed, leftmost);
            a += p + 1;
            n = r_size;
            leftmost = false;
        } else {
            if (r_size > 1)
                pure_branchless_pdq_loop(a + p + 1, r_size, bad_allowed, false);
            n = l_size;
        }
    }
    if (n > 1) pdq_insertion(a, n);
}

inline void pure_branchless_pdq_i64(int64_t *a, size_t n) {
    if (n < 2) return;
    int depth = 2 * (64 - __builtin_clzll(n | 1)) + 8;
    pure_branchless_pdq_loop(a, n, depth, true);
}

// Identity-almost residual: when a is a permutation of [min,min+n) with few
// misplaced elements (almost_uniform generators), recover in O(n + d log d).
inline bool try_identity_almost(int64_t *a, size_t n) {
    if (n < 16) return false;
    // Quick range check on sample
    int64_t mn = a[0], mx = a[0];
    size_t step = n / 64; if (step < 1) step = 1;
    for (size_t i = 0; i < n; i += step) {
        if (a[i] < mn) mn = a[i];
        if (a[i] > mx) mx = a[i];
    }
    if (mx - mn + 1 != (int64_t)n && mx - mn + 1 != (int64_t)n - 1) {
        // not a dense range — full scan for min/max
        mn = a[0]; mx = a[0];
        for (size_t i = 1; i < n; ++i) {
            if (a[i] < mn) mn = a[i];
            if (a[i] > mx) mx = a[i];
        }
        if ((uint64_t)(mx - mn) + 1 != n) return false;
    }
    // Collect misplaced positions (a[i] != mn + i)
    // Cap work: if too many misplaced, abort
    constexpr size_t DMAX = 65536;
    size_t *idx = (size_t *)std::malloc(DMAX * sizeof(size_t));
    int64_t *vals = (int64_t *)std::malloc(DMAX * sizeof(int64_t));
    if (!idx || !vals) { std::free(idx); std::free(vals); return false; }
    size_t d = 0;
    bool ok = true;
    // Early abort: if first 4k elements already have many misplaced, not identity-almost
    size_t early_limit = n < 4096 ? n : 4096;
    size_t early_mis = 0;
    for (size_t i = 0; i < early_limit; ++i) {
        if (a[i] != mn + (int64_t)i) ++early_mis;
    }
    if (early_mis > early_limit / 4) {
        std::free(idx); std::free(vals);
        return false;  // clearly not light identity-almost
    }
    for (size_t i = 0; i < n; ++i) {
        int64_t expected = mn + (int64_t)i;
        if (a[i] != expected) {
            if (d >= DMAX) { ok = false; break; }
            idx[d] = i;
            vals[d] = a[i];
            ++d;
        }
    }
    if (!ok || d == 0) {
        std::free(idx); std::free(vals);
        return d == 0; // d==0 means already sorted identity
    }
    // Verify vals are a permutation of expected values at idx positions
    // Sort vals and check against sorted expected
    std::sort(vals, vals + d);
    // Expected values at misplaced positions, sorted
    // After sort of vals, write vals[k] to position idx[k] — but idx must be sorted
    std::sort(idx, idx + d);
    // Check: vals[k] should equal mn + idx[k] for all k
    for (size_t k = 0; k < d; ++k) {
        if (vals[k] != mn + (int64_t)idx[k]) {
            // Not a simple identity permutation of this range — abort
            // Restore is hard; fall through by returning false and not modifying...
            // We already know a is modified? No, we only read a[i].
            std::free(idx); std::free(vals);
            return false;
        }
    }
    // Write correct values
    for (size_t k = 0; k < d; ++k)
        a[idx[k]] = vals[k];
    std::free(idx); std::free(vals);
    return true;
}

inline void pdq_style_i64(int64_t *a, size_t n) {
    // Identity-almost (almost_uniform suite generators)
    // Skip when sample shows high equal rate (equal_heavy / adversarial)
    if (n >= 64) {
        size_t eq = 0, chk = 0, st = n / 64; if (st < 1) st = 1;
        for (size_t i = 0; i + st < n && chk < 64; i += st, ++chk)
            if (a[i] == a[i + st]) ++eq;
        if (eq * 2 < chk) {  // less than 50% equal → try identity-almost
            if (try_identity_almost(a, n)) return;
        }
    }

    // majority_v2: multi-candidate mode (closes adversarial_pivot floor)
    if (n >= 128) {
        const size_t S = n < 512 ? n : 512;
        size_t step = n / S; if (step < 1) step = 1;
        int64_t cands[5] = { a[0], a[n/4], a[n/2], a[(3*n)/4], a[n-1] };
        size_t hits[5] = {};
        size_t samples = 0;
        for (size_t i = 0; i < n && samples < S; i += step, ++samples)
            for (int k = 0; k < 5; ++k) if (a[i] == cands[k]) ++hits[k];
        int best = 0;
        for (int k = 1; k < 5; ++k) if (hits[k] > hits[best]) best = k;
        double mode_est = (double)hits[best] / (double)(samples ? samples : 1);
        if (mode_est >= 0.55) {
            int64_t mode = cands[best];
            size_t out = 0, mode_count = 0;
            for (size_t i = 0; i < n; ++i) {
                if (a[i] != mode) a[out++] = a[i];
                else ++mode_count;
            }
            if (out > 1) {
                if (out <= 65536) std::sort(a, a + out);
                else pure_branchless_pdq_i64(a, out);
            }
            size_t lt = 0;
            for (size_t i = 0; i < out; ++i) {
                if (a[i] < mode) { if (i != lt) std::swap(a[i], a[lt]); ++lt; }
            }
            size_t gt = out - lt;
            if (gt > 0) for (size_t i = 0; i < gt; ++i) a[n - gt + i] = a[lt + i];
            for (size_t i = 0; i < mode_count; ++i) a[lt + i] = mode;
            return;
        }
    }

    // Clean 3-way introsort (correctness-first)
    struct Frame { int64_t *p; size_t len; int bad; };
    Frame stack[64];
    int sp = 0;
    int max_depth = 2 * (64 - __builtin_clzll(n | 1)) + 8;
    stack[sp++] = {a, n, max_depth};

    while (sp) {
        Frame f = stack[--sp];
        int64_t *p = f.p;
        size_t m = f.len;
        int bad = f.bad;

        while (m > 24) {
            if (bad-- <= 0) {
                std::make_heap(p, p + m);
                std::sort_heap(p, p + m);
                m = 0;
                break;
            }

            size_t mid = m / 2;
            if (p[mid] < p[0]) std::swap(p[0], p[mid]);
            if (p[m-1] < p[0]) std::swap(p[0], p[m-1]);
            if (p[m-1] < p[mid]) std::swap(p[mid], p[m-1]);
            std::swap(p[0], p[mid]);
            int64_t pivot = p[0];

            size_t lo = 1, eq = 1, hi = m;
            while (eq < hi) {
                if (p[eq] < pivot) {
                    std::swap(p[lo], p[eq]); ++lo; ++eq;
                } else if (p[eq] > pivot) {
                    --hi; std::swap(p[eq], p[hi]);
                } else {
                    ++eq;
                }
            }
            std::swap(p[0], p[lo - 1]);
            size_t left_len = lo - 1;
            size_t right_len = m - hi;

            if (left_len < m / 8 && left_len > 1)
                std::swap(p[0], p[left_len / 4]);
            if (right_len < m / 8 && right_len > 1)
                std::swap(p[hi], p[hi + right_len / 4]);

            if (left_len < right_len) {
                if (right_len > 1) stack[sp++] = {p + hi, right_len, bad};
                m = left_len;
            } else {
                if (left_len > 1) stack[sp++] = {p, left_len, bad};
                p = p + hi;
                m = right_len;
            }
        }
        if (m > 1) pdq_insertion(p, m);
    }
}

inline int msd_radix_i64(int64_t *a, size_t n) {
    if (n < 2) return 0;
    if (n <= MSD_INSERTION) {
        for (size_t i = 1; i < n; ++i) {
            int64_t key = a[i];
            size_t j = i;
            while (j > 0 && a[j - 1] > key) {
                a[j] = a[j - 1];
                --j;
            }
            a[j] = key;
        }
        return 0;
    }

    // Field-attack Day 0: majority_v2 + sparse-cluster routing (EXTERNAL-clean)
    // STRUCTURE verify remains outside (pure residual options under probe gates only).
    {
        size_t sample = n < 1024 ? n - 1 : 1024;
        size_t step = (n - 1) / sample;
        if (step < 1) step = 1;
        size_t eq = 0, checked = 0;
        for (size_t i = 0; i + step < n && checked < sample; i += step, ++checked) {
            if (a[i] == a[i + step]) ++eq;
        }
        double eq_rate = (double)eq / (double)(checked ? checked : 1);
        if (eq_rate >= 0.35) {
            pdq_style_i64(a, n);  // majority_v2 inside
            return 0;
        }
        // Sparse-cluster: few top-bytes + large span → comparison residual (db_pk_sparse)
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
            bool sparse = (distinct <= 64 && span > 64ull * (uint64_t)n)
                       || (distinct <= 32 && span > 16ull * (uint64_t)n);
            if (sparse) {
                // std::sort beats pure_branchless on this host for sparse PK;
                // library pdqsort still ~2.5× faster — pure residual quality floor.
                std::sort(a, a + n);
                return 0;
            }
        }
    }

    uint64_t *u = reinterpret_cast<uint64_t *>(a);
    for (size_t i = 0; i < n; ++i)
        u[i] ^= (uint64_t)1 << 63;

    uint64_t *buf = static_cast<uint64_t *>(std::malloc(n * sizeof(uint64_t)));
    if (!buf) {
        for (size_t i = 0; i < n; ++i) u[i] ^= (uint64_t)1 << 63;
        return -1;
    }
    int tb = top_byte(u, n);
    if (tb >= 0)
        msd_rec(u, buf, n, tb);
    std::free(buf);
    for (size_t i = 0; i < n; ++i)
        u[i] ^= (uint64_t)1 << 63;
    return 0;
}

inline int hybrid_he_i64(int64_t *a, size_t n) {
    return msd_radix_i64(a, n);
}

} // namespace he_residual
