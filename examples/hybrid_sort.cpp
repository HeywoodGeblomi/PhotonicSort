/* Minimal stranger demo — DEFAULT dual residual path.
 * Build (after baselines present):
 *   cmake -B build && cmake --build build
 *   ./build/hybrid_sort_example
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
  std::uniform_int_distribution<int64_t> dist(0, 1'000'000'000LL);
  for (size_t i = 0; i < N; ++i) a[i] = dist(rng);

  auto t0 = std::chrono::steady_clock::now();
  hybrid_residual::sort_i64(a.data(), a.size());
  auto t1 = std::chrono::steady_clock::now();
  double ms = std::chrono::duration<double, std::milli>(t1 - t0).count();

  bool ok = std::is_sorted(a.begin(), a.end());
  std::printf("hybrid_residual::sort_i64 n=%zu  %.3f ms  ok=%d\n", N, ms, (int)ok);
  std::printf("DEFAULT = dual residual / residual talent (post-#167)\n");
  return ok ? 0 : 1;
}
