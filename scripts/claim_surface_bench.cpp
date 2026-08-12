// Claim-surface multi-baseline bench for CI (i32/u32/i64/f64)
// Outputs CSV for scripts/field_metrics.py. EXTERNAL-clean. Not field-level.
#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <random>
#include <vector>
#include "baselines/pdqsort.h"
#include "baselines/ska_sort.hpp"
#include "residual/pure_residual_menu.hpp"
#include "residual/pure_residual_menu_i32.hpp"
#include "residual/pure_residual_menu_u32.hpp"
#include "residual/pure_residual_menu_generic.hpp"

using Clock = std::chrono::steady_clock;
template<typename F> double med(F f, int R) {
  std::vector<double> t; t.reserve((size_t)R);
  for (int r = 0; r < R; ++r) {
    auto a = Clock::now(); f(); auto b = Clock::now();
    t.push_back(std::chrono::duration<double, std::milli>(b - a).count());
  }
  std::sort(t.begin(), t.end());
  return t[(size_t)R / 2];
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
template<typename T> std::vector<T> gen_db_pk(size_t n) {
  std::vector<T> v(n); std::mt19937_64 rng(42); T x = 0;
  for (size_t i = 0; i < n; ++i) { x = (T)(x + 1 + (rng() % 3)); v[i] = x; }
  for (size_t s = 0; s < n / 1000; ++s) { size_t i = rng() % n, j = rng() % n; std::swap(v[i], v[j]); }
  return v;
}
template<typename T> std::vector<T> gen_ts(size_t n) {
  std::vector<T> v(n); std::mt19937_64 rng(42); T t = (T)1700000000;
  for (size_t i = 0; i < n; ++i) { t = (T)(t + (rng() % 5)); v[i] = t; }
  return v;
}
template<typename T> std::vector<T> gen_few(size_t n, int k) {
  std::vector<T> v(n); std::mt19937_64 rng(42);
  for (size_t i = 0; i < n; ++i) v[i] = (T)(rng() % (size_t)k);
  return v;
}
template<typename T> std::vector<T> gen_eq(size_t n) {
  std::vector<T> v(n); size_t bs = n / 8;
  for (int b = 0; b < 8; ++b)
    for (size_t i = (size_t)b * bs; i < ((b + 1 == 8) ? n : (size_t)(b + 1) * bs); ++i)
      v[i] = (T)b;
  std::mt19937_64 rng(42);
  for (size_t s = 0; s < n / 100; ++s) { size_t i = rng() % n, j = rng() % n; std::swap(v[i], v[j]); }
  return v;
}
template<typename T> std::vector<T> gen_saw(size_t n) {
  std::vector<T> v(n); for (size_t i = 0; i < n; ++i) v[i] = (T)(i % 1024); return v;
}
template<typename T> std::vector<T> gen_adv(size_t n) {
  std::vector<T> v(n); for (size_t i = 0; i < n; ++i) v[i] = (T)((i * 2654435761ull) >> 32); return v;
}
template<typename T> std::vector<T> gen_push(size_t n) {
  std::vector<T> v(n); size_t a = n / 4, b = 3 * n / 4;
  for (size_t i = 0; i < a; ++i) v[i] = (T)(i + n / 2);
  for (size_t i = a; i < b; ++i) v[i] = (T)(i - a);
  for (size_t i = b; i < n; ++i) v[i] = (T)i;
  return v;
}
template<typename T> std::vector<T> gen_rseg(size_t n) {
  auto v = gen_sorted<T>(n); size_t seg = n / 8;
  for (int s = 0; s < 8; ++s) {
    size_t lo = (size_t)s * seg, hi = (s + 1 == 8) ? n : lo + seg;
    std::reverse(v.begin() + (ptrdiff_t)lo, v.begin() + (ptrdiff_t)hi);
  }
  return v;
}
template<typename T> std::vector<T> gen_zipf(size_t n) {
  std::vector<T> v(n); std::mt19937_64 rng(42);
  std::vector<double> w(16); double sum = 0;
  for (int i = 0; i < 16; ++i) { w[(size_t)i] = 1.0 / (i + 1); sum += w[(size_t)i]; }
  for (int i = 0; i < 16; ++i) w[(size_t)i] /= sum;
  for (size_t i = 0; i < n; ++i) {
    double u = std::generate_canonical<double, 53>(rng), c = 0; int ch = 15;
    for (int j = 0; j < 16; ++j) { c += w[(size_t)j]; if (u <= c) { ch = j; break; } }
    v[i] = (T)ch;
  }
  return v;
}

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
  double s = -1.0;
  if (use_ska) s = med([&] { auto a = base; ska_sort(a.begin(), a.end()); }, reps);
  double t = med([&] { auto a = base; std::sort(a.begin(), a.end()); }, reps);
  double best = p;
  if (t < best) best = t;
  if (use_ska && s > 0 && s < best) best = s;
  auto chk = base; menu(chk.data(), n);
  int ok = std::is_sorted(chk.begin(), chk.end()) ? 1 : 0;
  double ratio = m / best;
  fprintf(csv, "%s,%s,%s,%zu,%d,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%d\n",
          arch_name(), type, pat, n, reps, m, p, s, t, best, ratio, ok);
  printf("%s %-4s %-18s ratio=%.3f ok=%d%s\n", arch_name(), type, pat, ratio, ok, ratio > 1.15 ? " SOFT" : "");
}

template<typename T, typename MenuFn>
void suite_int(FILE *csv, const char *type, size_t n, int reps, MenuFn menu, bool use_ska) {
  row(csv, type, "sorted", gen_sorted<T>(n), menu, reps, use_ska);
  row(csv, type, "reverse", gen_reverse<T>(n), menu, reps, use_ska);
  row(csv, type, "almost_sorted", gen_almost<T>(n), menu, reps, use_ska);
  row(csv, type, "sawtooth", gen_saw<T>(n), menu, reps, use_ska);
  row(csv, type, "reverse_segments", gen_rseg<T>(n), menu, reps, use_ska);
  row(csv, type, "push_middle", gen_push<T>(n), menu, reps, use_ska);
  row(csv, type, "few_k4_dense", gen_few<T>(n, 4), menu, reps, use_ska);
  row(csv, type, "few_k16_dense", gen_few<T>(n, 16), menu, reps, use_ska);
  row(csv, type, "equal_heavy", gen_eq<T>(n), menu, reps, use_ska);
  row(csv, type, "zipf_k16", gen_zipf<T>(n), menu, reps, use_ska);
  row(csv, type, "db_pk", gen_db_pk<T>(n), menu, reps, use_ska);
  row(csv, type, "timestamps", gen_ts<T>(n), menu, reps, use_ska);
  row(csv, type, "adversarial", gen_adv<T>(n), menu, reps, use_ska);
}

void suite_f64(FILE *csv, size_t n, int reps) {
  auto menu = [](double *a, size_t n) { pure_residual::sort_f64(a, n); };
  row(csv, "f64", "sorted", gen_sorted<double>(n), menu, reps, false);
  row(csv, "f64", "reverse", gen_reverse<double>(n), menu, reps, false);
  row(csv, "f64", "almost_sorted", gen_almost<double>(n), menu, reps, false);
  row(csv, "f64", "sawtooth", gen_saw<double>(n), menu, reps, false);
  row(csv, "f64", "reverse_segments", gen_rseg<double>(n), menu, reps, false);
  row(csv, "f64", "push_middle", gen_push<double>(n), menu, reps, false);
  row(csv, "f64", "few_k4_dense", gen_few<double>(n, 4), menu, reps, false);
  row(csv, "f64", "few_k16_dense", gen_few<double>(n, 16), menu, reps, false);
  row(csv, "f64", "equal_heavy", gen_eq<double>(n), menu, reps, false);
  row(csv, "f64", "zipf_k16", gen_zipf<double>(n), menu, reps, false);
  row(csv, "f64", "db_pk", gen_db_pk<double>(n), menu, reps, false);
  row(csv, "f64", "timestamps", gen_ts<double>(n), menu, reps, false);
  row(csv, "f64", "adversarial", gen_adv<double>(n), menu, reps, false);
}

int main(int argc, char **argv) {
  size_t n = 1000000;
  int reps = 5;
  const char *out = "claim_surface.csv";
  const char *only = "all";
  for (int i = 1; i < argc; ++i) {
    if (!strcmp(argv[i], "--n") && i + 1 < argc) n = (size_t)strtoull(argv[++i], nullptr, 10);
    else if (!strcmp(argv[i], "--reps") && i + 1 < argc) reps = atoi(argv[++i]);
    else if (!strcmp(argv[i], "--out") && i + 1 < argc) out = argv[++i];
    else if (!strcmp(argv[i], "--only") && i + 1 < argc) only = argv[++i];
  }
  FILE *csv = fopen(out, "w");
  fprintf(csv, "arch,type,pattern,n,reps,menu_ms,pdq_ms,ska_ms,std_ms,best_ms,ratio_best,ok\n");
  printf("# claim_surface_bench arch=%s n=%zu reps=%d\n", arch_name(), n, reps);
  if (!strcmp(only, "all") || !strcmp(only, "i64"))
    suite_int<int64_t>(csv, "i64", n, reps, [](int64_t *a, size_t n) { pure_residual::sort_i64(a, n); }, true);
  if (!strcmp(only, "all") || !strcmp(only, "i32"))
    suite_int<int32_t>(csv, "i32", n, reps, [](int32_t *a, size_t n) { pure_residual::sort_i32(a, n); }, true);
  if (!strcmp(only, "all") || !strcmp(only, "u32"))
    suite_int<uint32_t>(csv, "u32", n, reps, [](uint32_t *a, size_t n) { pure_residual::sort_u32(a, n); }, true);
  if (!strcmp(only, "all") || !strcmp(only, "f64"))
    suite_f64(csv, n, reps);
  fclose(csv);
  printf("# wrote %s\n", out);
  return 0;
}
