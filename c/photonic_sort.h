/* PhotonicSort — Give everything. Take nothing. Become photonic.
 *
 * Highly optimized C port of the classical adaptive hybrid sort.
 * Research: Angulo/Steinberg negative-time photons (arXiv:2409.03680).
 * Metaphor only — does not claim P=NP or physical retrocausality.
 *
 * Version: 1.1.0-c
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
#define PHOTONIC_SORT_VERSION_MINOR 1
#define PHOTONIC_SORT_VERSION_PATCH 0
#define PHOTONIC_SORT_VERSION_STRING "1.1.0-c"

#ifndef PHOTONIC_SAMPLE_LIMIT
#define PHOTONIC_SAMPLE_LIMIT 4096u
#endif

typedef struct photonic_probe {
    size_t n;
    double inv_ratio;
    size_t max_run;
    size_t run_count;
    size_t direction_changes;
    size_t equal_count;
    double confidence;
    double group_delay_proxy;
    double sortedness;
    int is_negative_delay;
    int monotone_sign;
} photonic_probe_t;

void photonic_probe_i64(const int64_t *restrict a, size_t n,
                        photonic_probe_t *restrict out);

/*
 * In-place PhotonicSort for int64.
 * - O(n) exit if fully sorted or fully reverse.
 * - Residual talent menu (Geblomi-infused): capacity-checked run merge,
 *   pdqsort-class introsort, LSD radix (int64); force_collapse = stable merge.
 * Returns path code: 0=trivial, 1=structure early-exit, 2=residual, -1=alloc fail.
 */
int photonic_sort_i64(int64_t *restrict a, size_t n);

int photonic_sort_i64_force_collapse(int64_t *restrict a, size_t n);

int photonic_sort_i64_copy(const int64_t *restrict src, int64_t *restrict dst,
                           size_t n);

typedef int (*photonic_cmp_fn)(const void *a, const void *b);

void photonic_probe_generic(const void *base, size_t n, size_t size,
                            photonic_cmp_fn cmp, photonic_probe_t *out);

int photonic_sort(void *base, size_t n, size_t size, photonic_cmp_fn cmp);

int photonic_is_sorted_i64(const int64_t *a, size_t n);
const char *photonic_sort_version(void);

#ifdef __cplusplus
}
#endif

#endif /* PHOTONIC_SORT_H */
