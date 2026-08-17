#define _POSIX_C_SOURCE 199309L
/* PhotonicSort C11 — native vs libc qsort (stdlib) benchmark.
 * Build: make native-bench   (from c/)
 * EXTERNAL-clean. THE BEASTIE BOYZ
 */
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

static int cmp_i64(const void *a, const void *b) {
  int64_t x = *(const int64_t *)a, y = *(const int64_t *)b;
  return (x > y) - (x < y);
}

static int64_t *alloc_n(size_t n) {
  return (int64_t *)malloc(n * sizeof(int64_t));
}

static void fill_sorted(int64_t *a, size_t n) {
  for (size_t i = 0; i < n; i++) a[i] = (int64_t)i;
}
static void fill_reverse(int64_t *a, size_t n) {
  for (size_t i = 0; i < n; i++) a[i] = (int64_t)(n - i);
}
static void fill_random(int64_t *a, size_t n, uint32_t s) {
  for (size_t i = 0; i < n; i++) {
    s = s * 1664525u + 1013904223u;
    a[i] = (int64_t)(s % 1000003u);
  }
}
static void fill_sawtooth(int64_t *a, size_t n, int p) {
  for (size_t i = 0; i < n; i++) a[i] = (int64_t)(i % (size_t)p);
}
static void fill_almost(int64_t *a, size_t n) {
  fill_sorted(a, n);
  for (size_t i = 0; i < n; i += n / 100 + 1)
    if (i + 1 < n) {
      int64_t t = a[i];
      a[i] = a[i + 1];
      a[i + 1] = t;
    }
}
static void fill_few(int64_t *a, size_t n, int k) {
  uint32_t s = 7;
  for (size_t i = 0; i < n; i++) {
    s = s * 1664525u + 1013904223u;
    a[i] = (int64_t)(s % (uint32_t)k);
  }
}

static double time_photonic(int64_t *src, size_t n, int reps) {
  int64_t *w = alloc_n(n);
  double best = 1e99;
  for (int r = 0; r < reps; r++) {
    memcpy(w, src, n * sizeof(int64_t));
    double t0 = now_ms();
    photonic_sort_i64(w, n);
    double dt = now_ms() - t0;
    if (dt < best) best = dt;
    if (!photonic_is_sorted_i64(w, n)) {
      free(w);
      return -1.0;
    }
  }
  free(w);
  return best;
}

static double time_qsort(int64_t *src, size_t n, int reps) {
  int64_t *w = alloc_n(n);
  double best = 1e99;
  for (int r = 0; r < reps; r++) {
    memcpy(w, src, n * sizeof(int64_t));
    double t0 = now_ms();
    qsort(w, n, sizeof(int64_t), cmp_i64);
    double dt = now_ms() - t0;
    if (dt < best) best = dt;
  }
  free(w);
  return best;
}

int main(void) {
  const size_t N = 1000000;
  const int REPS = 5;
  int64_t *buf = alloc_n(N);
  if (!buf) return 1;

  printf("PhotonicSort %s  C11 native vs libc qsort  n=%zu reps=%d (best-of)\n",
         photonic_sort_version(), N, REPS);
  printf("%-14s %10s %10s %8s\n", "pattern", "photonic", "qsort", "ratio");

  const char *names[] = {"sorted", "reverse", "random", "sawtooth",
                         "almost", "few_k16"};
  for (int c = 0; c < 6; c++) {
    if (c == 0) fill_sorted(buf, N);
    else if (c == 1) fill_reverse(buf, N);
    else if (c == 2) fill_random(buf, N, 42);
    else if (c == 3) fill_sawtooth(buf, N, 64);
    else if (c == 4) fill_almost(buf, N);
    else fill_few(buf, N, 16);

    double tp = time_photonic(buf, N, REPS);
    double tq = time_qsort(buf, N, REPS);
    if (tp < 0) {
      printf("%-14s FAIL correctness\n", names[c]);
      continue;
    }
    printf("%-14s %9.3fms %9.3fms %7.3fx\n", names[c], tp, tq, tp / tq);
  }

  free(buf);
  return 0;
}
