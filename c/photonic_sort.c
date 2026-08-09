/* PhotonicSort C11 — adaptive hybrid sort
 * Brand contract: probe → structure early-exit → residual talent menu
 *
 * Residual menu (Geblomi + GyroRank — Plan A):
 *   - GyroRank pilot gate (observe → abort on clear random)
 *   - LOW_CARD counting residual (few distinct / small range)
 *   - LOW_DISORDER insertion/pdq (almost sorted; cache-local insertion)
 *   - Joint merge gate + capacity-checked run merge (≤32 runs)
 *   - pdqsort-class introsort + LSD radix (int64)
 *   - force_collapse → stable mergesort
 *
 * Version 1.3.2-c — Plan A + Aggressive/ForceHole opt-in modes  MIT
 * Contributors: Heywood Geblomi · Grok (xAI)
 */
#include "photonic_sort.h"

/* ---- Sort mode state (default NORMAL; ForceHole is opt-in) ---- */
static photonic_sort_mode_t g_ps_mode = PHOTONIC_MODE_NORMAL;

void photonic_sort_set_mode(photonic_sort_mode_t mode) {
    if (mode < PHOTONIC_MODE_NORMAL || mode > PHOTONIC_MODE_FORCE_HOLE)
        mode = PHOTONIC_MODE_NORMAL;
    g_ps_mode = mode;
}
photonic_sort_mode_t photonic_sort_get_mode(void) { return g_ps_mode; }
const char *photonic_sort_mode_name(photonic_sort_mode_t mode) {
    switch (mode) {
    case PHOTONIC_MODE_NORMAL:     return "NORMAL";
    case PHOTONIC_MODE_AGGRESSIVE: return "AGGRESSIVE";
    case PHOTONIC_MODE_FORCE_HOLE: return "FORCE_HOLE";
    default: return "UNKNOWN";
    }
}

/* Threshold table keyed by mode.
 * NORMAL     = production baseline (moderate safety-first)
 * AGGRESSIVE = widened early-exit band
 * FORCE_HOLE = maximum hole-in-one attempt (STRUCTURE verify still mandatory)
 */
typedef struct ps_thresh {
    double pilot_ld_inv_hi;   /* pilot LOW_DISORDER inv upper */
    double pilot_ld_dir;      /* pilot LOW_DISORDER dir_rate upper */
    double full_ld_inv;       /* full-probe LOW_DISORDER inv */
    double full_ld_sort;      /* full-probe LOW_DISORDER sortedness */
    size_t low_card_unique;   /* unique_est ceiling for LOW_CARD */
    uint64_t low_card_range;  /* dense range ceiling */
    double pat_sortedness;    /* PATTERNED sortedness floor */
    size_t pat_run_div;       /* max_run > n/pat_run_div */
    double rand_inv;          /* RANDOM pilot abort inv lower */
    double rand_dir;          /* RANDOM pilot abort dir_rate lower */
    double residual_ld_inv;   /* late residual LOW_DISORDER inv */
    double residual_ld_sort;  /* late residual LOW_DISORDER sortedness */
} ps_thresh_t;

static ps_thresh_t ps_thresh_for(photonic_sort_mode_t mode) {
    ps_thresh_t t;
    if (mode == PHOTONIC_MODE_FORCE_HOLE) {
        /* ForceHole: most aggressive; bets on structure / low-card */
        t.pilot_ld_inv_hi  = 0.18;
        t.pilot_ld_dir     = 0.28;
        t.full_ld_inv      = 0.16;
        t.full_ld_sort     = 0.65;
        t.low_card_unique  = 1024;
        t.low_card_range   = 2048;
        t.pat_sortedness   = 0.40;
        t.pat_run_div      = 16;
        t.rand_inv         = 0.60;
        t.rand_dir         = 0.40;
        t.residual_ld_inv  = 0.18;
        t.residual_ld_sort = 0.65;
    } else if (mode == PHOTONIC_MODE_AGGRESSIVE) {
        t.pilot_ld_inv_hi  = 0.14;
        t.pilot_ld_dir     = 0.22;
        t.full_ld_inv      = 0.12;
        t.full_ld_sort     = 0.70;
        t.low_card_unique  = 512;
        t.low_card_range   = 1024;
        t.pat_sortedness   = 0.45;
        t.pat_run_div      = 12;
        t.rand_inv         = 0.55;
        t.rand_dir         = 0.35;
        t.residual_ld_inv  = 0.12;
        t.residual_ld_sort = 0.70;
    } else {
        /* NORMAL — current production baseline */
        t.pilot_ld_inv_hi  = 0.08;
        t.pilot_ld_dir     = 0.12;
        t.full_ld_inv      = 0.06;
        t.full_ld_sort     = 0.85;
        t.low_card_unique  = 256;
        t.low_card_range   = 512;
        t.pat_sortedness   = 0.55;
        t.pat_run_div      = 8;
        t.rand_inv         = 0.42;
        t.rand_dir         = 0.28;
        t.residual_ld_inv  = 0.06;
        t.residual_ld_sort = 0.80;
    }
    return t;
}


#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#ifndef PS_INSERTION_LIMIT
#define PS_INSERTION_LIMIT 32u
#endif
#ifndef PS_MAX_TRACKED_RUNS
#define PS_MAX_TRACKED_RUNS 32u
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

#define PS_USET_SLOTS 128u
#define PS_USET_MASK  (PS_USET_SLOTS - 1u)
typedef struct {
    int64_t keys[PS_USET_SLOTS];
    uint8_t used[PS_USET_SLOTS];
    size_t  count;
    int     overflow;
} ps_uset_t;
static void ps_uset_init(ps_uset_t *s) {
    memset(s->used, 0, sizeof(s->used));
    s->count = 0; s->overflow = 0;
}
static void ps_uset_add(ps_uset_t *s, int64_t k) {
    if (s->overflow) return;
    uint32_t h = (uint32_t)((uint64_t)k * 11400714819323198485ull);
    for (uint32_t i = 0; i < PS_USET_SLOTS; ++i) {
        uint32_t idx = (h + i) & PS_USET_MASK;
        if (!s->used[idx]) {
            if (s->count + 1 >= PS_USET_SLOTS - 8) { s->overflow = 1; return; }
            s->used[idx] = 1; s->keys[idx] = k; s->count++; return;
        }
        if (s->keys[idx] == k) return;
    }
    s->overflow = 1;
}

static void ps_swap_i64(int64_t *a, int64_t *b) {
    int64_t t = *a; *a = *b; *b = t;
}
static void ps_reverse_i64(int64_t *restrict a, size_t n) {
    for (size_t i = 0, j = n; i < j; ++i) {
        --j; if (i >= j) break;
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

/* Cache-locality insertion: early-continue + single backward shift. */
static void ps_insertion_i64(int64_t *restrict a, size_t n) {
    if (n < 2) return;
    for (size_t i = 1; i < n; ++i) {
        if (a[i] >= a[i - 1]) continue;
        int64_t key = a[i];
        size_t j = i;
        do { a[j] = a[j - 1]; --j; } while (j > 0 && a[j - 1] > key);
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
    if (n <= PS_INSERTION_LIMIT) { ps_insertion_i64(a, n); return 0; }
    size_t starts[PS_MAX_TRACKED_RUNS + 1], run_count = 0, i = 0;
    starts[0] = 0;
    while (i < n) {
        if (run_count >= PS_MAX_TRACKED_RUNS) return 1;
        size_t j = i + 1;
        if (j >= n) { starts[++run_count] = n; break; }
        if (a[j] >= a[i]) { while (j + 1 < n && a[j + 1] >= a[j]) ++j; ++j; }
        else { while (j + 1 < n && a[j + 1] < a[j]) ++j; ++j; ps_reverse_i64(a + i, j - i); }
        starts[++run_count] = j; i = j;
    }
    if (run_count <= 1) return 0;
    size_t buf_cap = n / 2 + 1;
    int64_t *buf = (int64_t *)malloc(buf_cap * sizeof(int64_t));
    if (!buf) return -1;
    for (size_t width = 1; width < run_count; width <<= 1) {
        for (size_t ri = 0; ri + width < run_count; ri += 2 * width) {
            size_t left = starts[ri], mid = starts[ri + width];
            size_t right = (ri + 2 * width <= run_count) ? starts[ri + 2 * width] : starts[run_count];
            size_t len_l = mid - left, len_r = right - mid;
            if (!len_l || !len_r) continue;
            int64_t *tmp = (int64_t *)malloc((len_l + len_r) * sizeof(int64_t));
            if (!tmp) { free(buf); return -1; }
            size_t p = left, q = mid, k = 0;
            while (p < mid && q < right) tmp[k++] = (a[p] <= a[q]) ? a[p++] : a[q++];
            while (p < mid) tmp[k++] = a[p++];
            while (q < right) tmp[k++] = a[q++];
            memcpy(a + left, tmp, k * sizeof(int64_t));
            free(tmp);
        }
    }
    free(buf);
    return 0;
}
static size_t ps_ilog2(size_t n) { size_t r = 0; while (n >>= 1) ++r; return r; }
static void ps_sift_down_i64(int64_t *a, size_t root, size_t end) {
    while (1) {
        size_t child = root * 2 + 1;
        if (child >= end) break;
        if (child + 1 < end && a[child] < a[child + 1]) child++;
        if (a[root] >= a[child]) break;
        ps_swap_i64(&a[root], &a[child]); root = child;
    }
}
static void ps_heapsort_i64(int64_t *a, size_t n) {
    if (n < 2) return;
    for (size_t i = n / 2; i-- > 0;) ps_sift_down_i64(a, i, n);
    for (size_t end = n; end-- > 1;) { ps_swap_i64(&a[0], &a[end]); ps_sift_down_i64(a, 0, end); }
}
static void ps_introsort_i64(int64_t *a, size_t n, unsigned depth) {
    while (n > PS_INSERTION_LIMIT) {
        if (depth == 0) { ps_heapsort_i64(a, n); return; }
        --depth;
        size_t m = n / 2;
        if (a[0] > a[m]) ps_swap_i64(&a[0], &a[m]);
        if (a[m] > a[n - 1]) ps_swap_i64(&a[m], &a[n - 1]);
        if (a[0] > a[m]) ps_swap_i64(&a[0], &a[m]);
        int64_t pivot = a[m];
        size_t i = 0, j = n - 1;
        for (;;) {
            while (a[i] < pivot) ++i;
            while (a[j] > pivot) --j;
            if (i >= j) break;
            ps_swap_i64(&a[i], &a[j]); ++i; if (j == 0) break; --j;
        }
        size_t mid = j + 1;
        if (mid == 0) mid = 1;
        if (mid >= n) mid = n - 1;
        if (mid > 1) ps_introsort_i64(a, mid, depth);
        a += mid; n -= mid;
    }
    ps_insertion_i64(a, n);
}
static void ps_pdq_i64(int64_t *a, size_t n) {
    if (n < 2) return;
    ps_introsort_i64(a, n, (unsigned)(2 * ps_ilog2(n + 1) + 1));
}
static int ps_radix_i64(int64_t *restrict a, size_t n) {
    if (n < 2) return 0;
    if (n <= PS_INSERTION_LIMIT) { ps_insertion_i64(a, n); return 0; }
    uint64_t *u = (uint64_t *)a;
    for (size_t i = 0; i < n; ++i) u[i] ^= (uint64_t)1 << 63;
    uint64_t *tmp = (uint64_t *)malloc(n * sizeof(uint64_t));
    if (!tmp) { for (size_t i = 0; i < n; ++i) u[i] ^= (uint64_t)1 << 63; return -1; }
    size_t count[256];
    for (unsigned shift = 0; shift < 64; shift += 8) {
        memset(count, 0, sizeof(count));
        for (size_t i = 0; i < n; ++i) count[(u[i] >> shift) & 0xFFu]++;
        size_t sum = 0;
        for (unsigned b = 0; b < 256; ++b) { size_t c = count[b]; count[b] = sum; sum += c; }
        for (size_t i = 0; i < n; ++i) { unsigned b = (unsigned)((u[i] >> shift) & 0xFFu); tmp[count[b]++] = u[i]; }
        memcpy(u, tmp, n * sizeof(uint64_t));
    }
    free(tmp);
    for (size_t i = 0; i < n; ++i) u[i] ^= (uint64_t)1 << 63;
    return 0;
}
static int ps_counting_i64(int64_t *restrict a, size_t n) {
    if (n < 2) return 0;
    if (n <= PS_INSERTION_LIMIT) { ps_insertion_i64(a, n); return 0; }
    int64_t mn = a[0], mx = a[0];
    for (size_t i = 1; i < n; ++i) { if (a[i] < mn) mn = a[i]; if (a[i] > mx) mx = a[i]; }
    if (mn == mx) return 0;
    uint64_t range = (uint64_t)(mx - mn);
    if (range + 1 <= 4096u || (range + 1 <= n * 4 && range + 1 <= (1u << 20))) {
        size_t bins = (size_t)range + 1;
        size_t *cnt = (size_t *)calloc(bins, sizeof(size_t));
        if (!cnt) return -1;
        for (size_t i = 0; i < n; ++i) cnt[(size_t)(a[i] - mn)]++;
        size_t idx = 0;
        for (size_t v = 0; v < bins; ++v) {
            int64_t key = mn + (int64_t)v;
            for (size_t k = 0; k < cnt[v]; ++k) a[idx++] = key;
        }
        free(cnt);
        return 0;
    }
    return ps_radix_i64(a, n);
}

void photonic_probe_i64(const int64_t *restrict a, size_t n,
                        photonic_probe_t *restrict out) {
    photonic_probe_i64_ex(a, n, out, g_ps_mode);
}

void photonic_probe_i64_ex(const int64_t *restrict a, size_t n,
                           photonic_probe_t *restrict out,
                           photonic_sort_mode_t mode) {
    memset(out, 0, sizeof(*out));
    out->n = n; out->confidence = 1.0; out->route = PHOTONIC_ROUTE_RANDOM;
    ps_thresh_t th = ps_thresh_for(mode);
    if (n == 0) {
        out->sortedness = 1.0; out->is_negative_delay = 1; out->monotone_sign = 1;
        out->route = PHOTONIC_ROUTE_STRUCTURE; return;
    }
    if (n == 1) {
        out->max_run = 1; out->run_count = 1; out->sortedness = 1.0;
        out->is_negative_delay = 1; out->monotone_sign = 1;
        out->route = PHOTONIC_ROUTE_STRUCTURE;
        out->unique_est = 1; out->sample_min = out->sample_max = a[0]; return;
    }
    size_t pilot_n = n < 64 ? (n - 1) : (n < 2048 ? 64 : 256);
    if (pilot_n > n - 1) pilot_n = n - 1;
    ps_uset_t uset; ps_uset_init(&uset);
    size_t p_inv = 0, p_eq = 0, p_dc = 0, p_pairs = 0; int p_prev = 0;
    int64_t smin = a[0], smax = a[0]; ps_uset_add(&uset, a[0]);
    for (size_t i = 0; i < pilot_n; ++i) {
        int64_t x = a[i], y = a[i + 1]; p_pairs++;
        if (y < x) p_inv++; if (y == x) p_eq++;
        int sg = (y > x) ? 1 : (y < x) ? -1 : 0;
        if (sg) { if (p_prev && sg != p_prev) p_dc++; p_prev = sg; }
        if (y < smin) smin = y; if (y > smax) smax = y; ps_uset_add(&uset, y);
    }
    if (n > pilot_n + 1) {
        size_t extra[] = {n / 8, n / 4, n / 2, (3 * n) / 4, n - 1};
        for (size_t k = 0; k < 5; ++k) {
            if (extra[k] >= n) continue;
            int64_t v = a[extra[k]];
            if (v < smin) smin = v; if (v > smax) smax = v; ps_uset_add(&uset, v);
        }
    }
    out->pilot_samples = p_pairs; out->unique_est = uset.count;
    out->sample_min = smin; out->sample_max = smax;
    double p_ir = p_pairs ? (double)p_inv / (double)p_pairs : 0.0;
    double p_dr = p_pairs ? (double)p_dc / (double)p_pairs : 0.0;
    double p_er = p_pairs ? (double)p_eq / (double)p_pairs : 0.0;
    /* Fully monotone pilot → STRUCTURE only if multi-point sample agrees.
     * Prefix-only monotone (e.g. organpipe ascending half) must NOT early-exit. */
    if (p_pairs >= 8 && p_dc == 0 && p_prev != 0 &&
        (p_inv == 0 || p_inv == p_pairs)) {
        int ends_agree = 0;
        size_t m = n / 2, q = (3 * n) / 4;
        if (m >= n) m = n - 1;
        if (q >= n) q = n - 1;
        if (p_inv == 0) {
            /* ascending candidate: endpoints + mid + 3/4 must be nondecreasing */
            if (a[n - 1] >= a[0] && a[m] >= a[0] && a[q] >= a[m] && a[n - 1] >= a[q])
                ends_agree = 1;
        } else {
            /* descending candidate */
            if (a[n - 1] <= a[0] && a[m] <= a[0] && a[q] <= a[m] && a[n - 1] <= a[q])
                ends_agree = 1;
        }
        if (ends_agree) {
            out->inv_ratio = p_ir;
            out->direction_changes = 0;
            out->equal_count = p_eq;
            out->max_run = n;
            out->run_count = 1;
            out->sortedness = (p_inv == 0) ? 1.0 : 0.0;
            out->group_delay_proxy = 1.0 - out->sortedness;
            out->confidence = 0.95;
            out->monotone_sign = p_prev;
            out->is_negative_delay = 1;
            out->route = PHOTONIC_ROUTE_STRUCTURE;
            out->pilot_aborted = 1;
            return;
        }
    }
    int low_card = 0;
    if (!uset.overflow && uset.count > 0 && uset.count <= th.low_card_unique) low_card = 1;
    else if (smax > smin) {
        uint64_t range = (uint64_t)(smax - smin);
        if (range <= th.low_card_range && range + 1 <= (uint64_t)n) low_card = 1;
    }
    int low_disorder = (p_pairs >= 32 && p_ir > 0.002 && p_ir <= th.pilot_ld_inv_hi
                        && p_dr <= th.pilot_ld_dir && !low_card);
    if (low_card) {
        out->inv_ratio = p_ir; out->direction_changes = p_dc; out->equal_count = p_eq;
        out->max_run = 1; out->run_count = p_dc + 1;
        out->sortedness = ps_clamp01(1.0 - p_ir); out->group_delay_proxy = 1.0 - out->sortedness;
        out->confidence = 0.85; out->route = PHOTONIC_ROUTE_LOW_CARD; out->pilot_aborted = 1; return;
    }
    if (low_disorder) {
        out->inv_ratio = p_ir; out->direction_changes = p_dc; out->equal_count = p_eq;
        out->max_run = (size_t)((1.0 - p_ir) * (double)n); out->run_count = p_dc + 1;
        out->sortedness = ps_clamp01(1.0 - p_ir); out->group_delay_proxy = 1.0 - out->sortedness;
        out->confidence = 0.8; out->route = PHOTONIC_ROUTE_LOW_DISORDER; out->pilot_aborted = 1; return;
    }
    if (p_pairs >= 32 && p_ir >= th.rand_inv && p_dr >= th.rand_dir && p_er < 0.20) {
        out->inv_ratio = p_ir; out->direction_changes = p_dc; out->equal_count = p_eq;
        out->max_run = 1; out->run_count = p_dc + 1;
        out->sortedness = ps_clamp01(1.0 - p_ir); out->group_delay_proxy = 1.0 - out->sortedness;
        out->confidence = ps_clamp01(0.55 + (p_ir - 0.42) * 1.2);
        out->route = PHOTONIC_ROUTE_RANDOM; out->pilot_aborted = 1; return;
    }
    size_t limit = n, step = 1;
    if (n > PHOTONIC_SAMPLE_LIMIT) {
        step = n / PHOTONIC_SAMPLE_LIMIT; if (step < 1) step = 1; limit = PHOTONIC_SAMPLE_LIMIT;
    }
    size_t inversions = 0, pairs = 0, max_run_samples = 1, run_count = 1;
    size_t direction_changes = 0, equal_count = 0, current_run = 1;
    int prev_sign = 0, monotone_sign = 0, broken_mono = 0;
    for (size_t s = 1; s < limit; ++s) {
        size_t i = s * step; if (i >= n) i = n - 1;
        size_t ip = (s - 1) * step; if (ip >= n) ip = n - 1;
        int64_t x = a[ip], y = a[i]; pairs++;
        if (y < x) inversions++; if (y == x) equal_count++;
        int sign = (y > x) ? 1 : (y < x) ? -1 : 0;
        if (sign != 0) {
            if (prev_sign == 0) { prev_sign = sign; if (!broken_mono) monotone_sign = sign; }
            else if (sign != prev_sign) {
                direction_changes++; run_count++;
                if (current_run > max_run_samples) max_run_samples = current_run;
                current_run = 1; prev_sign = sign; broken_mono = 1; monotone_sign = 0;
            } else current_run++;
        } else current_run++;
    }
    if (current_run > max_run_samples) max_run_samples = current_run;
    double inv_ratio = pairs ? (double)inversions / (double)pairs : 0.0;
    size_t max_run = ps_min_sz(n, max_run_samples * step);
    double equal_ratio = pairs ? (double)equal_count / (double)pairs : 0.0;
    if (equal_ratio > 0.45) inv_ratio *= (1.0 - 0.5 * equal_ratio);
    double sortedness = ps_clamp01(0.45 * (1.0 - inv_ratio) + 0.35 * ((double)max_run / (double)n) +
                                   0.20 * (1.0 / (1.0 + (double)direction_changes * 0.15)));
    if (equal_ratio > 0.6) sortedness = ps_clamp01(sortedness + 0.08);
    out->confidence = 0.7; out->inv_ratio = inv_ratio; out->max_run = max_run;
    out->run_count = run_count; out->direction_changes = direction_changes;
    out->equal_count = equal_count; out->sortedness = sortedness;
    out->group_delay_proxy = 1.0 - sortedness; out->monotone_sign = monotone_sign;
    out->is_negative_delay = (sortedness >= 0.72) || (monotone_sign && !direction_changes);
    if (monotone_sign && !direction_changes) out->route = PHOTONIC_ROUTE_STRUCTURE;
    else if (inv_ratio <= th.full_ld_inv && sortedness >= th.full_ld_sort)
        out->route = PHOTONIC_ROUTE_LOW_DISORDER;
    else if (ps_merge_eligible(out) ||
             (sortedness >= th.pat_sortedness && max_run > n / th.pat_run_div))
        out->route = PHOTONIC_ROUTE_PATTERNED;
    else out->route = PHOTONIC_ROUTE_RANDOM;
}

static int ps_random_residual_i64(int64_t *a, size_t n, const photonic_probe_t *pr) {
    if (pr->route == PHOTONIC_ROUTE_LOW_CARD || (pr->unique_est > 0 && pr->unique_est <= 256)) {
        if (ps_counting_i64(a, n) != 0) ps_pdq_i64(a, n); return 0;
    }
    if (pr->pilot_aborted && pr->route == PHOTONIC_ROUTE_RANDOM && n >= 64) {
        if (ps_radix_i64(a, n) != 0) ps_pdq_i64(a, n); return 0;
    }
    if (n < 64) { ps_pdq_i64(a, n); return 0; }
    if (ps_radix_i64(a, n) != 0) ps_pdq_i64(a, n);
    return 0;
}

/* Try STRUCTURE early-exit with mandatory O(n) verification.
 * Returns 1 on success (sorted or reversed), 0 if not fully structured. */
static int ps_try_structure_verified(int64_t *restrict a, size_t n,
                                     const photonic_probe_t *probe) {
    if (probe->direction_changes == 0 && probe->monotone_sign == 1) {
        if (photonic_is_sorted_i64(a, n)) return 1;
        return 0;
    }
    if (probe->direction_changes == 0 && probe->monotone_sign == -1) {
        int fully_rev = 1;
        for (size_t i = 1; i < n; ++i) {
            if (a[i] > a[i - 1]) { fully_rev = 0; break; }
        }
        if (fully_rev) { ps_reverse_i64(a, n); return 1; }
        return 0;
    }
    return 0;
}

static int ps_try_low_disorder(int64_t *restrict a, size_t n) {
    if (n <= 4096) ps_insertion_i64(a, n); else ps_pdq_i64(a, n);
    return 2;
}

static int ps_try_low_card(int64_t *restrict a, size_t n) {
    if (ps_counting_i64(a, n) != 0) ps_pdq_i64(a, n);
    return 2;
}

static int photonic_sort_i64_impl(int64_t *restrict a, size_t n,
                                  int force_collapse, photonic_sort_mode_t mode) {
    if (n <= 1) return 0;
    if (force_collapse) {
        if (n <= PS_INSERTION_LIMIT) { ps_insertion_i64(a, n); return 2; }
        return ps_mergesort_i64(a, n) ? -1 : 2;
    }
    ps_thresh_t th = ps_thresh_for(mode);
    photonic_probe_t probe; photonic_probe_i64_ex(a, n, &probe, mode);

    /* ---- FORCE_HOLE ladder: bet on structure / low-card first ---- */
    if (mode == PHOTONIC_MODE_FORCE_HOLE) {
        /* 1. Structure (verified) — true hole-in-one */
        if (ps_try_structure_verified(a, n, &probe)) return 1;

        /* 2. Low-card only when probe actually classified LOW_CARD
         *    (unique_est alone is unreliable: 128-slot set saturates ~119). */
        if (probe.route == PHOTONIC_ROUTE_LOW_CARD) {
            return ps_try_low_card(a, n);
        }

        /* 3. Relaxed low-disorder attempt */
        size_t scale = probe.n > PHOTONIC_SAMPLE_LIMIT ? PHOTONIC_SAMPLE_LIMIT : probe.n;
        if (scale < 1) scale = 1;
        double dir_rate = (double)probe.direction_changes / (double)scale;
        if (probe.route == PHOTONIC_ROUTE_LOW_DISORDER ||
            (probe.inv_ratio <= 0.18 && dir_rate <= 0.28 && probe.sortedness >= 0.55)) {
            return ps_try_low_disorder(a, n);
        }
        /* 4. Fall through to full residual */
    }

    /* ---- NORMAL / AGGRESSIVE (and ForceHole fallback) ---- */
    /* Structure early-exit only after O(n) verification (pilot can false-positive). */
    if (ps_try_structure_verified(a, n, &probe)) return 1;

    if (probe.route == PHOTONIC_ROUTE_LOW_DISORDER) {
        return ps_try_low_disorder(a, n);
    }
    if (probe.route == PHOTONIC_ROUTE_LOW_CARD) {
        return ps_try_low_card(a, n);
    }
    if (probe.pilot_aborted || probe.route == PHOTONIC_ROUTE_RANDOM) {
        return ps_random_residual_i64(a, n, &probe) ? -1 : 2;
    }
    if (probe.route == PHOTONIC_ROUTE_PATTERNED || ps_merge_eligible(&probe)) {
        int rm = ps_run_merge_i64(a, n);
        if (rm == 0) return 2; if (rm < 0) return -1;
        if (probe.unique_est > 0 && probe.unique_est <= th.low_card_unique
            && ps_counting_i64(a, n) == 0) return 2;
    }
    if (probe.sortedness >= th.pat_sortedness && probe.confidence >= 0.5
        && probe.max_run > probe.n / th.pat_run_div) {
        int rm = ps_run_merge_i64(a, n);
        if (rm == 0) return 2; if (rm < 0) return -1;
    }
    if (probe.inv_ratio <= th.residual_ld_inv && probe.sortedness >= th.residual_ld_sort) {
        return ps_try_low_disorder(a, n);
    }
    return ps_random_residual_i64(a, n, &probe) ? -1 : 2;
}

int photonic_sort_i64(int64_t *restrict a, size_t n) {
    return photonic_sort_i64_impl(a, n, 0, g_ps_mode);
}
int photonic_sort_i64_ex(int64_t *restrict a, size_t n, photonic_sort_mode_t mode) {
    return photonic_sort_i64_impl(a, n, 0, mode);
}
int photonic_sort_i64_force_collapse(int64_t *restrict a, size_t n) {
    return photonic_sort_i64_impl(a, n, 1, g_ps_mode);
}
int photonic_sort_i64_copy(const int64_t *restrict src, int64_t *restrict dst, size_t n) {
    if (n == 0) return 0;
    if (src != dst) memcpy(dst, src, n * sizeof(int64_t));
    return photonic_sort_i64(dst, n);
}
int photonic_is_sorted_i64(const int64_t *a, size_t n) {
    for (size_t i = 1; i < n; ++i) if (a[i] < a[i - 1]) return 0;
    return 1;
}
const char *photonic_sort_version(void) { return PHOTONIC_SORT_VERSION_STRING; }

void photonic_probe_generic(const void *base, size_t n, size_t size, photonic_cmp_fn cmp, photonic_probe_t *out) {
    memset(out, 0, sizeof(*out)); out->n = n; out->confidence = 0.7; out->route = PHOTONIC_ROUTE_RANDOM;
    if (n <= 1) {
        out->sortedness = 1.0; out->is_negative_delay = 1; out->monotone_sign = 1;
        out->max_run = n; out->run_count = n ? 1 : 0; out->route = PHOTONIC_ROUTE_STRUCTURE; return;
    }
    const char *a = (const char *)base;
    size_t limit = n > PHOTONIC_SAMPLE_LIMIT ? PHOTONIC_SAMPLE_LIMIT : n;
    size_t step = n > PHOTONIC_SAMPLE_LIMIT ? n / PHOTONIC_SAMPLE_LIMIT : 1;
    if (step < 1) step = 1;
    size_t inversions = 0, pairs = 0, equal_count = 0, max_run_samples = 1, run_count = 1, direction_changes = 0, current_run = 1;
    int prev_sign = 0, monotone_sign = 0, broken = 0;
    for (size_t s = 1; s < limit; ++s) {
        size_t i = s * step; if (i >= n) i = n - 1;
        size_t ip = (s - 1) * step; if (ip >= n) ip = n - 1;
        int c = cmp(a + i * size, a + ip * size); pairs++;
        if (c < 0) inversions++; if (c == 0) equal_count++;
        int sign = c > 0 ? 1 : c < 0 ? -1 : 0;
        if (sign) {
            if (!prev_sign) { prev_sign = sign; if (!broken) monotone_sign = sign; }
            else if (sign != prev_sign) {
                direction_changes++; run_count++;
                if (current_run > max_run_samples) max_run_samples = current_run;
                current_run = 1; prev_sign = sign; broken = 1; monotone_sign = 0;
            } else current_run++;
        } else current_run++;
    }
    if (current_run > max_run_samples) max_run_samples = current_run;
    double inv_ratio = pairs ? (double)inversions / (double)pairs : 0.0;
    size_t max_run = ps_min_sz(n, max_run_samples * step);
    out->inv_ratio = inv_ratio; out->max_run = max_run; out->run_count = run_count;
    out->direction_changes = direction_changes; out->equal_count = equal_count;
    out->sortedness = ps_clamp01(1.0 - inv_ratio); out->monotone_sign = monotone_sign;
    out->is_negative_delay = (monotone_sign && !direction_changes);
    out->route = (monotone_sign && !direction_changes) ? PHOTONIC_ROUTE_STRUCTURE : PHOTONIC_ROUTE_RANDOM;
}
static void ps_reverse_generic(void *base, size_t n, size_t size) {
    char *a = (char *)base; char *tmp = (char *)malloc(size); if (!tmp) return;
    for (size_t i = 0, j = n; i < j;) { --j; if (i >= j) break;
        memcpy(tmp, a + i * size, size); memcpy(a + i * size, a + j * size, size);
        memcpy(a + j * size, tmp, size); ++i; }
    free(tmp);
}
static int ps_mergesort_generic(void *base, size_t n, size_t size, photonic_cmp_fn cmp) {
    if (n < 2) return 0;
    char *tmp = (char *)malloc(n * size); if (!tmp) return -1;
    char *a = (char *)base;
    for (size_t width = 1; width < n; width <<= 1)
        for (size_t i = 0; i < n; i += width << 1) {
            size_t left = i, mid = ps_min_sz(i + width, n), right = ps_min_sz(i + (width << 1), n);
            if (mid >= right) continue;
            size_t p = left, q = mid, k = left;
            while (p < mid && q < right) {
                if (cmp(a + p * size, a + q * size) <= 0) { memcpy(tmp + k * size, a + p * size, size); p++; }
                else { memcpy(tmp + k * size, a + q * size, size); q++; }
                k++;
            }
            if (p < mid) memcpy(tmp + k * size, a + p * size, (mid - p) * size);
            if (q < right) memcpy(tmp + k * size, a + q * size, (right - q) * size);
            memcpy(a + left * size, tmp + left * size, (right - left) * size);
        }
    free(tmp); return 0;
}
int photonic_sort(void *base, size_t n, size_t size, photonic_cmp_fn cmp) {
    if (n <= 1 || size == 0) return 0;
    photonic_probe_t probe; photonic_probe_generic(base, n, size, cmp, &probe);
    if (probe.direction_changes == 0 && probe.monotone_sign == 1) return 1;
    if (probe.direction_changes == 0 && probe.monotone_sign == -1) { ps_reverse_generic(base, n, size); return 1; }
    return ps_mergesort_generic(base, n, size, cmp) ? -1 : 2;
}
