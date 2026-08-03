/* PhotonicSort — highly optimized C implementation
 * Give everything. Take nothing. Become photonic.
 *
 * Build:  make -C c
 * Demo:   ./c/demo
 *
 * Complexity: O(n) structure exits; O(n log n) worst-case stable residual.
 * No P=NP claims. Retrocausality is design metaphor only.
 */
#include "photonic_sort.h"

#include <stdlib.h>
#include <string.h>
#include <limits.h>

/* ---------- small helpers ------------------------------------------------ */

static inline size_t ps_min_sz(size_t a, size_t b) { return a < b ? a : b; }
static inline size_t ps_max_sz(size_t a, size_t b) { return a > b ? a : b; }

static inline double ps_clamp01(double x) {
    if (x < 0.0) return 0.0;
    if (x > 1.0) return 1.0;
    return x;
}

/* Deterministic LCG for extra pair samples (no global RNG). */
static inline uint32_t ps_lcg(uint32_t *s) {
    *s = (*s) * 1664525u + 1013904223u;
    return *s;
}

static inline void ps_swap_i64(int64_t *a, int64_t *b) {
    int64_t t = *a;
    *a = *b;
    *b = t;
}

/* In-place reverse of a[0..n). */
static void ps_reverse_i64(int64_t *restrict a, size_t n) {
    size_t i = 0, j = n;
    while (i < j) {
        --j;
        if (i >= j) break;
        ps_swap_i64(&a[i], &a[j]);
        ++i;
    }
}

/* ---------- probe i64 ---------------------------------------------------- */

void photonic_probe_i64(const int64_t *restrict a, size_t n,
                        photonic_probe_t *restrict out) {
    memset(out, 0, sizeof(*out));
    out->n = n;
    if (n <= 1) {
        out->max_run = n;
        out->run_count = n ? 1 : 0;
        out->confidence = 1.0;
        out->sortedness = 1.0;
        out->is_negative_delay = 1;
        out->monotone_sign = n ? 1 : 0;
        return;
    }

    size_t step;
    size_t n_idx;
    int full;

    if (n <= (size_t)PHOTONIC_SAMPLE_LIMIT) {
        step = 1;
        n_idx = n;
        full = 1;
        out->confidence = 1.0;
    } else {
        step = n / (size_t)PHOTONIC_SAMPLE_LIMIT;
        if (step < 1) step = 1;
        n_idx = (n + step - 1) / step;
        if ((n - 1) % step != 0) {
            /* endpoint always included via explicit last check below */
        }
        full = 0;
        out->confidence = (double)n_idx / (double)n;
        if (out->confidence > 1.0) out->confidence = 1.0;
    }

    size_t max_run_samples = 1;
    size_t run_count = 1;
    size_t direction_changes = 0;
    size_t equal_count = 0;
    size_t current_run = 1;
    int prev_dir = 0; /* -1 desc, 0 flat, +1 asc */
    size_t inv_pairs = 0;
    size_t total_pairs = 0;
    size_t asc_edges = 0;
    size_t desc_edges = 0;

    size_t prev_i = 0;
    size_t k = step;
    /* Walk stratified indices 0, step, 2*step, ... and ensure last element. */
    for (;;) {
        size_t j;
        if (full) {
            if (k >= n) break;
            j = k;
        } else {
            if (k >= n) {
                if (prev_i != n - 1) {
                    j = n - 1;
                } else {
                    break;
                }
            } else {
                j = k;
            }
        }

        int64_t av = a[prev_i];
        int64_t bv = a[j];
        total_pairs++;

        if (av > bv) {
            inv_pairs++;
            desc_edges++;
            if (prev_dir == 1) {
                direction_changes++;
                run_count++;
                if (current_run > max_run_samples) max_run_samples = current_run;
                current_run = 1;
            } else {
                current_run++;
            }
            prev_dir = -1;
        } else if (av < bv) {
            asc_edges++;
            if (prev_dir == -1) {
                direction_changes++;
                run_count++;
                if (current_run > max_run_samples) max_run_samples = current_run;
                current_run = 1;
            } else {
                current_run++;
            }
            prev_dir = 1;
        } else {
            equal_count++;
            current_run++;
        }
        if (current_run > max_run_samples) max_run_samples = current_run;

        prev_i = j;
        if (!full && j == n - 1 && (k >= n || k != j)) break;
        k += step;
        if (full && k >= n) break;
    }

    size_t max_run = ps_min_sz(n, max_run_samples * step);

    /* Deterministic extra pair probes */
    size_t extra = ps_min_sz((size_t)256, n / 4);
    if (extra > 0 && n > 2) {
        uint32_t state = (uint32_t)(n ^ 0x9E3779B9u);
        for (size_t e = 0; e < extra; e++) {
            size_t i = (size_t)(ps_lcg(&state) % (uint32_t)n);
            size_t j = (size_t)(ps_lcg(&state) % (uint32_t)n);
            if (i == j) continue;
            if (i > j) {
                size_t t = i;
                i = j;
                j = t;
            }
            total_pairs++;
            if (a[i] > a[j]) inv_pairs++;
        }
    }

    double inv_ratio = total_pairs ? (double)inv_pairs / (double)total_pairs : 0.0;

    int monotone_sign;
    if (desc_edges > 0 && asc_edges == 0)
        monotone_sign = -1;
    else if (asc_edges > 0 && desc_edges == 0)
        monotone_sign = 1;
    else
        monotone_sign = 0;

    double run_fraction = (double)max_run / (double)n;
    double inv_term = 1.0 - (inv_ratio * 2.0 < 1.0 ? inv_ratio * 2.0 : 1.0);
    double dir_den = (double)ps_max_sz((size_t)1, n / 8);
    double dir_term = 1.0 - ((double)direction_changes / dir_den < 1.0
                                 ? (double)direction_changes / dir_den
                                 : 1.0);
    double sortedness = ps_clamp01(0.45 * inv_term + 0.35 * run_fraction + 0.20 * dir_term);

    int is_neg =
        (sortedness >= 0.72) ||
        (max_run >= (size_t)(n * 0.45)) ||
        (direction_changes <= 3 && inv_ratio < 0.15) ||
        (max_run >= (size_t)(n * 0.25) && inv_ratio < 0.05) ||
        (monotone_sign != 0 && direction_changes == 0);

    out->inv_ratio = inv_ratio;
    out->max_run = max_run;
    out->run_count = run_count;
    out->direction_changes = direction_changes;
    out->equal_count = equal_count;
    out->group_delay_proxy = 1.0 - sortedness;
    out->sortedness = sortedness;
    out->is_negative_delay = is_neg ? 1 : 0;
    out->monotone_sign = monotone_sign;
}

/* ---------- stable bottom-up mergesort (int64) --------------------------- */

static void ps_merge_i64(int64_t *restrict a, int64_t *restrict tmp,
                         size_t left, size_t mid, size_t right) {
    size_t i = left, j = mid, k = left;
    while (i < mid && j < right) {
        /* stable: take left on equal */
        if (a[i] <= a[j])
            tmp[k++] = a[i++];
        else
            tmp[k++] = a[j++];
    }
    while (i < mid) tmp[k++] = a[i++];
    while (j < right) tmp[k++] = a[j++];
    for (k = left; k < right; k++) a[k] = tmp[k];
}

static int ps_mergesort_i64(int64_t *restrict a, size_t n) {
    if (n < 2) return 0;
    int64_t *tmp = (int64_t *)malloc(n * sizeof(int64_t));
    if (!tmp) return -1;

    for (size_t width = 1; width < n; width <<= 1) {
        for (size_t i = 0; i < n; i += width << 1) {
            size_t left = i;
            size_t mid = ps_min_sz(i + width, n);
            size_t right = ps_min_sz(i + (width << 1), n);
            if (mid < right) ps_merge_i64(a, tmp, left, mid, right);
        }
    }
    free(tmp);
    return 0;
}

/* Tiny insertion sort for small n (cache friendly). */
static void ps_insertion_i64(int64_t *restrict a, size_t n) {
    for (size_t i = 1; i < n; i++) {
        int64_t key = a[i];
        size_t j = i;
        while (j > 0 && a[j - 1] > key) {
            a[j] = a[j - 1];
            j--;
        }
        a[j] = key;
    }
}

/* Hybrid residual: insertion for tiny, stable mergesort otherwise. */
static int ps_residual_i64(int64_t *restrict a, size_t n) {
    if (n < 2) return 0;
    if (n <= 32) {
        ps_insertion_i64(a, n);
        return 0;
    }
    return ps_mergesort_i64(a, n);
}

/* ---------- main i64 entry ----------------------------------------------- */

static int photonic_sort_i64_impl(int64_t *restrict a, size_t n, int force_collapse) {
    if (n <= 1) return 0;

    photonic_probe_t probe;
    photonic_probe_i64(a, n, &probe);

    if (!force_collapse && probe.is_negative_delay) {
        /* O(n) pure monotone exits */
        if (probe.direction_changes == 0) {
            if (probe.monotone_sign == 1) {
                return 1; /* already sorted */
            }
            if (probe.monotone_sign == -1) {
                ps_reverse_i64(a, n);
                return 1;
            }
        }
        /* Structured residual — stable merge (Timsort-class on long runs) */
        if (ps_residual_i64(a, n) != 0) return -1;
        return 1;
    }

    /* Retrocausal collapse ≡ stable place-by-rank (mergesort stable order) */
    if (ps_residual_i64(a, n) != 0) return -1;
    return 2;
}

int photonic_sort_i64(int64_t *restrict a, size_t n) {
    return photonic_sort_i64_impl(a, n, 0);
}

int photonic_sort_i64_force_collapse(int64_t *restrict a, size_t n) {
    return photonic_sort_i64_impl(a, n, 1);
}

int photonic_sort_i64_copy(const int64_t *restrict src, int64_t *restrict dst,
                           size_t n) {
    if (dst != src) {
        if (n) memcpy(dst, src, n * sizeof(int64_t));
    }
    return photonic_sort_i64(dst, n);
}

int photonic_is_sorted_i64(const int64_t *a, size_t n) {
    for (size_t i = 1; i < n; i++) {
        if (a[i - 1] > a[i]) return 0;
    }
    return 1;
}

const char *photonic_sort_version(void) {
    return PHOTONIC_SORT_VERSION_STRING;
}

/* ---------- generic path (element size + cmp) ---------------------------- */

static inline const char *ps_at(const void *base, size_t size, size_t i) {
    return (const char *)base + i * size;
}

static inline char *ps_at_mut(void *base, size_t size, size_t i) {
    return (char *)base + i * size;
}

static void ps_swap_bytes(char *a, char *b, size_t size) {
    /* Word-sized swaps when possible */
    while (size >= sizeof(size_t)) {
        size_t t;
        memcpy(&t, a, sizeof(size_t));
        memcpy(a, b, sizeof(size_t));
        memcpy(b, &t, sizeof(size_t));
        a += sizeof(size_t);
        b += sizeof(size_t);
        size -= sizeof(size_t);
    }
    while (size--) {
        char t = *a;
        *a++ = *b;
        *b++ = t;
    }
}

static void ps_reverse_generic(void *base, size_t n, size_t size) {
    size_t i = 0, j = n;
    while (i < j) {
        --j;
        if (i >= j) break;
        ps_swap_bytes(ps_at_mut(base, size, i), ps_at_mut(base, size, j), size);
        ++i;
    }
}

void photonic_probe_generic(const void *base, size_t n, size_t size,
                            photonic_cmp_fn cmp, photonic_probe_t *out) {
    memset(out, 0, sizeof(*out));
    out->n = n;
    if (n <= 1) {
        out->max_run = n;
        out->run_count = n ? 1 : 0;
        out->confidence = 1.0;
        out->sortedness = 1.0;
        out->is_negative_delay = 1;
        out->monotone_sign = n ? 1 : 0;
        return;
    }

    size_t step;
    int full;
    if (n <= (size_t)PHOTONIC_SAMPLE_LIMIT) {
        step = 1;
        full = 1;
        out->confidence = 1.0;
    } else {
        step = n / (size_t)PHOTONIC_SAMPLE_LIMIT;
        if (step < 1) step = 1;
        full = 0;
        size_t n_idx = (n + step - 1) / step;
        out->confidence = (double)n_idx / (double)n;
    }

    size_t max_run_samples = 1, run_count = 1, direction_changes = 0;
    size_t equal_count = 0, current_run = 1;
    int prev_dir = 0;
    size_t inv_pairs = 0, total_pairs = 0, asc_edges = 0, desc_edges = 0;
    size_t prev_i = 0;
    size_t k = step;

    for (;;) {
        size_t j;
        if (full) {
            if (k >= n) break;
            j = k;
        } else {
            if (k >= n) {
                if (prev_i != n - 1)
                    j = n - 1;
                else
                    break;
            } else {
                j = k;
            }
        }

        int c = cmp(ps_at(base, size, prev_i), ps_at(base, size, j));
        total_pairs++;
        if (c > 0) {
            inv_pairs++;
            desc_edges++;
            if (prev_dir == 1) {
                direction_changes++;
                run_count++;
                if (current_run > max_run_samples) max_run_samples = current_run;
                current_run = 1;
            } else {
                current_run++;
            }
            prev_dir = -1;
        } else if (c < 0) {
            asc_edges++;
            if (prev_dir == -1) {
                direction_changes++;
                run_count++;
                if (current_run > max_run_samples) max_run_samples = current_run;
                current_run = 1;
            } else {
                current_run++;
            }
            prev_dir = 1;
        } else {
            equal_count++;
            current_run++;
        }
        if (current_run > max_run_samples) max_run_samples = current_run;

        prev_i = j;
        if (!full && j == n - 1 && (k >= n || k != j)) break;
        k += step;
        if (full && k >= n) break;
    }

    size_t max_run = ps_min_sz(n, max_run_samples * step);

    size_t extra = ps_min_sz((size_t)256, n / 4);
    if (extra > 0 && n > 2) {
        uint32_t state = (uint32_t)(n ^ 0x9E3779B9u);
        for (size_t e = 0; e < extra; e++) {
            size_t i = (size_t)(ps_lcg(&state) % (uint32_t)n);
            size_t j = (size_t)(ps_lcg(&state) % (uint32_t)n);
            if (i == j) continue;
            if (i > j) {
                size_t t = i;
                i = j;
                j = t;
            }
            total_pairs++;
            if (cmp(ps_at(base, size, i), ps_at(base, size, j)) > 0) inv_pairs++;
        }
    }

    double inv_ratio = total_pairs ? (double)inv_pairs / (double)total_pairs : 0.0;
    int monotone_sign = (desc_edges > 0 && asc_edges == 0)   ? -1
                        : (asc_edges > 0 && desc_edges == 0) ? 1
                                                            : 0;

    double run_fraction = (double)max_run / (double)n;
    double inv_term = 1.0 - (inv_ratio * 2.0 < 1.0 ? inv_ratio * 2.0 : 1.0);
    double dir_den = (double)ps_max_sz((size_t)1, n / 8);
    double dir_term = 1.0 - ((double)direction_changes / dir_den < 1.0
                                 ? (double)direction_changes / dir_den
                                 : 1.0);
    double sortedness = ps_clamp01(0.45 * inv_term + 0.35 * run_fraction + 0.20 * dir_term);

    int is_neg =
        (sortedness >= 0.72) || (max_run >= (size_t)(n * 0.45)) ||
        (direction_changes <= 3 && inv_ratio < 0.15) ||
        (max_run >= (size_t)(n * 0.25) && inv_ratio < 0.05) ||
        (monotone_sign != 0 && direction_changes == 0);

    out->inv_ratio = inv_ratio;
    out->max_run = max_run;
    out->run_count = run_count;
    out->direction_changes = direction_changes;
    out->equal_count = equal_count;
    out->group_delay_proxy = 1.0 - sortedness;
    out->sortedness = sortedness;
    out->is_negative_delay = is_neg ? 1 : 0;
    out->monotone_sign = monotone_sign;
}

/* Stable bottom-up mergesort for generic elements. */
static int ps_mergesort_generic(void *base, size_t n, size_t size, photonic_cmp_fn cmp) {
    if (n < 2) return 0;
    char *tmp = (char *)malloc(n * size);
    if (!tmp) return -1;
    char *a = (char *)base;

    for (size_t width = 1; width < n; width <<= 1) {
        for (size_t i = 0; i < n; i += width << 1) {
            size_t left = i;
            size_t mid = ps_min_sz(i + width, n);
            size_t right = ps_min_sz(i + (width << 1), n);
            if (mid >= right) continue;

            size_t p = left, q = mid, k = left;
            while (p < mid && q < right) {
                if (cmp(a + p * size, a + q * size) <= 0) {
                    memcpy(tmp + k * size, a + p * size, size);
                    p++;
                } else {
                    memcpy(tmp + k * size, a + q * size, size);
                    q++;
                }
                k++;
            }
            if (p < mid) memcpy(tmp + k * size, a + p * size, (mid - p) * size);
            if (q < right) memcpy(tmp + k * size, a + q * size, (right - q) * size);
            memcpy(a + left * size, tmp + left * size, (right - left) * size);
        }
    }
    free(tmp);
    return 0;
}

int photonic_sort(void *base, size_t n, size_t size, photonic_cmp_fn cmp) {
    if (n <= 1 || size == 0) return 0;

    photonic_probe_t probe;
    photonic_probe_generic(base, n, size, cmp, &probe);

    if (probe.is_negative_delay && probe.direction_changes == 0) {
        if (probe.monotone_sign == 1) return 1;
        if (probe.monotone_sign == -1) {
            ps_reverse_generic(base, n, size);
            return 1;
        }
    }

    if (ps_mergesort_generic(base, n, size, cmp) != 0) return -1;
    return probe.is_negative_delay ? 1 : 2;
}
