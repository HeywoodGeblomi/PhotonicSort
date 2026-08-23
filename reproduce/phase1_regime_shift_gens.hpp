#pragma once
/*
 * phase1_regime_shift_gens.hpp — PHO-EXT-001 Phase 1
 * Pure dual-evidence regime-shift generators for charged-surface expansion.
 *
 * Late Phase-Shift Mixed Blocks: classical high, σ_Δ near floor → dual_owned=false
 * Near-Tie HE Odd-Event Starvation: same contested residual_talent band
 *
 * Include ONLY from charged-surface / SP multi-arch harnesses.
 * NEVER include from hybrid_residual_menu.hpp or any production residual unit.
 *
 * EXTERNAL-clean. No ChiState. No OwnershipHarness. THE BEASTIE BOYZ 2026-08-23
 */
#include <algorithm>
#include <cstdint>
#include <random>
#include <vector>
#include <cmath>
#include <limits>
#include <type_traits>

namespace photonic {
namespace phase1 {

template<typename T>
inline std::vector<T> gen_late_phase_shift_mixed_blocks(size_t n,
    size_t k_blocks = 8, double shift_loc = 0.65) {
  std::vector<T> v(n);
  if (n == 0) return v;
  size_t len = std::max<size_t>(1, n / k_blocks);
  for (size_t s = 0; s < k_blocks; ++s) {
    size_t lo = s * len;
    size_t hi = (s + 1 == k_blocks) ? n : std::min(n, (s + 1) * len);
    bool ascending = (s % 2 == 0);
    if ((double)lo / (double)n >= shift_loc) ascending = !ascending;
    if (ascending) {
      for (size_t i = lo; i < hi; ++i) v[i] = (T)(i - lo);
    } else {
      for (size_t i = lo; i < hi; ++i) v[i] = (T)(hi - 1 - i);
    }
  }
  size_t shift_idx = (size_t)(shift_loc * n);
  for (size_t i = 0; i < 12 && shift_idx + i * 3 + 1 < n; ++i) {
    size_t j = shift_idx + i * 3;
    std::swap(v[j], v[j + 1]);
  }
  return v;
}

template<typename T>
inline std::vector<T> gen_near_tie_he_odd_starvation(size_t n,
    double event_density = 0.006, uint32_t seed = 42) {
  std::vector<T> v(n);
  std::mt19937_64 rng(seed);
  if constexpr (std::is_signed_v<T>) {
    std::uniform_int_distribution<T> dist(
        std::numeric_limits<T>::min() / 4,
        std::numeric_limits<T>::max() / 4);
    for (size_t i = 0; i < n; ++i) v[i] = dist(rng);
  } else {
    std::uniform_int_distribution<T> dist(0, std::numeric_limits<T>::max() / 4);
    for (size_t i = 0; i < n; ++i) v[i] = dist(rng);
  }
  size_t step = std::max<size_t>(8, (size_t)(1.0 / event_density));
  for (size_t i = step; i + 1 < n; i += step) {
    if (v[i] >= v[i - 1]) v[i] = (T)(v[i - 1] - (T)1);
    else                  v[i] = (T)(v[i - 1] + (T)1);
    for (size_t k = 1; k < step / 4 && i + k < n; ++k)
      v[i + k] = (T)(v[i] + (T)k);
  }
  return v;
}

} // namespace phase1
} // namespace photonic
