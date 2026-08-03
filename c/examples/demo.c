#define _POSIX_C_SOURCE 199309L
/* PhotonicSort C demo — marketing executable */
#include "../photonic_sort.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>

static double now_ms(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec * 1000.0 + (double)ts.tv_nsec / 1e6;
}

static int64_t *make_range(size_t n, int reverse) {
    int64_t *a = (int64_t *)malloc(n * sizeof(int64_t));
    if (!a) return NULL;
    for (size_t i = 0; i < n; i++)
        a[i] = reverse ? (int64_t)(n - i) : (int64_t)i;
    return a;
}

static int64_t *make_random(size_t n, uint32_t seed) {
    int64_t *a = (int64_t *)malloc(n * sizeof(int64_t));
    if (!a) return NULL;
    uint32_t s = seed ? seed : 1u;
    for (size_t i = 0; i < n; i++) {
        s = s * 1664525u + 1013904223u;
        a[i] = (int64_t)(s % 10001u);
    }
    return a;
}

static int64_t *make_sawtooth(size_t n, int period) {
    int64_t *a = (int64_t *)malloc(n * sizeof(int64_t));
    if (!a) return NULL;
    for (size_t i = 0; i < n; i++) a[i] = (int64_t)(i % (size_t)period);
    return a;
}

static int64_t *make_organpipe(size_t half) {
    size_t n = half * 2;
    int64_t *a = (int64_t *)malloc(n * sizeof(int64_t));
    if (!a) return NULL;
    for (size_t i = 0; i < half; i++) a[i] = (int64_t)i;
    for (size_t i = 0; i < half; i++) a[half + i] = (int64_t)(half - i);
    return a;
}

static const char *path_name(int code) {
    if (code == 0) return "trivial";
    if (code == 1) return "neg-time";
    if (code == 2) return "collapse";
    return "error";
}

static void run_case(const char *name, int64_t *data, size_t n) {
    photonic_probe_t p;
    photonic_probe_i64(data, n, &p);

    int64_t *work = (int64_t *)malloc(n * sizeof(int64_t));
    memcpy(work, data, n * sizeof(int64_t));

    double t0 = now_ms();
    int path = photonic_sort_i64(work, n);
    double ms = now_ms() - t0;

    int ok = photonic_is_sorted_i64(work, n);
    printf("%-18s %6zu  %-14s  %s  %10.3f\n", name, n, path_name(path),
           ok ? "True" : "FAIL", ms);
    free(work);
    free(data);
}

int main(void) {
    printf("========================================================================\n");
    printf("PhotonicSort %s — Give everything. Take nothing. Become photonic.\n",
           photonic_sort_version());
    printf("========================================================================\n\n");
    printf("Highly optimized C · stable residual · O(n) structure exits\n");
    printf("Research: arXiv:2409.03680 · metaphor only · no P=NP claim\n\n");
    printf("------------------------------------------------------------------------\n");
    printf("%-18s %6s  %-14s  %-4s  %10s\n", "Case", "n", "path", "ok", "time (ms)");
    printf("------------------------------------------------------------------------\n");

    run_case("already sorted", make_range(1000, 0), 1000);
    run_case("reverse sorted", make_range(1000, 1), 1000);
    run_case("random", make_random(2000, 42), 2000);
    run_case("sawtooth", make_sawtooth(1500, 50), 1500);
    run_case("organpipe", make_organpipe(500), 1000);
    run_case("few unique", make_random(1200, 7), 1200);

    {
        int64_t small[] = {7, 2, 9, 1, 5, 3, 8, 4, 6, 0};
        size_t n = sizeof(small) / sizeof(small[0]);
        printf("------------------------------------------------------------------------\n\n");
        printf("Verbose walk-through:\n  Input : [");
        for (size_t i = 0; i < n; i++) printf("%s%lld", i ? ", " : "", (long long)small[i]);
        printf("]\n");

        photonic_probe_t pr;
        photonic_probe_i64(small, n, &pr);
        printf("  [Photonic] Probe inv_ratio=%.3f max_run=%zu dir_changes=%zu "
               "sortedness=%.3f group_delay=%.3f\n",
               pr.inv_ratio, pr.max_run, pr.direction_changes, pr.sortedness,
               pr.group_delay_proxy);

        int path = photonic_sort_i64(small, n);
        printf("  [Photonic] path=%s\n  Output: [", path_name(path));
        for (size_t i = 0; i < n; i++) printf("%s%lld", i ? ", " : "", (long long)small[i]);
        printf("]\n  Verified sorted: %s\n\n", photonic_is_sorted_i64(small, n) ? "True" : "False");
    }

    {
        size_t n = 50000;
        int64_t *s = make_range(n, 0);
        int64_t *r = make_range(n, 1);
        double t0 = now_ms();
        photonic_sort_i64(s, n);
        double ts = now_ms() - t0;
        t0 = now_ms();
        photonic_sort_i64(r, n);
        double tr = now_ms() - t0;
        int ok = photonic_is_sorted_i64(s, n) && photonic_is_sorted_i64(r, n);
        printf("O(n) structure check (n=%zu): sorted=%.3f ms  reverse=%.3f ms\n", n, ts, tr);
        printf("  verified: %s\n\n", ok ? "True" : "False");
        free(s);
        free(r);
    }

    printf("Become photonic.\n");
    printf("========================================================================\n");
    return 0;
}
