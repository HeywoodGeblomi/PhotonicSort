/*
    pdqsort.h - Pattern-defeating quicksort.

    Copyright (c) 2021 Orson Peters

    This software is provided 'as-is', without any express or implied warranty. In no event will the
    authors be held liable for any damages arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose, including commercial
    applications, and to alter it and redistribute it freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not claim that you wrote the
       original software. If you use this software in a product, an acknowledgment in the product
       documentation would be appreciated but is not required.

    2. Altered source versions must be plainly marked as such, and must not be misrepresented as
       being the original software.

    3. This notice may not be removed or altered from any source distribution.
*/

/* Altered for PhotonicSort residual path: header renamed pdqsort_residual.h, residual-only use.
 * THE BEASTIE BOYZ — path-1 2026-08-12 */

#ifndef PDQSORT_RESIDUAL_H
#define PDQSORT_RESIDUAL_H

#include <algorithm>
#include <cstddef>
#include <functional>
#include <utility>
#include <iterator>

#if __cplusplus >= 201103L
    #define PDQSORT_PREFER_MOVE(x) std::move(x)
#else
    #define PDQSORT_PREFER_MOVE(x) (x)
#endif

namespace pdqsort_detail {
    enum {
        insertion_sort_threshold = 24,
        ninther_threshold = 128,
        partial_insertion_sort_limit = 8,
        block_size = 64,
        cacheline_size = 64
    };

    template<class T> struct is_default_compare : std::false_type {};
    template<class T> struct is_default_compare<std::less<T>> : std::true_type {};
    template<class T> struct is_default_compare<std::greater<T>> : std::true_type {};

    template<class Iter, class Compare>
    inline void insertion_sort(Iter begin, Iter end, Compare comp) {
        typedef typename std::iterator_traits<Iter>::value_type T;
        if (begin == end) return;
        for (Iter cur = begin + 1; cur != end; ++cur) {
            Iter sift = cur;
            Iter sift_1 = cur - 1;
            if (comp(*sift, *sift_1)) {
                T tmp = PDQSORT_PREFER_MOVE(*sift);
                do { *sift-- = PDQSORT_PREFER_MOVE(*sift_1); }
                while (sift != begin && comp(tmp, *--sift_1));
                *sift = PDQSORT_PREFER_MOVE(tmp);
            }
        }
    }

    template<class Iter, class Compare>
    inline void unguarded_insertion_sort(Iter begin, Iter end, Compare comp) {
        typedef typename std::iterator_traits<Iter>::value_type T;
        if (begin == end) return;
        for (Iter cur = begin + 1; cur != end; ++cur) {
            Iter sift = cur;
            Iter sift_1 = cur - 1;
            if (comp(*sift, *sift_1)) {
                T tmp = PDQSORT_PREFER_MOVE(*sift);
                do { *sift-- = PDQSORT_PREFER_MOVE(*sift_1); }
                while (comp(tmp, *--sift_1));
                *sift = PDQSORT_PREFER_MOVE(tmp);
            }
        }
    }

    template<class Iter, class Compare>
    inline bool partial_insertion_sort(Iter begin, Iter end, Compare comp) {
        typedef typename std::iterator_traits<Iter>::value_type T;
        if (begin == end) return true;
        std::size_t limit = 0;
        for (Iter cur = begin + 1; cur != end; ++cur) {
            Iter sift = cur;
            Iter sift_1 = cur - 1;
            if (comp(*sift, *sift_1)) {
                T tmp = PDQSORT_PREFER_MOVE(*sift);
                do { *sift-- = PDQSORT_PREFER_MOVE(*sift_1); }
                while (sift != begin && comp(tmp, *--sift_1));
                *sift = PDQSORT_PREFER_MOVE(tmp);
                limit += cur - sift;
            }
            if (limit > partial_insertion_sort_limit) return false;
        }
        return true;
    }

    template<class Iter, class Compare>
    inline void sort2(Iter a, Iter b, Compare comp) {
        if (comp(*b, *a)) std::iter_swap(a, b);
    }

    template<class Iter, class Compare>
    inline void sort3(Iter a, Iter b, Iter c, Compare comp) {
        sort2(a, b, comp); sort2(b, c, comp); sort2(a, b, comp);
    }

    template<class T>
    inline T* align_cacheline(T* p) {
#if defined(UINTPTR_MAX) && UINTPTR_MAX == 0xffffffff
        std::size_t ip = reinterpret_cast<std::size_t>(p);
#else
        std::size_t ip = reinterpret_cast<std::size_t>(p);
#endif
        ip = (ip + cacheline_size - 1) & -cacheline_size;
        return reinterpret_cast<T*>(ip);
    }

    template<class Iter>
    inline void swap_offsets(Iter first, Iter last,
        unsigned char* offsets_l, unsigned char* offsets_r,
        size_t num, bool use_swaps) {
        typedef typename std::iterator_traits<Iter>::value_type T;
        if (use_swaps) {
            for (size_t i = 0; i < num; ++i)
                std::iter_swap(first + offsets_l[i], last - offsets_r[i]);
        } else if (num > 0) {
            Iter l = first + offsets_l[0]; Iter r = last - offsets_r[0];
            T tmp(PDQSORT_PREFER_MOVE(*l)); *l = PDQSORT_PREFER_MOVE(*r);
            for (size_t i = 1; i < num; ++i) {
                l = first + offsets_l[i]; *r = PDQSORT_PREFER_MOVE(*l);
                r = last - offsets_r[i]; *l = PDQSORT_PREFER_MOVE(*r);
            }
            *r = PDQSORT_PREFER_MOVE(tmp);
        }
    }

    template<class Iter, class Compare, bool Branchless>
    inline std::pair<Iter, bool> partition_right(Iter begin, Iter end, Compare comp) {
        typedef typename std::iterator_traits<Iter>::value_type T;
        T pivot(PDQSORT_PREFER_MOVE(*begin));
        Iter first = begin;
        Iter last = end;
        while (comp(*++first, pivot));
        if (first - 1 == begin) while (first < last && !comp(*--last, pivot));
        else while (!comp(*--last, pivot));
        bool already_partitioned = first >= last;
        if (!already_partitioned) {
            std::iter_swap(first, last);
            ++first;
            while (true) {
                while (comp(*first, pivot)) ++first;
                while (!comp(*--last, pivot));
                if (first >= last) break;
                std::iter_swap(first, last);
                ++first;
            }
        }
        Iter pivot_pos = first - 1;
        *begin = PDQSORT_PREFER_MOVE(*pivot_pos);
        *pivot_pos = PDQSORT_PREFER_MOVE(pivot);
        return std::make_pair(pivot_pos, already_partitioned);
    }

    template<class Iter, class Compare>
    inline Iter partition_left(Iter begin, Iter end, Compare comp) {
        typedef typename std::iterator_traits<Iter>::value_type T;
        T pivot(PDQSORT_PREFER_MOVE(*begin));
        Iter first = begin;
        Iter last = end;
        while (comp(pivot, *--last));
        if (last + 1 == end) while (first < last && !comp(pivot, *++first));
        else while (!comp(pivot, *++first));
        while (first < last) {
            std::iter_swap(first, last);
            while (comp(pivot, *--last));
            while (!comp(pivot, *++first));
        }
        Iter pivot_pos = last;
        *begin = PDQSORT_PREFER_MOVE(*pivot_pos);
        *pivot_pos = PDQSORT_PREFER_MOVE(pivot);
        return pivot_pos;
    }

    template<class Iter, class Compare, bool Branchless>
    inline void pdqsort_loop(Iter begin, Iter end, Compare comp, int bad_allowed, bool leftmost = true) {
        typedef typename std::iterator_traits<Iter>::value_type T;
        typedef typename std::iterator_traits<Iter>::difference_type diff_t;
        while (true) {
            diff_t size = end - begin;
            if (size < insertion_sort_threshold) {
                if (leftmost) insertion_sort(begin, end, comp);
                else unguarded_insertion_sort(begin, end, comp);
                return;
            }
            diff_t s2 = size / 2;
            if (size > ninther_threshold) {
                sort3(begin, begin + s2, end - 1, comp);
                sort3(begin + 1, begin + (s2 - 1), end - 2, comp);
                sort3(begin + 2, begin + (s2 + 1), end - 3, comp);
                sort3(begin + (s2 - 1), begin + s2, begin + (s2 + 1), comp);
                std::iter_swap(begin, begin + s2);
            } else sort3(begin + s2, begin, end - 1, comp);
            if (!leftmost && !comp(*(begin - 1), *begin)) {
                begin = partition_left(begin, end, comp) + 1;
                continue;
            }
            std::pair<Iter, bool> part_result =
                partition_right<Iter, Compare, Branchless>(begin, end, comp);
            Iter pivot_pos = part_result.first;
            bool already_partitioned = part_result.second;
            diff_t l_size = pivot_pos - begin;
            diff_t r_size = end - (pivot_pos + 1);
            bool highly_unbalanced = l_size < size / 8 || r_size < size / 8;
            if (highly_unbalanced) {
                if (--bad_allowed == 0) {
                    std::make_heap(begin, end, comp);
                    std::sort_heap(begin, end, comp);
                    return;
                }
                if (l_size >= insertion_sort_threshold) {
                    std::iter_swap(begin, begin + l_size / 4);
                    std::iter_swap(pivot_pos - 1, pivot_pos - l_size / 4);
                    if (l_size > ninther_threshold) {
                        std::iter_swap(begin + 1, begin + (l_size / 4 + 1));
                        std::iter_swap(begin + 2, begin + (l_size / 4 + 2));
                        std::iter_swap(pivot_pos - 2, pivot_pos - (l_size / 4 + 1));
                        std::iter_swap(pivot_pos - 3, pivot_pos - (l_size / 4 + 2));
                    }
                }
                if (r_size >= insertion_sort_threshold) {
                    std::iter_swap(pivot_pos + 1, pivot_pos + (1 + r_size / 4));
                    std::iter_swap(end - 1, end - r_size / 4);
                    if (r_size > ninther_threshold) {
                        std::iter_swap(pivot_pos + 2, pivot_pos + (2 + r_size / 4));
                        std::iter_swap(pivot_pos + 3, pivot_pos + (3 + r_size / 4));
                        std::iter_swap(end - 2, end - (1 + r_size / 4));
                        std::iter_swap(end - 3, end - (2 + r_size / 4));
                    }
                }
            } else {
                if (already_partitioned && partial_insertion_sort(begin, pivot_pos, comp)
                    && partial_insertion_sort(pivot_pos + 1, end, comp)) return;
            }
            pdqsort_loop<Iter, Compare, Branchless>(begin, pivot_pos, comp, bad_allowed, leftmost);
            begin = pivot_pos + 1;
            leftmost = false;
        }
    }
}

template<class Iter, class Compare>
inline void pdqsort(Iter begin, Iter end, Compare comp) {
    if (begin == end) return;
    pdqsort_detail::pdqsort_loop<Iter, Compare,
        pdqsort_detail::is_default_compare<typename std::decay<Compare>::type>::value ? true : false>(
        begin, end, comp, pdqsort_detail::log2(end - begin));
}

template<class Iter>
inline void pdqsort(Iter begin, Iter end) {
    typedef typename std::iterator_traits<Iter>::value_type T;
    pdqsort(begin, end, std::less<T>());
}

namespace pdqsort_detail {
    template<class T>
    inline int log2(T n) {
        int log = 0;
        while (n >>= 1) ++log;
        return log;
    }
}

#endif
