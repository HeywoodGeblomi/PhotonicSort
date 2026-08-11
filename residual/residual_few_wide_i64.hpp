#pragma once
/* FEW_WIDE v2.2: hashed collect + branchless k=2 partition. EXTERNAL-clean. */
#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <algorithm>
#include <cstddef>
namespace residual_few_wide {
static constexpr size_t KMAX = 16, HCAP = 64, BLOCK = 64;
inline uint32_t mix64(uint64_t x) {
  x ^= x >> 30; x *= 0xbf58476d1ce4e5b9ULL; x ^= x >> 27; x *= 0x94d049bb133111ebULL; x ^= x >> 31;
  return (uint32_t)x;
}
inline bool residual_two_value_branchless(int64_t *a, size_t n) {
  if (n < 2) return true;
  int64_t v0 = a[0], v1 = v0;
  for (size_t i = 1; i < n; ++i) if (a[i] != v0) { v1 = a[i]; break; }
  if (v0 == v1) return true;
  int64_t pivot = v0 < v1 ? v0 : v1;
  unsigned char offsets_l[BLOCK], offsets_r[BLOCK];
  size_t num_l = 0, num_r = 0;
  int64_t *first = a, *last = a + n;
  while (last - first > 2 * (std::ptrdiff_t)BLOCK) {
    if (num_l == 0) { num_l = 0; for (size_t i = 0; i < BLOCK; ++i) { offsets_l[num_l] = (unsigned char)i; num_l += (unsigned)(first[i] != pivot); } }
    if (num_r == 0) { num_r = 0; for (size_t i = 0; i < BLOCK; ++i) { offsets_r[num_r] = (unsigned char)i; num_r += (unsigned)(*(last - 1 - i) == pivot); } }
    size_t num = num_l < num_r ? num_l : num_r;
    for (size_t i = 0; i < num; ++i) std::swap(first[offsets_l[i]], *(last - 1 - offsets_r[i]));
    for (size_t i = 0; i < num_l - num; ++i) offsets_l[i] = offsets_l[i + num];
    for (size_t i = 0; i < num_r - num; ++i) offsets_r[i] = offsets_r[i + num];
    num_l -= num; num_r -= num;
    if (num_l == 0) first += BLOCK;
    if (num_r == 0) last -= BLOCK;
  }
  while (first < last) {
    while (first < last && *first == pivot) ++first;
    while (first < last && *(last - 1) != pivot) --last;
    if (first >= last) break;
    if (*first != v0 && *first != v1) return false;
    if (*(last - 1) != v0 && *(last - 1) != v1) return false;
    std::swap(*first, *(last - 1)); ++first; --last;
  }
  return true;
}
inline size_t collect_uniques(const int64_t *a, size_t n, int64_t *uniq_out) {
  int64_t slot[HCAP]; uint8_t used[HCAP] = {}; size_t k = 0;
  for (size_t i = 0; i < n; ++i) {
    int64_t v = a[i]; uint32_t h = mix64((uint64_t)v) & (HCAP - 1);
    for (;;) {
      if (!used[h]) { if (k >= KMAX) return 0; used[h] = 1; slot[h] = v; uniq_out[k++] = v; break; }
      if (slot[h] == v) break;
      h = (h + 1) & (HCAP - 1);
    }
  }
  return k;
}
inline void build_rank_map(const int64_t *uniq, size_t k, int64_t *slot, uint8_t *used, uint8_t *rank_out) {
  std::memset(used, 0, HCAP);
  for (size_t r = 0; r < k; ++r) {
    uint32_t h = mix64((uint64_t)uniq[r]) & (HCAP - 1);
    while (used[h]) h = (h + 1) & (HCAP - 1);
    used[h] = 1; slot[h] = uniq[r]; rank_out[h] = (uint8_t)r;
  }
}
inline uint8_t lookup_rank(int64_t v, const int64_t *slot, const uint8_t *used, const uint8_t *rank) {
  uint32_t h = mix64((uint64_t)v) & (HCAP - 1);
  while (used[h]) { if (slot[h] == v) return rank[h]; h = (h + 1) & (HCAP - 1); }
  return 0;
}
inline bool residual_few_wide_i64(int64_t *a, size_t n) {
  if (n < 2) return true;
  { size_t S = n < 64 ? n : 64, st = n / S; if (st < 1) st = 1;
    int64_t s0 = a[0], s1 = s0; bool two = false, more = false;
    for (size_t i = 0; i < n && !more; i += st) {
      int64_t v = a[i];
      if (v != s0) { if (!two) { s1 = v; two = true; } else if (v != s1) more = true; }
    }
    if (!more && two) { if (residual_two_value_branchless(a, n)) return true; }
    if (!more && !two) return true;
  }
  int64_t uniq[KMAX]; size_t k = collect_uniques(a, n, uniq);
  if (k == 0) return false; if (k == 1) return true;
  if (k == 2) return residual_two_value_branchless(a, n);
  for (size_t i = 1; i < k; ++i) {
    int64_t key = uniq[i]; size_t j = i;
    while (j > 0 && uniq[j - 1] > key) { uniq[j] = uniq[j - 1]; --j; }
    uniq[j] = key;
  }
  int64_t slot[HCAP]; uint8_t used[HCAP], rank[HCAP];
  build_rank_map(uniq, k, slot, used, rank);
  size_t cnt[KMAX] = {};
  for (size_t i = 0; i < n; ++i) ++cnt[lookup_rank(a[i], slot, used, rank)];
  size_t p = 0;
  for (size_t r = 0; r < k; ++r) { int64_t v = uniq[r]; for (size_t c = cnt[r]; c; --c) a[p++] = v; }
  return true;
}
inline bool should_try_few_wide(const int64_t *a, size_t n) {
  if (n < 2) return false;
  size_t S = n < 4096 ? n : 4096, st = n / S; if (st < 1) st = 1;
  int64_t sample_vals[4096]; size_t ns = 0; int64_t smin = a[0], smax = a[0];
  for (size_t i = 0; i < n && ns < S; i += st) {
    int64_t v = a[i]; sample_vals[ns++] = v;
    if (v < smin) smin = v; if (v > smax) smax = v;
  }
  uint64_t srange = (uint64_t)(smax - smin);
  bool wide = (srange >= (1ull << 20)) || (srange >= (uint64_t)(n * 3 / 4));
  if (!wide) return false;
  std::sort(sample_vals, sample_vals + ns);
  size_t sample_u = 1;
  for (size_t i = 1; i < ns; ++i) if (sample_vals[i] != sample_vals[i - 1]) ++sample_u;
  return sample_u <= KMAX + 2;
}
} // namespace residual_few_wide
