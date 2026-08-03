/* PhotonicSort C unit tests */
#include "../photonic_sort.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static int g_fail = 0;

#define EXPECT(cond, msg)                                                      \
    do {                                                                       \
        if (!(cond)) {                                                         \
            fprintf(stderr, "FAIL %s:%d: %s\n", __FILE__, __LINE__, msg);     \
            g_fail++;                                                          \
        }                                                                      \
    } while (0)

static int cmp_i64(const void *a, const void *b) {
    int64_t x = *(const int64_t *)a;
    int64_t y = *(const int64_t *)b;
    return (x > y) - (x < y);
}

static void test_empty_singleton(void) {
    int64_t one = 42;
    EXPECT(photonic_sort_i64(NULL, 0) == 0, "empty");
    EXPECT(photonic_sort_i64(&one, 1) == 0, "singleton");
    EXPECT(one == 42, "singleton value");
}

static void test_sorted(void) {
    int64_t a[200];
    for (int i = 0; i < 200; i++) a[i] = i;
    photonic_probe_t p;
    photonic_probe_i64(a, 200, &p);
    EXPECT(p.is_negative_delay, "sorted neg delay");
    EXPECT(p.monotone_sign == 1, "sorted sign");
    EXPECT(photonic_sort_i64(a, 200) == 1, "sorted path");
    EXPECT(photonic_is_sorted_i64(a, 200), "still sorted");
}

static void test_reverse(void) {
    int64_t a[200];
    for (int i = 0; i < 200; i++) a[i] = 200 - i;
    photonic_probe_t p;
    photonic_probe_i64(a, 200, &p);
    EXPECT(p.is_negative_delay, "rev neg delay");
    EXPECT(p.monotone_sign == -1, "rev sign");
    photonic_sort_i64(a, 200);
    EXPECT(photonic_is_sorted_i64(a, 200), "rev sorted");
    for (int i = 0; i < 200; i++) EXPECT(a[i] == i + 1, "rev values");
}

static void test_random(void) {
    int64_t a[500];
    uint32_t s = 1;
    for (int i = 0; i < 500; i++) {
        s = s * 1664525u + 1013904223u;
        a[i] = (int64_t)(s % 10000);
    }
    photonic_sort_i64(a, 500);
    EXPECT(photonic_is_sorted_i64(a, 500), "random sorted");
}

static void test_duplicates_stable_order_ok(void) {
    int64_t a[] = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
    size_t n = sizeof(a) / sizeof(a[0]);
    photonic_sort_i64(a, n);
    EXPECT(photonic_is_sorted_i64(a, n), "dups sorted");
}

static void test_force_collapse(void) {
    int64_t a[] = {7, 2, 9, 1, 5, 3, 8, 4, 6, 0};
    photonic_sort_i64_force_collapse(a, 10);
    for (int i = 0; i < 10; i++) EXPECT(a[i] == i, "collapse values");
}

static void test_probe_deterministic(void) {
    int64_t a[800];
    uint32_t s = 99;
    for (int i = 0; i < 800; i++) {
        s = s * 1664525u + 1013904223u;
        a[i] = (int64_t)(s % 1000);
    }
    photonic_probe_t x, y;
    photonic_probe_i64(a, 800, &x);
    photonic_probe_i64(a, 800, &y);
    EXPECT(x.inv_ratio == y.inv_ratio, "det inv");
    EXPECT(x.max_run == y.max_run, "det run");
    EXPECT(x.is_negative_delay == y.is_negative_delay, "det flag");
}

static void test_generic(void) {
    int64_t a[] = {5, 1, 4, 2, 3};
    photonic_sort(a, 5, sizeof(int64_t), cmp_i64);
    EXPECT(photonic_is_sorted_i64(a, 5), "generic sorted");
}

static void test_copy(void) {
    int64_t src[] = {3, 1, 2};
    int64_t dst[3];
    photonic_sort_i64_copy(src, dst, 3);
    EXPECT(src[0] == 3 && src[1] == 1 && src[2] == 2, "src intact");
    EXPECT(dst[0] == 1 && dst[1] == 2 && dst[2] == 3, "dst sorted");
}

static void test_large_structure(void) {
    size_t n = 20000;
    int64_t *a = (int64_t *)malloc(n * sizeof(int64_t));
    for (size_t i = 0; i < n; i++) a[i] = (int64_t)i;
    photonic_sort_i64(a, n);
    EXPECT(photonic_is_sorted_i64(a, n), "large sorted");
    for (size_t i = 0; i < n; i++) a[i] = (int64_t)(n - i);
    photonic_sort_i64(a, n);
    EXPECT(photonic_is_sorted_i64(a, n), "large reverse");
    free(a);
}

int main(void) {
    printf("PhotonicSort C tests (%s)\n", photonic_sort_version());
    test_empty_singleton();
    test_sorted();
    test_reverse();
    test_random();
    test_duplicates_stable_order_ok();
    test_force_collapse();
    test_probe_deterministic();
    test_generic();
    test_copy();
    test_large_structure();
    if (g_fail) {
        printf("%d failure(s)\n", g_fail);
        return 1;
    }
    printf("All tests passed.\n");
    return 0;
}
