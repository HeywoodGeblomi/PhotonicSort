/* PhotonicSort — Give everything. Take nothing. Become photonic.
 *
 * Highly optimized C port of the classical adaptive hybrid sort.
 * Research: Angulo/Steinberg negative-time photons (arXiv:2409.03680).
 * Metaphor only — does not claim P=NP or physical retrocausality.
 *
 * Version: 1.0.1-c
 * License: MIT
 * Team: Grok + Harper + Benjamin + Lucas + Heywood Geblomi
 */
#ifndef PHOTONIC_SORT_H
#define PHOTONIC_SORT_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define PHOTONIC_SORT_VERSION_MAJOR 1
#define PHOTONIC_SORT_VERSION_MINOR 0
#define PHOTONIC_SORT_VERSION_PATCH 1
#define PHOTONIC_SORT_VERSION_STRING "1.0.1-c"

/* Probe sample ceiling (full scan at or below this length). */
#ifndef PHOTONIC_SAMPLE_LIMIT
#define PHOTONIC_SAMPLE_LIMIT 4096u
#endif

/* ---------------------------------------------------------------------------
 * Probe result — Gyro-style disorder profile (single O(n) walk).
 * --------------------------------------------------------------------------- */
typedef struct photonic_probe {
    size_t n;
    double inv_ratio;
    size_t max_run;            /* element-span scaled */
    size_t run_count;
    size_t direction_changes;
    size_t equal_count;
    double confidence;         /* 1.0 = full scan */
    double group_delay_proxy;  /* 1 - sortedness */
    double sortedness;
    int is_negative_delay;     /* early-exit eligible */
    int monotone_sign;         /* +1 asc, -1 desc, 0 mixed */
} photonic_probe_t;

/* ---------------------------------------------------------------------------
 * int64_t — primary highly optimized path (in-place)
 * --------------------------------------------------------------------------- */

/* Single-pass probe over a[0..n). */
void photonic_probe_i64(const int64_t *restrict a, size_t n,
                        photonic_probe_t *restrict out);

/*
 * In-place PhotonicSort for int64.
 * - O(n) exit if fully sorted or fully reverse (no key).
 * - Otherwise stable mergesort residual (near-linear on long runs).
 * Returns path code: 0=trivial, 1=neg-time/structure, 2=collapse/residual.
 */
int photonic_sort_i64(int64_t *restrict a, size_t n);

/* Same as photonic_sort_i64 but forces rank-collapse residual (stable merge). */
int photonic_sort_i64_force_collapse(int64_t *restrict a, size_t n);

/* Out-of-place: write sorted(src) into dst (dst may equal src). */
int photonic_sort_i64_copy(const int64_t *restrict src, int64_t *restrict dst,
                           size_t n);

/* ---------------------------------------------------------------------------
 * Generic (element size + comparator) — still adaptive
 * cmp(a,b) <0 if a<b, 0 if equal, >0 if a>b  (qsort-style, no context)
 * --------------------------------------------------------------------------- */
typedef int (*photonic_cmp_fn)(const void *a, const void *b);

void photonic_probe_generic(const void *base, size_t n, size_t size,
                            photonic_cmp_fn cmp, photonic_probe_t *out);

/* In-place adaptive sort. Uses O(n) structure exits when possible. */
int photonic_sort(void *base, size_t n, size_t size, photonic_cmp_fn cmp);

/* ---------------------------------------------------------------------------
 * Utilities
 * --------------------------------------------------------------------------- */
int photonic_is_sorted_i64(const int64_t *a, size_t n);
const char *photonic_sort_version(void);

#ifdef __cplusplus
}
#endif

#endif /* PHOTONIC_SORT_H */
