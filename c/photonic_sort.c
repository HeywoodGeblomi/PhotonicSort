/* PhotonicSort C11 — adaptive hybrid sort
 * Brand contract: probe → structure early-exit → residual talent menu
 *
 * Residual menu (Geblomi + GyroRank-infused):
 *   - GyroRank pilot gate (observe → abort on clear random)
 *   - Joint merge gate (max_run ∧ low dir_change)
 *   - Capacity-checked run merge (patterned, half-buffer)
 *   - pdqsort-class introsort (random comparable path)
 *   - LSD radix on int64 (scalar random fast path)
 *   - Equal-heavy bias + confidence routing
 *   - force_collapse → stable mergesort
 *
 * Version 1.2.0-c  MIT
 */
#include "photonic_sort.h"

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#ifndef PS_INSERTION_LIMIT
#define PS_INSERTION_LIMIT 32u
#endif
#ifndef PS_MAX_TRACKED_RUNS
#define PS_MAX_TRACKED_RUNS 8u
#endif
#ifndef PS_NINTHER_THRESH
#define PS_NINTHER_THRESH 128u
#endif

static size_t ps_min_sz(size_t a, size_t b) { return a < b ? a : b; }
static size_t ps_max_sz(size_t a, size_t b) { return a > b ? a : b; }
static double ps_clamp01(double x) {
    if (x < 0.0) return 0.0;
    if (x > 1.0) return 1.0;
    return x;
}

static void ps_swap_i64(int64_t *a, int64_t *b) {
    int64_t t = *a;
    *a = *b;
    *b = t;
}

static void ps_reverse_i64(int64_t *restrict a, size_t n) {
    for (size_t i = 0, j = n; i < j; ++i) {
        --j;
        if (i >= j) break;
        ps_swap_i64(&a[i], &a[j]);
    }
}

static int ps_merge_eligible(const photonic_probe_t *pr) {
    if (pr->n < 2) return 0;
    double run_frac = (double)pr->max_run / (double)pr->n;
    size_t scale = pr->n > PHOTONIC_SAMPLE_LIMIT ? PHOTONIC_SAMPLE_LIMIT : pr->n;
    if (scale < 1) scale = 1;
    double dir_rate = (double)pr->direction_changes / (double)scale;
    int strong_runs = (pr->max_run > pr->n / 6) || (run_frac > 0.12);
    int low_flips = (dir_rate <= 0.12) ||
                    (pr->direction_changes <= ps_max_sz(4, scale / 16));
    return strong_runs && low_flips;
}

void photonic_probe_i64(const int64_t *restrict a, size_t n,
                        photonic_probe_t *restrict out) {
    memset(out, 0, sizeof(*out));
    out->n = n;
    out->confidence = 1.0;
    out->route = PHOTONIC_ROUTE_RANDOM;
    out->pilot_aborted = 0;
    out->pilot_samples = 0;
    if (n == 0) {
        out->sortedness = 1.0;
        out->is_negative_delay = 1;
        out->monotone_sign = 1;
        out->route = PHOTONIC_ROUTE_STRUCTURE;
        return;
    }
    if (n == 1) {
        out->max_run = 1;
        out->run_count = 1;
        out->sortedness = 1.0;
        out->is_negative_delay = 1;
        out->monotone_sign = 1;
        out->route = PHOTONIC_ROUTE_STRUCTURE;
        return;
    }

    /* GyroRank pilot: observe → gate (cheap random abort) */
    size_t pilot_n = n < 64 ? (n - 1) : (n < 2048 ? 64 : 256);
    if (pilot_n > n - 1) pilot_n = n - 1;
    size_t p_inv = 0, p_eq = 0, p_dc = 0, p_pairs = 0;
    int p_prev = 0;
    for (size_t i = 0; i < pilot_n; ++i) {
        int64_t x = a[i], y = a[i + 1];
        p_pairs++;
        if (y < x) p_inv++;
        if (y == x) p_eq++;
        int sg = (y > x) ? 1 : (y < x) ? -1 : 0;
        if (sg) {
            if (p_prev && sg != p_prev) p_dc++;
            if (sg) p_prev = sg;
        }
    }
    out->pilot_samples = p_pairs;
    double p_ir = p_pairs ? (double)p_inv / (double)p_pairs : 0.0;
    double p_dr = p_pairs ? (double)p_dc / (double)p_pairs : 0.0;
    double p_er = p_pairs ? (double)p_eq / (double)p_pairs : 0.0;

    if (p_pairs >= 32 && p_ir >= 0.42 && p_dr >= 0.28 && p_er < 0.20) {
        out->inv_ratio = p_ir;
        out->direction_changes = p_dc;
        out->equal_count = p_eq;
        out->max_run = 1;
        out->run_count = p_dc + 1;
        out->sortedness = ps_clamp01(1.0 - p_ir);
        out->group_delay_proxy = 1.0 - out->sortedness;
        out->is_negative_delay = 0;
        out->monotone_sign = 0;
        out->confidence = ps_clamp01(0.55 + (p_ir - 0.42) * 1.2);
        out->route = PHOTONIC_ROUTE_RANDOM;
        out->pilot_aborted = 1;
        return;
    }

    size_t limit = n;
    size_t step = 1;
    if (n > PHOTONIC_SAMPLE_LIMIT) {
        step = n / PHOTONIC_SAMPLE_LIMIT;
        if (step < 1) step = 1;
        limit = PHOTONIC_SAMPLE_LIMIT;
    }

    size_t inversions = 0, pairs = 0, max_run_samples = 1, run_count = 1;
    size_t direction_changes = 0, equal_count = 0, current_run = 1;
    int prev_sign = 0, monotone_sign = 0, broken_mono = 0;

    for (size_t s = 1; s < limit; ++s) {
        size_t i = s * step;
        if (i >= n) i = n - 1;
        size_t ip = (s - 1) * step;
        if (ip >= n) ip = n - 1;
        int64_t x = a[ip], y = a[i];
        pairs++;
        if (y < x) inversions++;
        if (y == x) equal_count++;
        int sign = (y > x) ? 1 : (y < x) ? -1 : 0;
        if (sign != 0) {
            if (prev_sign == 0) {
                prev_sign = sign;
                if (!broken_mono) monotone_sign = sign;
            } else if (sign != prev_sign) {
                direction_changes++;
                run_count++;
                if (current_run > max_run_samples) max_run_samples = current_run;
                current_run = 1;
                prev_sign = sign;
                broken_mono = 1;
                monotone_sign = 0;
            } else {
                current_run++;
            }
        } else {
            current_run++;
        }
    }
    if (current_run > max_run_samples) max_run_samples = current_run;

    if (step > 1) {
        size_t extra[] = {0, n / 4, n / 2, (3 * n) / 4, n > 0 ? n - 2 : 0};
        for (size_t k = 0; k < 5; ++k) {
            size_t i = extra[k];
            if (i + 1 >= n) continue;
            pairs++;
            if (a[i + 1] < a[i]) inversions++;
            if (a[i + 1] == a[i]) equal_count++;
        }
    }

    double inv_ratio = pairs ? (double)inversions / (double)pairs : 0.0;
    size_t max_run = ps_min_sz(n, max_run_samples * step);
    double equal_ratio = pairs ? (double)equal_count / (double)pairs : 0.0;
    if (equal_ratio > 0.45)
        inv_ratio *= (1.0 - 0.5 * equal_ratio);

    double inv_term = 1.0 - inv_ratio;
    double run_fraction = (double)max_run / (double)n;
    double dir_term = 1.0 / (1.0 + (double)direction_changes * 0.15);
    double sortedness = ps_clamp01(0.45 * inv_term + 0.35 * run_fraction + 0.20 * dir_term);
    if (equal_ratio > 0.6)
        sortedness = ps_clamp01(sortedness + 0.08);

    int is_neg =
        (sortedness >= 0.72) || (max_run >= (size_t)(n * 0.45)) ||
        (direction_changes <= 3 && inv_ratio < 0.15) ||
        (max_run >= (size_t)(n * 0.25) && inv_ratio < 0.05) ||
        (monotone_sign != 0 && direction_changes == 0);

    {
        double thr = 0.22;
        double dist = inv_ratio < thr ? (thr - inv_ratio) : (inv_ratio - thr);
        if (dist < 0) dist = -dist;
        double conf = 0.35 + dist / (thr + 1e-9) * 0.45;
        if (max_run > n / 4) conf += 0.1;
        if (equal_ratio > 0.5) conf += 0.15;
        double dir_rate = (double)direction_changes / (double)(limit > 0 ? limit : 1);
        if (max_run > n / 6 && dir_rate > 0.12) conf -= 0.12;
        out->confidence = ps_clamp01(conf);
    }

    out->inv_ratio = inv_ratio;
    out->max_run = max_run;
    out->run_count = run_count;
    out->direction_changes = direction_changes;
    out->equal_count = equal_count;
    out->group_delay_proxy = 1.0 - sortedness;
    out->sortedness = sortedness;
    out->is_negative_delay = is_neg ? 1 : 0;
    out->monotone_sign = monotone_sign;

    if (monotone_sign != 0 && direction_changes == 0)
        out->route = PHOTONIC_ROUTE_STRUCTURE;
    else if (ps_merge_eligible(out) || (sortedness >= 0.55 && max_run > n / 8))
        out->route = PHOTONIC_ROUTE_PATTERNED;
    else
        out->route = PHOTONIC_ROUTE_RANDOM;
}

static void ps_insertion_i64(int64_t *restrict a, size_t n) {
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

static void ps_merge_i64(int64_t *restrict a, int64_t *restrict tmp,
                         size_t left, size_t mid, size_t right) {
    size_t i = left, j = mid, k = left;
    while (i < mid && j < right) {
        if (a[i] <= a[j]) tmp[k++] = a[i++];
        else tmp[k++] = a[j++];
    }
    while (i < mid) tmp[k++] = a[i++];
    while (j < right) tmp[k++] = a[j++];
    for (size_t t = left; t < right; ++t) a[t] = tmp[t];
}

static int ps_mergesort_i64(int64_t *restrict a, size_t n) {
    if (n < 2) return 0;
    int64_t *tmp = (int64_t *)malloc(n * sizeof(int64_t));
    if (!tmp) return -1;
    for (size_t width = 1; width < n; width <<= 1) {
        for (size_t i = 0; i < n; i += width << 1) {
            size_t left = i, mid = ps_min_sz(i + width, n),
                   right = ps_min_sz(i + (width << 1), n);
            if (mid < right) ps_merge_i64(a, tmp, left, mid, right);
        }
    }
    free(tmp);
    return 0;
}

static int ps_run_merge_i64(int64_t *restrict a, size_t n) {
    if (n < 2) return 0;
    if (n <= PS_INSERTION_LIMIT) {
        ps_insertion_i64(a, n);
        return 0;
    }
    size_t starts[PS_MAX_TRACKED_RUNS + 1];
    size_t run_count = 0, i = 0;
    starts[0] = 0;
    while (i < n) {
        if (run_count >= PS_MAX_TRACKED_RUNS) return 1;
        size_t j = i + 1;
        if (j >= n) {
            starts[++run_count] = n;
            break;
        }
        if (a[j] >= a[i]) {
            while (j + 1 < n && a[j + 1] >= a[j]) ++j;
            ++j;
        } else {
            while (j + 1 < n && a[j + 1] < a[j]) ++j;
            ++j;
            ps_reverse_i64(a + i, j - i);
        }
        starts[++run_count] = j;
        i = j;
        if (run_count > PS_MAX_TRACKED_RUNS) return 1;
    }
    if (run_count <= 1) return 0;
    size_t buf_cap = n / 2 + 1;
    int64_t *buf = (int64_t *)malloc(buf_cap * sizeof(int64_t));
    if (!buf) return -1;
    for (size_t width = 1; width < run_count; width <<= 1) {
        for (size_t ri = 0; ri + width < run_count; ri += 2 * width) {
            size_t left = starts[ri], mid = starts[ri + width];
            size_t right = (ri + 2 * width <= run_count) ? starts[ri + 2 * width]
                                                         : starts[run_count];
            size_t len_l = mid - left, len_r = right - mid;
            if (len_l == 0 || len_r == 0) continue;
            if (len_l <= len_r) {
                if (len_l > buf_cap) {
                    int64_t *tmp = (int64_t *)malloc((len_l + len_r) * sizeof(int64_t));
                    if (!tmp) { free(buf); return -1; }
                    size_t p = left, q = mid, k = 0;
                    while (p < mid && q < right)
                        tmp[k++] = (a[p] <= a[q]) ? a[p++] : a[q++];
                    while (p < mid) tmp[k++] = a[p++];
                    while (q < right) tmp[k++] = a[q++];
                    memcpy(a + left, tmp, k * sizeof(int64_t));
                    free(tmp);
                } else {
                    memcpy(buf, a + left, len_l * sizeof(int64_t));
                    size_t bi = 0, q = mid, k = left;
                    while (bi < len_l && q < right)
                        a[k++] = (a[q] < buf[bi]) ? a[q++] : buf[bi++];
                    while (bi < len_l) a[k++] = buf[bi++];
                }
            } else {
                if (len_r > buf_cap) {
                    int64_t *tmp = (int64_t *)malloc((len_l + len_r) * sizeof(int64_t));
                    if (!tmp) { free(buf); return -1; }
                    size_t p = left, q = mid, k = 0;
                    while (p < mid && q < right)
                        tmp[k++] = (a[p] <= a[q]) ? a[p++] : a[q++];
                    while (p < mid) tmp[k++] = a[p++];
                    while (q < right) tmp[k++] = a[q++];
                    memcpy(a + left, tmp, k * sizeof(int64_t));
                    free(tmp);
                } else {
                    memcpy(buf, a + mid, len_r * sizeof(int64_t));
                    size_t bi = len_r, p = mid, k = right;
                    while (bi > 0 && p > left) {
                        if (buf[bi - 1] < a[p - 1]) a[--k] = a[--p];
                        else a[--k] = buf[--bi];
                    }
                    while (bi > 0) a[--k] = buf[--bi];
                }
            }
        }
    }
    free(buf);
    return 0;
}

static size_t ps_ilog2(size_t n) {
    size_t r = 0;
    while (n >>= 1) ++r;
    return r;
}
static int64_t ps_median3(int64_t a, int64_t b, int64_t c) {
    if (a < b) {
        if (b < c) return b;
        return (a < c) ? c : a;
    }
    if (a < c) return a;
    return (b < c) ? c : b;
}
static void ps_sift_down_i64(int64_t *a, size_t root, size_t end) {
    while (1) {
        size_t child = root * 2 + 1;
        if (child >= end) break;
        if (child + 1 < end && a[child] < a[child + 1]) child++;
        if (a[root] >= a[child]) break;
        ps_swap_i64(&a[root], &a[child]);
        root = child;
    }
}
static void ps_heapsort_i64(int64_t *a, size_t n) {
    if (n < 2) return;
    for (size_t i = n / 2; i-- > 0;) ps_sift_down_i64(a, i, n);
    for (size_t end = n; end-- > 1;) {
        ps_swap_i64(&a[0], &a[end]);
        ps_sift_down_i64(a, 0, end);
    }
}
static void ps_introsort_i64(int64_t *a, size_t n, unsigned depth) {
    while (n > PS_INSERTION_LIMIT) {
        if (depth == 0) {
            ps_heapsort_i64(a, n);
            return;
        }
        --depth;
        size_t m = n / 2;
        if (n >= PS_NINTHER_THRESH) {
            size_t q = n / 4;
            int64_t p1 = ps_median3(a[0], a[q], a[m / 2]);
            int64_t p2 = ps_median3(a[m - q / 2], a[m], a[m + q / 2]);
            int64_t p3 = ps_median3(a[n - 1 - q], a[n - 1 - m / 2], a[n - 1]);
            int64_t piv = ps_median3(p1, p2, p3);
            for (size_t i = 0; i < n; ++i)
                if (a[i] == piv) {
                    ps_swap_i64(&a[i], &a[m]);
                    break;
                }
        } else {
            int64_t piv = ps_median3(a[0], a[m], a[n - 1]);
            if (a[m] != piv) {
                if (a[0] == piv) ps_swap_i64(&a[0], &a[m]);
                else ps_swap_i64(&a[n - 1], &a[m]);
            }
        }
        int64_t pivot = a[m];
        size_t lo = 0, hi = n;
        while (lo < hi) {
            if (a[lo] < pivot) ++lo;
            else if (a[hi - 1] > pivot) --hi;
            else if (a[lo] > pivot) {
                ps_swap_i64(&a[lo], &a[hi - 1]);
                --hi;
            } else ++lo;
        }
        size_t first_ge = 0;
        while (first_ge < n && a[first_ge] < pivot) ++first_ge;
        size_t first_gt = first_ge;
        while (first_gt < n && a[first_gt] <= pivot) ++first_gt;
        if (first_ge > 1) ps_introsort_i64(a, first_ge, depth);
        if (first_gt < n) ps_introsort_i64(a + first_gt, n - first_gt, depth);
        return;
    }
    ps_insertion_i64(a, n);
}
static void ps_pdq_i64(int64_t *a, size_t n) {
    if (n < 2) return;
    ps_introsort_i64(a, n, (unsigned)(2 * ps_ilog2(n + 1) + 1));
}

static int ps_radix_i64(int64_t *restrict a, size_t n) {
    if (n < 2) return 0;
    if (n <= PS_INSERTION_LIMIT) {
        ps_insertion_i64(a, n);
        return 0;
    }
    uint64_t *u = (uint64_t *)a;
    for (size_t i = 0; i < n; ++i) u[i] ^= (uint64_t)1 << 63;
    uint64_t *tmp = (uint64_t *)malloc(n * sizeof(uint64_t));
    if (!tmp) {
        for (size_t i = 0; i < n; ++i) u[i] ^= (uint64_t)1 << 63;
        return -1;
    }
    size_t count[256];
    for (unsigned shift = 0; shift < 64; shift += 8) {
        memset(count, 0, sizeof(count));
        for (size_t i = 0; i < n; ++i) count[(u[i] >> shift) & 0xFFu]++;
        size_t sum = 0;
        for (unsigned b = 0; b < 256; ++b) {
            size_t c = count[b];
            count[b] = sum;
            sum += c;
        }
        for (size_t i = 0; i < n; ++i) {
            unsigned b = (unsigned)((u[i] >> shift) & 0xFFu);
            tmp[count[b]++] = u[i];
        }
        memcpy(u, tmp, n * sizeof(uint64_t));
    }
    free(tmp);
    for (size_t i = 0; i < n; ++i) u[i] ^= (uint64_t)1 << 63;
    return 0;
}

static int ps_random_residual_i64(int64_t *a, size_t n, const photonic_probe_t *pr) {
    if (pr->pilot_aborted && n >= 64) {
        if (ps_radix_i64(a, n) != 0) ps_pdq_i64(a, n);
        return 0;
    }
    double eq = (pr->n > 0 && pr->equal_count > 0)
                    ? (double)pr->equal_count /
                          (double)(pr->n > PHOTONIC_SAMPLE_LIMIT ? PHOTONIC_SAMPLE_LIMIT
                                                                 : pr->n)
                    : 0.0;
    if (eq > 0.35 || pr->confidence < 0.35 || n < 64) {
        ps_pdq_i64(a, n);
        return 0;
    }
    if (ps_radix_i64(a, n) != 0) {
        ps_pdq_i64(a, n);
        return 0;
    }
    return 0;
}

static int photonic_sort_i64_impl(int64_t *restrict a, size_t n, int force_collapse) {
    if (n <= 1) return 0;
    if (force_collapse) {
        if (n <= PS_INSERTION_LIMIT) {
            ps_insertion_i64(a, n);
            return 2;
        }
        if (ps_mergesort_i64(a, n) != 0) return -1;
        return 2;
    }

    photonic_probe_t probe;
    photonic_probe_i64(a, n, &probe);

    if (probe.direction_changes == 0 && probe.monotone_sign == 1) return 1;
    if (probe.direction_changes == 0 && probe.monotone_sign == -1) {
        ps_reverse_i64(a, n);
        return 1;
    }

    if (probe.pilot_aborted || probe.route == PHOTONIC_ROUTE_RANDOM) {
        if (ps_random_residual_i64(a, n, &probe) != 0) return -1;
        return 2;
    }

    if (probe.route == PHOTONIC_ROUTE_PATTERNED || ps_merge_eligible(&probe)) {
        int rm = ps_run_merge_i64(a, n);
        if (rm == 0) return 2;
        if (rm < 0) return -1;
    }

    if (probe.sortedness >= 0.55 && probe.confidence >= 0.5 &&
        probe.max_run > probe.n / 8) {
        int rm = ps_run_merge_i64(a, n);
        if (rm == 0) return 2;
        if (rm < 0) return -1;
    }

    if (ps_random_residual_i64(a, n, &probe) != 0) return -1;
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
    if (n == 0) return 0;
    if (src != dst) memcpy(dst, src, n * sizeof(int64_t));
    return photonic_sort_i64(dst, n);
}
int photonic_is_sorted_i64(const int64_t *a, size_t n) {
    for (size_t i = 1; i < n; ++i)
        if (a[i] < a[i - 1]) return 0;
    return 1;
}
const char *photonic_sort_version(void) { return PHOTONIC_SORT_VERSION_STRING; }

void photonic_probe_generic(const void *base, size_t n, size_t size,
                            photonic_cmp_fn cmp, photonic_probe_t *out) {
    memset(out, 0, sizeof(*out));
    out->n = n;
    out->confidence = 1.0;
    out->route = PHOTONIC_ROUTE_RANDOM;
    if (n <= 1) {
        out->sortedness = 1.0;
        out->is_negative_delay = 1;
        out->monotone_sign = 1;
        out->max_run = n;
        out->run_count = n ? 1 : 0;
        out->route = PHOTONIC_ROUTE_STRUCTURE;
        return;
    }
    const char *a = (const char *)base;
    size_t limit = n > PHOTONIC_SAMPLE_LIMIT ? PHOTONIC_SAMPLE_LIMIT : n;
    size_t step = n > PHOTONIC_SAMPLE_LIMIT ? n / PHOTONIC_SAMPLE_LIMIT : 1;
    if (step < 1) step = 1;
    size_t inversions = 0, pairs = 0, equal_count = 0;
    size_t max_run_samples = 1, run_count = 1, direction_changes = 0, current_run = 1;
    int prev_sign = 0, monotone_sign = 0, broken = 0;
    for (size_t s = 1; s < limit; ++s) {
        size_t i = s * step;
        if (i >= n) i = n - 1;
        size_t ip = (s - 1) * step;
        if (ip >= n) ip = n - 1;
        int c = cmp(a + i * size, a + ip * size);
        pairs++;
        if (c < 0) inversions++;
        if (c == 0) equal_count++;
        int sign = (c > 0) ? 1 : (c < 0) ? -1 : 0;
        if (sign != 0) {
            if (prev_sign == 0) {
                prev_sign = sign;
                if (!broken) monotone_sign = sign;
            } else if (sign != prev_sign) {
                direction_changes++;
                run_count++;
                if (current_run > max_run_samples) max_run_samples = current_run;
                current_run = 1;
                prev_sign = sign;
                broken = 1;
                monotone_sign = 0;
            } else
                current_run++;
        } else
            current_run++;
    }
    if (current_run > max_run_samples) max_run_samples = current_run;
    double inv_ratio = pairs ? (double)inversions / (double)pairs : 0.0;
    double equal_ratio = pairs ? (double)equal_count / (double)pairs : 0.0;
    if (equal_ratio > 0.45) inv_ratio *= (1.0 - 0.5 * equal_ratio);
    size_t max_run = ps_min_sz(n, max_run_samples * step);
    double sortedness = ps_clamp01(0.45 * (1.0 - inv_ratio) + 0.35 * ((double)max_run / (double)n) +
                                   0.20 * (1.0 / (1.0 + (double)direction_changes * 0.15)));
    out->inv_ratio = inv_ratio;
    out->max_run = max_run;
    out->run_count = run_count;
    out->direction_changes = direction_changes;
    out->equal_count = equal_count;
    out->sortedness = sortedness;
    out->group_delay_proxy = 1.0 - sortedness;
    out->is_negative_delay =
        (sortedness >= 0.72) || (monotone_sign != 0 && direction_changes == 0);
    out->monotone_sign = monotone_sign;
    out->confidence = 0.7;
    if (monotone_sign && !direction_changes)
        out->route = PHOTONIC_ROUTE_STRUCTURE;
    else
        out->route = PHOTONIC_ROUTE_RANDOM;
}

static void ps_reverse_generic(void *base, size_t n, size_t size) {
    char *a = (char *)base;
    char *tmp = (char *)malloc(size);
    if (!tmp) return;
    for (size_t i = 0, j = n; i < j; ++i) {
        --j;
        if (i >= j) break;
        memcpy(tmp, a + i * size, size);
        memcpy(a + i * size, a + j * size, size);
        memcpy(a + j * size, tmp, size);
    }
    free(tmp);
}

static int ps_mergesort_generic(void *base, size_t n, size_t size, photonic_cmp_fn cmp) {
    if (n < 2) return 0;
    char *tmp = (char *)malloc(n * size);
    if (!tmp) return -1;
    char *a = (char *)base;
    for (size_t width = 1; width < n; width <<= 1) {
        for (size_t i = 0; i < n; i += width << 1) {
            size_t left = i, mid = ps_min_sz(i + width, n),
                   right = ps_min_sz(i + (width << 1), n);
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
    if (probe.direction_changes == 0 && probe.monotone_sign == 1) return 1;
    if (probe.direction_changes == 0 && probe.monotone_sign == -1) {
        ps_reverse_generic(base, n, size);
        return 1;
    }
    if (ps_mergesort_generic(base, n, size, cmp) != 0) return -1;
    return 2;
}
