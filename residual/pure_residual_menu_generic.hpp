#pragma once
/* pure_residual_menu_generic — full-attack: eq*2 + range≤4 unroll. Not field-level. THE BEASTIE BOYZ */
#include <cstddef>
#include <cstdint>
#include <algorithm>
#include <iterator>
#include <functional>
#include <cmath>
#include <cstdlib>
#include "pdqsort_residual.h"

namespace pure_residual {

template <class Iter, class Comp>
inline int sort_generic(Iter first, Iter last, Comp comp) {
    using T = typename std::iterator_traits<Iter>::value_type;
    size_t n = (size_t)(last - first);
    if (n < 2) return 0;

    // STRUCTURE: all-equal sample + verify
    {
        const size_t S = n < 128 ? n : 128;
        size_t st = n / S; if (st < 1) st = 1;
        bool maybe = true;
        for (size_t i = 0, c = 0; i < n && c < S; i += st, ++c) {
            if (comp(*(first + (ptrdiff_t)i), *first) || comp(*first, *(first + (ptrdiff_t)i))) {
                maybe = false; break;
            }
        }
        if (maybe) {
            bool all = true;
            for (size_t i = 1; i < n; ++i)
                if (comp(*(first + (ptrdiff_t)i), *first) || comp(*first, *(first + (ptrdiff_t)i))) {
                    all = false; break;
                }
            if (all) return 0;
        }
    }

    // STRUCTURE: already sorted ascending
    {
        bool asc = true;
        for (size_t i = 1; i < n; ++i) {
            if (comp(*(first + (ptrdiff_t)i), *(first + (ptrdiff_t)(i - 1)))) {
                asc = false; break;
            }
        }
        if (asc) return 0;
    }
    // STRUCTURE: reverse sorted
    {
        bool desc = true;
        for (size_t i = 1; i < n; ++i) {
            if (comp(*(first + (ptrdiff_t)(i - 1)), *(first + (ptrdiff_t)i))) {
                desc = false; break;
            }
        }
        if (desc) { std::reverse(first, last); return 0; }
    }

    // Equal-first at 50% (eq*2 >= S) — avoids false-trigger on uniform k=4
    if (n >= 256) {
        const size_t S = 256;
        size_t eq = 0;
        for (size_t c = 0; c < S; ++c) {
            size_t i = (c * (n - 1)) / S;
            size_t j = i + 1 < n ? i + 1 : i;
            if (!comp(*(first + (ptrdiff_t)i), *(first + (ptrdiff_t)j)) &&
                !comp(*(first + (ptrdiff_t)j), *(first + (ptrdiff_t)i)))
                ++eq;
        }
        if (eq * 2 >= S) {
            residual_pdqsort(first, last, comp);
            return 0;
        }
    }

    // Discrete low-card counting
    // range≤4: specialized uint32 + 8-wide writes (x86 few_k4 attack)
    // 5–32: stack size_t; larger: calloc
    if (n >= 64) {
        const size_t S = n < 1024 ? n : 1024;
        size_t st = n / S; if (st < 1) st = 1;
        double smin = 0, smax = 0;
        bool have = false, intish = true;
        for (size_t i = 0, c0 = 0; i < n && c0 < S; i += st, ++c0) {
            double v = (double)*(first + (ptrdiff_t)i);
            if (!have) { smin = smax = v; have = true; }
            if (v < smin) smin = v;
            if (v > smax) smax = v;
            if (v != std::floor(v)) intish = false;
        }
        if (intish && have && smax - smin < 65536.0 && smax >= smin) {
            size_t range = (size_t)(smax - smin) + 1;
            if (range <= 4096 && range < n) {
                double amin = smin, amax = smax;
                for (size_t i = 0; i < n; ++i) {
                    double v = (double)*(first + (ptrdiff_t)i);
                    if (v < amin) amin = v;
                    if (v > amax) amax = v;
                    if (v != std::floor(v)) { intish = false; break; }
                }
                range = (size_t)(amax - amin) + 1;
                if (intish && range <= 4096 && range < n) {
                    if (range <= 4) {
                        uint32_t cnt4[4] = {};
                        for (size_t i = 0; i < n; ++i)
                            cnt4[(size_t)((double)*(first + (ptrdiff_t)i) - amin)]++;
                        T keys[4];
                        for (size_t v = 0; v < range; ++v)
                            keys[v] = (T)(amin + (double)v);
                        size_t p = 0;
                        for (size_t v = 0; v < range; ++v) {
                            T key = keys[v];
                            uint32_t c = cnt4[v];
                            while (c >= 8) {
                                *(first + (ptrdiff_t)(p)) = key;
                                *(first + (ptrdiff_t)(p + 1)) = key;
                                *(first + (ptrdiff_t)(p + 2)) = key;
                                *(first + (ptrdiff_t)(p + 3)) = key;
                                *(first + (ptrdiff_t)(p + 4)) = key;
                                *(first + (ptrdiff_t)(p + 5)) = key;
                                *(first + (ptrdiff_t)(p + 6)) = key;
                                *(first + (ptrdiff_t)(p + 7)) = key;
                                p += 8; c -= 8;
                            }
                            while (c--) {
                                *(first + (ptrdiff_t)(p++)) = key;
                            }
                        }
                        return 0;
                    }
                    size_t stack_cnt[32];
                    size_t *cnt = nullptr;
                    bool on_stack = (range <= 32);
                    if (on_stack) {
                        for (size_t v = 0; v < range; ++v) stack_cnt[v] = 0;
                        cnt = stack_cnt;
                    } else {
                        cnt = (size_t *)std::calloc(range, sizeof(size_t));
                    }
                    if (cnt) {
                        for (size_t i = 0; i < n; ++i)
                            cnt[(size_t)((double)*(first + (ptrdiff_t)i) - amin)]++;
                        size_t p = 0;
                        for (size_t v = 0; v < range; ++v) {
                            T key = (T)(amin + (double)v);
                            size_t c0 = cnt[v];
                            while (c0 >= 8) {
                                *(first + (ptrdiff_t)(p)) = key;
                                *(first + (ptrdiff_t)(p + 1)) = key;
                                *(first + (ptrdiff_t)(p + 2)) = key;
                                *(first + (ptrdiff_t)(p + 3)) = key;
                                *(first + (ptrdiff_t)(p + 4)) = key;
                                *(first + (ptrdiff_t)(p + 5)) = key;
                                *(first + (ptrdiff_t)(p + 6)) = key;
                                *(first + (ptrdiff_t)(p + 7)) = key;
                                p += 8; c0 -= 8;
                            }
                            while (c0--) {
                                *(first + (ptrdiff_t)(p++)) = key;
                            }
                        }
                        if (!on_stack) std::free(cnt);
                        return 0;
                    }
                }
            }
        }
    }

    // Low inversion → residual_pdqsort
    if (n >= 256) {
        const size_t S = 512;
        size_t inv = 0;
        for (size_t c = 0; c < S; ++c) {
            size_t i = 1 + (c * (n - 1)) / S;
            if (comp(*(first + (ptrdiff_t)i), *(first + (ptrdiff_t)(i - 1))))
                ++inv;
        }
        if (inv * 100 <= S) {
            residual_pdqsort(first, last, comp);
            return 0;
        }
    }

    // Run structure → residual_pdqsort
    {
        size_t runs = 1; int dir = 0;
        for (size_t i = 1; i < n; ++i) {
            int d = 0;
            if (comp(*(first + (ptrdiff_t)(i - 1)), *(first + (ptrdiff_t)i))) d = 1;
            else if (comp(*(first + (ptrdiff_t)i), *(first + (ptrdiff_t)(i - 1)))) d = -1;
            if (d == 0) continue;
            if (dir == 0) dir = d;
            else if (d != dir) { ++runs; dir = d; if (runs > 64) break; }
        }
        if (runs >= 2 && runs <= 64) {
            residual_pdqsort(first, last, comp);
            return 0;
        }
    }

    residual_pdqsort(first, last, comp);
    return 0;
}

template <class Iter>
inline int sort_generic(Iter first, Iter last) {
    using T = typename std::iterator_traits<Iter>::value_type;
    return sort_generic(first, last, std::less<T>());
}

inline int sort_f64(double *a, size_t n) {
    return sort_generic(a, a + n, std::less<double>());
}

} // namespace pure_residual
