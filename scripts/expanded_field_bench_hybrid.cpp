// Expanded Field Suite — hybrid residual path-(a) harness
// EXTERNAL-clean. THE BEASTIE BOYZ 2026-08-12
#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <random>
#include <string>
#include <vector>

#include "baselines/pdqsort.h"
#include "baselines/ska_sort.hpp"
#include "residual/pure_residual_menu.hpp"
#include "residual/pure_residual_menu_i32.hpp"
#include "residual/pure_residual_menu_u32.hpp"
#include "residual/hybrid_residual_menu.hpp"

static size_t n = 1000000;
static int reps = 5;

template<typename F>
double med(F f, int r) {
  std::vector<double> t;
  f();
  for (int i = 0; i < r; ++i) {
    auto a = std::chrono::steady_clock::now();
    f();
    auto b = std::chrono::steady_clock::now();
    t.push_back(std::chrono::duration<double, std::milli>(b - a).count());
  }
  std::sort(t.begin(), t.end());
  return t[t.size() / 2];
}

template<typename T> std::vector<T> gen_sorted(size_t n) {
  std::vector<T> v(n); for (size_t i = 0; i < n; ++i) v[i] = (T)i; return v;
}
template<typename T> std::vector<T> gen_reverse(size_t n) {
  std::vector<T> v(n); for (size_t i = 0; i < n; ++i) v[i] = (T)(n - 1 - i); return v;
}
template<typename T> std::vector<T> gen_almost(size_t n) {
  auto v = gen_sorted<T>(n); std::mt19937_64 rng(42);
  for (size_t s = 0; s < n / 1000; ++s) { size_t i = rng() % n, j = rng() % n; std::swap(v[i], v[j]); }
  return v;
}
template<typename T> std::vector<T> gen_organpipe(size_t n) {
  std::vector<T> v(n);
  for (size_t i = 0; i < n / 2; ++i) v[i] = (T)i;
  for (size_t i = n / 2; i < n; ++i) v[i] = (T)(n - 1 - i);
  return v;
}
template<typename T> std::vector<T> gen_saw(size_t n) {
  std::vector<T> v(n); for (size_t i = 0; i < n; ++i) v[i] = (T)(i % 64); return v;
}
template<typename T> std::vector<T> gen_rseg(size_t n) {
  std::vector<T> v(n); const size_t bs = 64;
  for (size_t b = 0; b < n; b += bs) {
    size_t e = std::min(b + bs, n);
    for (size_t i = b; i < e; ++i) v[i] = (T)(e - 1 - (i - b) + b);
  }
  return v;
}
template<typename T> std::vector<T> gen_push(size_t n) {
  std::vector<T> v(n); size_t mid = n / 10;
  for (size_t i = 0; i < mid; ++i) v[i] = (T)(i + n / 2);
  for (size_t i = mid; i < n; ++i) v[i] = (T)(i - mid);
  return v;
}
template<typename T> std::vector<T> gen_runs_noise(size_t n) {
  std::vector<T> v(n); std::mt19937_64 rng(42); T x = 0;
  for (size_t i = 0; i < n; ++i) {
    if (i % 128 == 0) x = (T)(rng() % 1000);
    else x = (T)(x + 1);
    v[i] = x;
  }
  for (size_t s = 0; s < n / 500; ++s) { size_t i = rng() % n, j = rng() % n; std::swap(v[i], v[j]); }
  return v;
}
template<typename T> std::vector<T> gen_few(size_t n, int k) {
  std::vector<T> v(n); std::mt19937_64 rng(42);
  for (size_t i = 0; i < n; ++i) v[i] = (T)(rng() % (uint64_t)k);
  return v;
}
template<typename T> std::vector<T> gen_few_wide(size_t n, int k) {
  std::vector<T> v(n); std::mt19937_64 rng(42);
  for (size_t i = 0; i < n; ++i) v[i] = (T)((rng() % (uint64_t)k) * 1000003ull);
  return v;
}
template<typename T> std::vector<T> gen_eq(size_t n) {
  std::vector<T> v(n); std::mt19937_64 rng(42);
  for (size_t i = 0; i < n; ++i) v[i] = (T)((rng() % 100 < 90) ? 0 : (rng() % 16));
  return v;
}
template<typename T> std::vector<T> gen_zipf(size_t n) {
  std::vector<T> v(n); std::mt19937_64 rng(42);
  for (size_t i = 0; i < n; ++i) {
    double u = std::generate_canonical<double, 53>(rng);
    v[i] = (T)std::min(15, (int)(std::pow(u, -0.5) - 1));
  }
  return v;
}
template<typename T> std::vector<T> gen_db_pk(size_t n) {
  std::vector<T> v(n); std::mt19937_64 rng(42); T x = 0;
  for (size_t i = 0; i < n; ++i) { x = (T)(x + 1 + (T)(rng() % 3)); v[i] = x; }
  for (size_t s = 0; s < n / 1000; ++s) { size_t i = rng() % n, j = rng() % n; std::swap(v[i], v[j]); }
  return v;
}
template<typename T> std::vector<T> gen_ts(size_t n) {
  std::vector<T> v(n); std::mt19937_64 rng(42); T t = (T)1700000000;
  for (size_t i = 0; i < n; ++i) { t = (T)(t + (T)(rng() % 5)); v[i] = t; }
  return v;
}
template<typename T> std::vector<T> gen_pipe_sparse(size_t n) {
  std::vector<T> v(n);
  for (size_t i = 0; i < n / 2; ++i) v[i] = (T)(i * 17);
  for (size_t i = n / 2; i < n; ++i) v[i] = (T)((n - 1 - i) * 17);
  std::mt19937_64 rng(42);
  for (size_t s = 0; s < n / 500; ++s) { size_t i = rng() % n, j = rng() % n; std::swap(v[i], v[j]); }
  return v;
}
template<typename T> std::vector<T> gen_adv(size_t n) {
  std::vector<T> v(n); for (size_t i = 0; i < n; ++i) v[i] = (T)((i * 2654435761ull) >> 32); return v;
}
template<typename T> std::vector<T> gen_random(size_t n) {
  std::vector<T> v(n); std::mt19937_64 rng(42);
  for (size_t i = 0; i < n; ++i) v[i] = (T)rng(); return v;
}
template<typename T> std::vector<T> gen_gaussianish(size_t n) {
  std::vector<T> v(n); std::mt19937_64 rng(42);
  std::normal_distribution<double> dist(0.0, (double)n / 8.0);
  for (size_t i = 0; i < n; ++i) v[i] = (T)(int64_t)std::llround(dist(rng));
  return v;
}
template<typename T> std::vector<T> gen_db_fk_zipf(size_t n) {
  size_t k = (size_t)std::sqrt((double)n); if (k < 16) k = 16; if (k > 65536) k = 65536;
  std::vector<T> v(n); std::mt19937_64 rng(42);
  std::vector<double> w(k); double sum = 0;
  for (size_t i = 0; i < k; ++i) { w[i] = 1.0 / (double)(i + 1); sum += w[i]; }
  for (size_t i = 0; i < k; ++i) w[i] /= sum;
  for (size_t i = 0; i < n; ++i) {
    double u = std::generate_canonical<double, 53>(rng), c = 0; size_t ch = k - 1;
    for (size_t j = 0; j < k; ++j) { c += w[j]; if (u <= c) { ch = j; break; } }
    v[i] = (T)ch;
  }
  return v;
}
template<typename T> std::vector<T> gen_timestamp_drift(size_t n) {
  std::vector<T> v(n); std::mt19937_64 rng(42); T t = (T)1700000000;
  for (size_t i = 0; i < n; ++i) {
    t = (T)(t + (T)(1 + rng() % 3));
    if (rng() % 100 == 0) t = (T)(t - (T)(rng() % 50));
    v[i] = t;
  }
  return v;
}
template<typename T> std::vector<T> gen_mixed_blocks(size_t n) {
  std::vector<T> v(n); for (size_t i = 0; i < n; ++i) v[i] = (T)i;
  std::mt19937_64 rng(42); const size_t bs = 256;
  for (size_t base = 0; base < n; base += bs * 2) {
    size_t lo = base + bs; if (lo >= n) break;
    size_t hi = std::min(lo + bs, n);
    std::shuffle(v.begin() + (ptrdiff_t)lo, v.begin() + (ptrdiff_t)hi, rng);
  }
  return v;
}
template<typename T> std::vector<T> gen_uniform_u32_style(size_t n) { return gen_random<T>(n); }

const char *arch_name() {
#if defined(__aarch64__) || defined(_M_ARM64)
  return "aarch64";
#elif defined(__x86_64__) || defined(_M_X64)
  return "x86_64";
#else
  return "unknown";
#endif
}

template<typename T, typename MenuFn>
void row(FILE *csv, const char *type, const char *pat, std::vector<T> base, MenuFn menu, int reps, bool use_ska) {
  size_t n = base.size();
  double m = med([&] { auto a = base; menu(a.data(), n); }, reps);
  double p = med([&] { auto a = base; pdqsort(a.begin(), a.end()); }, reps);
  double s = 0;
  if (use_ska) s = med([&] { auto a = base; ska_sort(a.begin(), a.end()); }, reps);
  double t = med([&] { auto a = base; std::sort(a.begin(), a.end()); }, reps);
  double best = p;
  if (t < best) best = t;
  if (use_ska && s > 0 && s < best) best = s;
  auto a = base; menu(a.data(), n);
  int ok = 1; for (size_t i = 1; i < n; ++i) if (a[i] < a[i - 1]) { ok = 0; break; }
  double ratio = m / best;
  fprintf(csv, "%s,%s,%s,%zu,%d,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%d\n",
          arch_name(), type, pat, n, reps, m, p, s, t, best, ratio, ok);
  printf("%s %-4s %-18s ratio=%.3f ok=%d%s\n", arch_name(), type, pat, ratio, ok,
         ratio > 1.15 ? " SOFT" : "");
}

template<typename T, typename MenuFn>
void suite_int(FILE *csv, const char *type, size_t n, int reps, MenuFn menu, bool use_ska) {
  row(csv, type, "sorted", gen_sorted<T>(n), menu, reps, use_ska);
  row(csv, type, "reverse", gen_reverse<T>(n), menu, reps, use_ska);
  row(csv, type, "almost_sorted", gen_almost<T>(n), menu, reps, use_ska);
  row(csv, type, "organpipe", gen_organpipe<T>(n), menu, reps, use_ska);
  row(csv, type, "sawtooth", gen_saw<T>(n), menu, reps, use_ska);
  row(csv, type, "reverse_segments", gen_rseg<T>(n), menu, reps, use_ska);
  row(csv, type, "push_middle", gen_push<T>(n), menu, reps, use_ska);
  row(csv, type, "runs_noise", gen_runs_noise<T>(n), menu, reps, use_ska);
  row(csv, type, "few_k4_dense", gen_few<T>(n, 4), menu, reps, use_ska);
  row(csv, type, "few_k16_dense", gen_few<T>(n, 16), menu, reps, use_ska);
  row(csv, type, "few_k16_wide", gen_few_wide<T>(n, 16), menu, reps, use_ska);
  row(csv, type, "equal_heavy", gen_eq<T>(n), menu, reps, use_ska);
  row(csv, type, "zipf_k16", gen_zipf<T>(n), menu, reps, use_ska);
  row(csv, type, "db_pk", gen_db_pk<T>(n), menu, reps, use_ska);
  row(csv, type, "timestamps", gen_ts<T>(n), menu, reps, use_ska);
  row(csv, type, "pipe_sparse", gen_pipe_sparse<T>(n), menu, reps, use_ska);
  row(csv, type, "adversarial", gen_adv<T>(n), menu, reps, use_ska);
  row(csv, type, "random", gen_random<T>(n), menu, reps, use_ska);
  row(csv, type, "gaussianish", gen_gaussianish<T>(n), menu, reps, use_ska);
  row(csv, type, "db_fk_zipf", gen_db_fk_zipf<T>(n), menu, reps, use_ska);
  row(csv, type, "timestamp_drift", gen_timestamp_drift<T>(n), menu, reps, use_ska);
  row(csv, type, "mixed_blocks", gen_mixed_blocks<T>(n), menu, reps, use_ska);
  row(csv, type, "uniform_u32", gen_uniform_u32_style<T>(n), menu, reps, use_ska);
}

int main(int argc, char **argv) {
  const char *out = "hybrid_expanded.csv";
  const char *only = nullptr;
  for (int i = 1; i < argc; ++i) {
    if (!strcmp(argv[i], "--n") && i + 1 < argc) n = (size_t)std::stoull(argv[++i]);
    else if (!strcmp(argv[i], "--reps") && i + 1 < argc) reps = atoi(argv[++i]);
    else if (!strcmp(argv[i], "--out") && i + 1 < argc) out = argv[++i];
    else if (!strcmp(argv[i], "--only") && i + 1 < argc) only = argv[++i];
  }
  FILE *csv = fopen(out, "w");
  if (!csv) return 1;
  fprintf(csv, "arch,type,pattern,n,reps,menu_ms,pdq_ms,ska_ms,std_ms,best_ms,ratio_best,ok\n");
  printf("# expanded_field_bench hybrid arch=%s n=%zu reps=%d patterns=23\n", arch_name(), n, reps);
  if (!only || !strcmp(only, "i64"))
    suite_int<int64_t>(csv, "i64", n, reps, [](int64_t *a, size_t nn) { hybrid_residual::sort_i64(a, nn); }, true);
  if (!only || !strcmp(only, "i32"))
    suite_int<int32_t>(csv, "i32", n, reps, [](int32_t *a, size_t nn) { hybrid_residual::sort_i32(a, nn); }, true);
  if (!only || !strcmp(only, "u32"))
    suite_int<uint32_t>(csv, "u32", n, reps, [](uint32_t *a, size_t nn) { hybrid_residual::sort_u32(a, nn); }, true);
  fprintf(stderr, "# wrote %s\n", out);
  fclose(csv);
  return 0;
}
