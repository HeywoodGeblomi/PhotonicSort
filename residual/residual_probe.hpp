#pragma once
// residual_probe.hpp — two-stage probe helpers for hybrid residual path
// Ticket B Dual Residual Deepening. EXTERNAL-clean.
// THE BEASTIE BOYZ 2026-08-23
//
// SOFT-SAFETY NOTE (Ticket B conservative land):
// sample_coarse + sample_dense are live infrastructure.
// Production decision path (is_border_he / is_strong_he / absolute inv-u thresholds
// against SAMPLE_SIZE=512, early exits that touch the charged Field-Level surface)
// continues to use the calibrated dense / sample_full surface.
// Coarse metrics are intentionally NOT yet wired into those predicates.
// Full two-stage activation requires a measurement pass after soft@1.20=0 is held.
// dual_evidence remains first and unchanged.

#include <cstddef>
#include <cstdint>
#include <algorithm>
#include <type_traits>
#include "residual_policy.hpp"

namespace residual_probe {

// Stage 1: cheap coarse sample (≤64–128 points)
template<typename T>
inline void sample_coarse(const T* a, size_t n,
                          size_t& inv, size_t& eq, size_t& u, size_t& desc_runs,
                          T& mn, T& mx) {
  inv = eq = desc_runs = 0;
  if (n < 2) { mn = mx = a[0]; u = 1; return; }
  const size_t stride = std::max<size_t>(1, n / residual_policy::COARSE_STRIDE_DIV);
  const size_t max_pts = 128;
  size_t count = 0;
  mn = mx = a[0];
  bool in_desc = false;
  T samp[128];
  for (size_t i = 0; i < n && count < max_pts; i += stride) {
    size_t j = (i + 1 < n) ? i + 1 : i;
    if (i > 0) {
      if (a[i] < a[i - stride]) {
        ++inv;
        if (!in_desc) { ++desc_runs; in_desc = true; }
      } else {
        in_desc = false;
      }
    }
    if (a[i] == a[j]) ++eq;
    if (a[i] < mn) mn = a[i];
    if (a[i] > mx) mx = a[i];
    samp[count++] = a[i];
  }
  if (count == 0) { u = 1; return; }
  std::sort(samp, samp + count);
  u = 1;
  for (size_t c = 1; c < count; ++c)
    if (samp[c] != samp[c - 1]) ++u;
}

// Stage 2: dense sample up to DENSE_CONFIRM_MAX (current sample_full behaviour)
template<typename T>
inline void sample_dense(const T* a, size_t n,
                         size_t& inv, size_t& eq, size_t& u, size_t& desc_runs,
                         T& mn, T& mx) {
  const size_t S = residual_policy::DENSE_CONFIRM_MAX;
  inv = eq = desc_runs = 0;
  T samp[residual_policy::DENSE_CONFIRM_MAX];
  mn = mx = a[0];
  bool in_desc = false;
  for (size_t c = 0; c < S; ++c) {
    size_t i = 1 + (c * (n - 1)) / S;
    size_t j = (i + 1 < n) ? i + 1 : i;
    if (a[i] < a[i - 1]) {
      ++inv;
      if (!in_desc) { ++desc_runs; in_desc = true; }
    } else {
      in_desc = false;
    }
    if (a[i] == a[j]) ++eq;
    if (a[i] < mn) mn = a[i];
    if (a[i] > mx) mx = a[i];
    samp[c] = a[i];
  }
  std::sort(samp, samp + S);
  u = 1;
  for (size_t c = 1; c < S; ++c)
    if (samp[c] != samp[c - 1]) ++u;
}

} // namespace residual_probe
