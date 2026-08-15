/* 10-line stranger demo — DEFAULT dual residual / residual talent.
 * Build (after baselines present):
 *   cmake -B build && cmake --build build && ./build/hybrid_sort_example
 * EXTERNAL-clean. THE BEASTIE BOYZ
 */
#include <cstdint>
#include <cstdio>
#include <vector>
#include <random>
#include <algorithm>
#include <chrono>
#include "hybrid_residual_menu.hpp"

int main() {
  constexpr size_t N = 1'000'000;
  std::vector<int64_t> a(N);
  std::mt19937_64 rng(42);
  for (size_t i = 0; i < N; ++i) a[i] = rng() % 1'000'000'000LL;

  auto t0 = std::chrono::steady_clock::now();
  hybrid_residual::sort_i64(a.data(), a.size());  // DEFAULT = dual residual talent
  auto ms = std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - t0).count();

  bool ok = std::is_sorted(a.begin(), a.end());
  std::printf("hybrid_residual::sort_i64 n=%zu  %.3f ms  ok=%d  (talent path default)\n", N, ms, (int)ok);
  return ok ? 0 : 1;
}
