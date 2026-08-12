/*
    pdqsort_residual.h - Full Orson Peters pdqsort, namespaced for PhotonicSort residual.
    Copyright (c) 2021 Orson Peters — zlib license (see orlp_pdqsort.h).
    THE BEASTIE BOYZ — pipe_sparse kill 2026-08-12
*/
#pragma once
#include "orlp_pdqsort.h"

template<class Iter, class Compare>
inline void residual_pdqsort(Iter begin, Iter end, Compare comp) {
    pdqsort(begin, end, comp);
}
template<class Iter>
inline void residual_pdqsort(Iter begin, Iter end) {
    pdqsort(begin, end);
}
