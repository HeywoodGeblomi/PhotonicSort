/* PHO-ACE-001 Phase A — C product API oracle vs libc qsort
 * Entry point: photonic_sort_i64 (and generic) as shipped in c/
 * H0: does not modify sort body. EXTERNAL-clean. THE BEASTIE BOYZ
 *
 * Exit 0 + ALL_CORRECTNESS_OK=1 only when every cell matches reference.
 * CI hard-fails on non-zero.
 */
#define _POSIX_C_SOURCE 200809L
#include "../photonic_sort.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>

static int g_fail = 0;
static int g_cells = 0;
static int g_pass = 0;

static int cmp_i64(const void *a, const void *b) {
  int64_t x = *(const int64_t *)a;
  int64_t y = *(const int64_t *)b;
  return (x > y) - (x < y);
}

static int64_t *alloc_n(size_t n) {
  if (n == 0) return NULL;
  int64_t *p = (int64_t *)malloc(n * sizeof(int64_t));
  if (!p) {
    fprintf(stderr, "OOM n=%zu\n", n);
    exit(2);
  }
  return p;
}

static void fill_sorted(int64_t *a, size_t n) {
  for (size_t i = 0; i < n; i++) a[i] = (int64_t)i;
}
static void fill_reverse(int64_t *a, size_t n) {
  for (size_t i = 0; i < n; i++) a[i] = (int64_t)(n - 1 - i);
}
static void fill_all_equal(int64_t *a, size_t n) {
  for (size_t i = 0; i < n; i++) a[i] = 42;
}
static void fill_random(int64_t *a, size_t n, uint32_t seed) {
  uint32_t s = seed;
  for (size_t i = 0; i < n; i++) {
    s = s * 1664525u + 1013904223u;
    a[i] = (int64_t)(int32_t)s; /* full range signed */
  }
}
static void fill_organpipe(int64_t *a, size_t n) {
  size_t half = n / 2;
  for (size_t i = 0; i < half; i++) a[i] = (int64_t)i;
  for (size_t i = half; i < n; i++) a[i] = (int64_t)(n - 1 - i);
}
static void fill_few_unique(int64_t *a, size_t n, int k) {
  uint32_t s = 7;
  for (size_t i = 0; i < n; i++) {
    s = s * 1664525u + 1013904223u;
    a[i] = (int64_t)(s % (uint32_t)k);
  }
}
static void fill_adversarial(int64_t *a, size_t n) {
  /* hash-like anti-quicksort pattern */
  for (size_t i = 0; i < n; i++)
    a[i] = (int64_t)((i * 2654435761ull) >> 32);
}
static void fill_almost_sorted(int64_t *a, size_t n) {
  fill_sorted(a, n);
  /* sparse swaps */
  for (size_t i = 0; i + 1 < n; i += (n / 500) + 1) {
    int64_t t = a[i];
    a[i] = a[i + 1];
    a[i + 1] = t;
  }
}

/* Oracle cell: photonic vs qsort, exact multiset+order match */
static void oracle_cell(const char *name, int64_t *src, size_t n) {
  g_cells++;
  int64_t *pho = alloc_n(n);
  int64_t *ref = alloc_n(n);
  if (n > 0) {
    memcpy(pho, src, n * sizeof(int64_t));
    memcpy(ref, src, n * sizeof(int64_t));
  }

  int path = photonic_sort_i64(pho, n);
  if (path < 0) {
    fprintf(stderr, "FAIL %s: photonic returned alloc-fail path=%d\n", name, path);
    g_fail++;
    free(pho); free(ref);
    return;
  }

  if (n > 0)
    qsort(ref, n, sizeof(int64_t), cmp_i64);

  int ok = 1;
  if (n > 0 && memcmp(pho, ref, n * sizeof(int64_t)) != 0)
    ok = 0;
  if (!photonic_is_sorted_i64(pho, n))
    ok = 0;

  if (!ok) {
    fprintf(stderr, "FAIL %s n=%zu path=%d content or sorted mismatch vs qsort\n",
            name, n, path);
    /* print first few diffs */
    size_t shown = 0;
    for (size_t i = 0; i < n && shown < 8; i++) {
      if (pho[i] != ref[i]) {
        fprintf(stderr, "  @%zu pho=%lld ref=%lld\n", i,
                (long long)pho[i], (long long)ref[i]);
        shown++;
      }
    }
    g_fail++;
  } else {
    g_pass++;
    printf("OK   %-18s n=%zu path=%d\n", name, n, path);
  }
  free(pho);
  free(ref);
}

static void oracle_generic_small(void) {
  g_cells++;
  int64_t a[] = {5, 1, 4, 2, 3, 0, 9, 8, 7, 6};
  size_t n = sizeof(a) / sizeof(a[0]);
  int64_t ref[10];
  memcpy(ref, a, sizeof(a));
  photonic_sort(a, n, sizeof(int64_t), cmp_i64);
  qsort(ref, n, sizeof(int64_t), cmp_i64);
  if (memcmp(a, ref, sizeof(a)) != 0 || !photonic_is_sorted_i64(a, n)) {
    fprintf(stderr, "FAIL generic small content mismatch\n");
    g_fail++;
  } else {
    g_pass++;
    printf("OK   %-18s n=%zu\n", "generic", n);
  }
}

int main(int argc, char **argv) {
  size_t N = 100000; /* CI default ≤1e5 */
  for (int i = 1; i < argc; i++) {
    if (!strcmp(argv[i], "--n") && i + 1 < argc)
      N = (size_t)strtoull(argv[++i], NULL, 10);
  }
  if (N > 2000000) N = 2000000; /* hard safety */

  printf("PHO-ACE-001 Phase A oracle  PhotonicSort %s  N=%zu\n",
         photonic_sort_version(), N);
  printf("Entry: photonic_sort_i64 + photonic_sort (product C API)\n");
  printf("Reference: libc qsort (same comparator)\n\n");

  /* trivial */
  oracle_cell("empty", NULL, 0);
  int64_t one = 42;
  oracle_cell("n1", &one, 1);
  if (one != 42) {
    fprintf(stderr, "FAIL n1 mutated value\n");
    g_fail++;
  }

  int64_t *buf = alloc_n(N);

  fill_sorted(buf, N);
  oracle_cell("sorted", buf, N);

  fill_reverse(buf, N);
  oracle_cell("reverse", buf, N);

  fill_all_equal(buf, N);
  oracle_cell("all_equal", buf, N);

  fill_random(buf, N, 42);
  oracle_cell("random", buf, N);

  fill_organpipe(buf, N);
  oracle_cell("organpipe", buf, N);

  fill_few_unique(buf, N, 4);
  oracle_cell("few_unique_k4", buf, N);

  fill_few_unique(buf, N, 16);
  oracle_cell("few_unique_k16", buf, N);

  fill_adversarial(buf, N);
  oracle_cell("adversarial", buf, N);

  fill_almost_sorted(buf, N);
  oracle_cell("almost_sorted", buf, N);

  /* modes: AGGRESSIVE and FORCE_HOLE must also be correct */
  fill_random(buf, N, 99);
  {
    int64_t *pho = alloc_n(N);
    int64_t *ref = alloc_n(N);
    memcpy(pho, buf, N * sizeof(int64_t));
    memcpy(ref, buf, N * sizeof(int64_t));
    photonic_sort_i64_ex(pho, N, PHOTONIC_MODE_AGGRESSIVE);
    qsort(ref, N, sizeof(int64_t), cmp_i64);
    g_cells++;
    if (memcmp(pho, ref, N * sizeof(int64_t)) != 0) {
      fprintf(stderr, "FAIL AGGRESSIVE mode content mismatch\n");
      g_fail++;
    } else {
      g_pass++;
      printf("OK   %-18s n=%zu\n", "mode_AGGRESSIVE", N);
    }
    free(pho); free(ref);
  }
  fill_random(buf, N, 123);
  {
    int64_t *pho = alloc_n(N);
    int64_t *ref = alloc_n(N);
    memcpy(pho, buf, N * sizeof(int64_t));
    memcpy(ref, buf, N * sizeof(int64_t));
    photonic_sort_i64_ex(pho, N, PHOTONIC_MODE_FORCE_HOLE);
    qsort(ref, N, sizeof(int64_t), cmp_i64);
    g_cells++;
    if (memcmp(pho, ref, N * sizeof(int64_t)) != 0) {
      fprintf(stderr, "FAIL FORCE_HOLE mode content mismatch\n");
      g_fail++;
    } else {
      g_pass++;
      printf("OK   %-18s n=%zu\n", "mode_FORCE_HOLE", N);
    }
    free(pho); free(ref);
  }

  oracle_generic_small();

  free(buf);

  printf("\ncells=%d pass=%d fail=%d\n", g_cells, g_pass, g_fail);
  if (g_fail == 0 && g_pass == g_cells) {
    printf("ALL_CORRECTNESS_OK=1\n");
    return 0;
  }
  printf("ALL_CORRECTNESS_OK=0\n");
  return 1;
}
