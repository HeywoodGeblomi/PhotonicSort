/*
 * PhotonicSort v1.3.2-c — mode sensitivity panel
 * NORMAL vs AGGRESSIVE vs FORCE_HOLE on almost-sorted / sparse-swap families.
 *
 * Build:  cc -O3 -std=c11 -I. sensitivity_modes.c photonic_sort.c -o sensitivity_modes
 * Run:    ./sensitivity_modes [n]
 */
#define _POSIX_C_SOURCE 200809L
#include "photonic_sort.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>

#define N_DEFAULT 100000u
#define TRIALS 24
#define SWAP_POINTS 12

static uint64_t rng_state = 0xC0FFEEULL;
static uint64_t rng_next(void) {
    rng_state ^= rng_state << 13;
    rng_state ^= rng_state >> 7;
    rng_state ^= rng_state << 17;
    return rng_state;
}
static int cmp_i64(const void *a, const void *b) {
    int64_t x = *(const int64_t *)a, y = *(const int64_t *)b;
    return (x > y) - (x < y);
}
static void fill_sorted(int64_t *a, size_t n) {
    for (size_t i = 0; i < n; ++i) a[i] = (int64_t)i;
}
static void apply_k_swaps(int64_t *a, size_t n, size_t k) {
    for (size_t s = 0; s < k; ++s) {
        size_t i = (size_t)(rng_next() % n);
        size_t j = (size_t)(rng_next() % n);
        if (i == j) continue;
        int64_t t = a[i]; a[i] = a[j]; a[j] = t;
    }
}
static double wall_ms(struct timespec a, struct timespec b) {
    return (b.tv_sec - a.tv_sec) * 1e3 + (b.tv_nsec - a.tv_nsec) * 1e-6;
}
static int cmp_double(const void *a, const void *b) {
    double x = *(const double *)a, y = *(const double *)b;
    return (x > y) - (x < y);
}
typedef struct {
    int path_hist[4];
    int ok, fail, n_times;
    double times[TRIALS];
} mode_stats_t;

static void run_band(size_t n, size_t n_swaps, photonic_sort_mode_t mode, mode_stats_t *st) {
    memset(st, 0, sizeof(*st));
    int64_t *src = (int64_t *)malloc(n * sizeof(int64_t));
    int64_t *buf = (int64_t *)malloc(n * sizeof(int64_t));
    int64_t *ref = (int64_t *)malloc(n * sizeof(int64_t));
    if (!src || !buf || !ref) { fprintf(stderr, "OOM\n"); exit(1); }
    for (int t = 0; t < TRIALS; ++t) {
        fill_sorted(src, n);
        apply_k_swaps(src, n, n_swaps);
        memcpy(buf, src, n * sizeof(int64_t));
        memcpy(ref, src, n * sizeof(int64_t));
        struct timespec t0, t1;
        clock_gettime(CLOCK_MONOTONIC, &t0);
        int path = photonic_sort_i64_ex(buf, n, mode);
        clock_gettime(CLOCK_MONOTONIC, &t1);
        st->times[st->n_times++] = wall_ms(t0, t1);
        if (path >= 0 && path <= 2) st->path_hist[path]++;
        else st->path_hist[3]++;
        qsort(ref, n, sizeof(int64_t), cmp_i64);
        int good = 1;
        for (size_t i = 0; i < n; ++i) if (buf[i] != ref[i]) { good = 0; break; }
        if (good) st->ok++; else st->fail++;
    }
    free(src); free(buf); free(ref);
}
static double median(double *v, int n) {
    if (n <= 0) return 0.0;
    qsort(v, (size_t)n, sizeof(double), cmp_double);
    if (n & 1) return v[n / 2];
    return 0.5 * (v[n / 2 - 1] + v[n / 2]);
}
static void print_row(const char *mode, size_t n_swaps, double swap_frac, const mode_stats_t *st) {
    int hole = st->path_hist[1], residual = st->path_hist[2];
    int other = st->path_hist[0] + st->path_hist[3];
    double hole_rate = 100.0 * hole / (double)TRIALS;
    double med = median((double *)st->times, st->n_times);
    printf("%-10s  %7zu  %7.4f  %5d  %5d  %5d  %6.1f%%  %7.3f  %3d/%-3d\n",
           mode, n_swaps, swap_frac, hole, residual, other, hole_rate, med, st->ok, st->fail);
}

int main(int argc, char **argv) {
    size_t n = N_DEFAULT;
    if (argc > 1) n = (size_t)strtoull(argv[1], NULL, 10);
    if (n < 64) n = 64;
    printf("PhotonicSort %s — mode sensitivity panel\n", photonic_sort_version());
    printf("n = %zu, trials = %d per cell\n", n, TRIALS);
    printf("Generators: sorted + exact k pairwise swaps (sparse disorder)\n\n");
    double fracs[SWAP_POINTS] = {
        0.0000, 0.0005, 0.0010, 0.0020, 0.0040, 0.0060,
        0.0080, 0.0100, 0.0200, 0.0400, 0.0800, 0.1200
    };
    photonic_sort_mode_t modes[3] = {
        PHOTONIC_MODE_NORMAL, PHOTONIC_MODE_AGGRESSIVE, PHOTONIC_MODE_FORCE_HOLE
    };
    const char *mode_names[3] = { "NORMAL", "AGGRESSIVE", "FORCE_HOLE" };
    printf("%-10s  %7s  %7s  %5s  %5s  %5s  %7s  %7s  %7s\n",
           "mode", "k_swaps", "frac", "hole", "resid", "other", "hole%", "med_ms", "ok/fail");
    printf("----------  -------  -------  -----  -----  -----  -------  -------  -------\n");
    for (int fi = 0; fi < SWAP_POINTS; ++fi) {
        size_t k = (size_t)(fracs[fi] * (double)n + 0.5);
        if (fi > 0 && k == 0) k = 1;
        for (int mi = 0; mi < 3; ++mi) {
            mode_stats_t st;
            rng_state = 0xC0FFEEULL ^ ((uint64_t)fi << 16) ^ (uint64_t)mi;
            run_band(n, k, modes[mi], &st);
            print_row(mode_names[mi], k, (double)k / (double)n, &st);
        }
        printf("\n");
    }
    printf("Legend: hole = path code 1 (STRUCTURE early-exit, verified)\n");
    printf("        resid = path code 2 (residual menu)\n");
    return 0;
}
