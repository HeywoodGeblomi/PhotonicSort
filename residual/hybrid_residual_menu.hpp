#pragma once
/* hybrid_residual_menu v28+ — DEFAULT dual residual production path.
 * Residual talent drives borderline HE by default.
 * Escape hatch: -DCLASSICAL_RESIDUAL restores unconditional ska on mid-band HE.
 * Track 3 thresholds via residual_policy. EXTERNAL-clean. THE BEASTIE BOYZ
 * Dual Residual Deepening 2026-08-23 — Phase 2 prefer + Ticket B probe helpers
 * + Measurement Option 3 (coarse only for conservative early exits).
 *
 * Measurement Option 3 (locked):
 * sample_coarse is used ONLY for conservative early exits that never touch the
 * charged Field-Level band. is_border_he / is_strong_he / absolute thresholds
 * remain fed exclusively by calibrated sample_full (SAMPLE_SIZE=512).
 * dual_evidence + Phase 1 extraction + Phase 2 pure specialist prefer unchanged.
 */
#include <cstdint>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <cmath>
#include <type_traits>
#include "pure_residual_menu.hpp"
#include "pure_residual_menu_i32.hpp"
#include "pure_residual_menu_u32.hpp"
#include "pdqsort_residual.h"
#include "ska_sort.hpp"
#include "residual_reverse_segments.hpp"
#include "residual_mixed_blocks.hpp"
#include "pdqsort.h"
#include "secondary_parity.hpp"
#include "residual_policy.hpp"
#include "residual_probe.hpp"
#include "residual_few_wide_i64.hpp"
#include "residual_low_disorder_i64.hpp"
#include "residual_sparse_i64.hpp"
#include "residual_adversarial_i64.hpp"
#include "residual_he_msd_i64.hpp"
#include "residual_push_middle_i32.hpp"

namespace hybrid_residual {

template<typename T> inline bool is_sorted_asc(const T *a, size_t n) {
  for (size_t i=1;i<n;++i) if (a[i]<a[i-1]) return false; return true;}
template<typename T> inline bool is_sorted_desc(const T *a, size_t n) {
  for (size_t i=1;i<n;++i) if (a[i]>a[i-1]) return false; return true;}

template<typename T> inline void sample_full(const T *a, size_t n, size_t &inv, size_t &eq, size_t &u, size_t &desc_runs, T &mn, T &mx) {
  const size_t S = residual_policy::SAMPLE_SIZE;
  inv=eq=desc_runs=0; T samp[residual_policy::SAMPLE_SIZE]; mn=mx=a[0]; bool in_desc=false;
  for (size_t c=0;c<S;++c){ size_t i=1+(c*(n-1))/S; size_t j=(i+1<n)?i+1:i;
    if(a[i]<a[i-1]){++inv; if(!in_desc){++desc_runs;in_desc=true;}} else in_desc=false;
    if(a[i]==a[j])++eq; if(a[i]<mn)mn=a[i]; if(a[i]>mx)mx=a[i]; samp[c]=a[i];}
  std::sort(samp,samp+S); u=1; for(size_t c=1;c<S;++c) if(samp[c]!=samp[c-1])++u;}

template<typename T> inline size_t dense_inv(const T *a, size_t n){
  size_t inv=0, step=n>8192?n/8192:1; for(size_t i=0;i+1<n;i+=step) if(a[i]>a[i+1])++inv; return inv;}
inline size_t dense_inv_samples(size_t n){ size_t step=n>8192?n/8192:1; return n>0?(n-1)/step:0;}

template<typename T> inline uint64_t domain_of(T mn, T mx){
  if constexpr (std::is_unsigned<T>::value) return (uint64_t)mx-(uint64_t)mn;
  else return (uint64_t)((int64_t)mx-(int64_t)mn);}

template<typename T> inline bool try_count_sort(T *a, size_t n, T smn, T smx){
  if(n<2) return true; T amin=smn,amax=smx;
  for(size_t i=0;i<n;++i){ if(a[i]<amin)amin=a[i]; if(a[i]>amax)amax=a[i];}
  if(amin==amax) return true; uint64_t range=domain_of(amin,amax);
  if(range>= residual_policy::COUNT_RANGE_MAX || range+1>=(uint64_t)n) return false;
  size_t *cnt=(size_t*)std::calloc((size_t)range+1,sizeof(size_t)); if(!cnt) return false;
  for(size_t i=0;i<n;++i){ uint64_t idx; if constexpr(std::is_unsigned<T>::value) idx=(uint64_t)a[i]-(uint64_t)amin;
    else idx=(uint64_t)((int64_t)a[i]-(int64_t)amin); cnt[idx]++;}
  size_t p=0; for(uint64_t v=0;v<=range;++v) for(size_t c=cnt[v];c;--c) a[p++]=(T)((int64_t)v+(int64_t)amin);
  std::free(cnt); return true;}

template<typename T, typename PureFn> inline int dispatch(T *a, size_t n, PureFn pure_fn){
  if(n<2) return 0;
  if(is_sorted_asc(a,n)) return 0;
  if(is_sorted_desc(a,n)){ std::reverse(a,a+n); return 0;}
  if(residual_reverse_segments::try_reverse_segments(a,n)) return 0;
  if(residual_mixed_blocks::try_mixed_blocks(a,n)) return 0;

  // Measurement Option 3: coarse only for conservative early exits that never
  // touch the charged band. If coarse shows zero inversions, the array is
  // extremely structured; fall through to pure residual (already a safe path).
  // is_border_he / is_strong_he remain fed exclusively by calibrated sample_full.
  {
    size_t c_inv=0, c_eq=0, c_u=0, c_desc=0; T c_mn, c_mx;
    residual_probe::sample_coarse(a, n, c_inv, c_eq, c_u, c_desc, c_mn, c_mx);
    if (c_inv == 0 && c_u <= 4) {
      // Extreme structure on coarse sample → pure residual (never HE band)
      return pure_fn(a, n);
    }
  }

  // Production charged path: calibrated dense sample (SAMPLE_SIZE=512)
  size_t inv,eq,u,desc_runs; T mn,mx;
  sample_full(a,n,inv,eq,u,desc_runs,mn,mx);
  const size_t S = residual_policy::SAMPLE_SIZE;
  uint64_t dom = domain_of(mn,mx);

  // Dual evidence (unchanged)
  auto de = secondary_parity::dual_evidence(a, n);
  const bool dual_owned          = de.confirmed;
  const residual_policy::ResidualTalent residual_talent = de.suggested;

  if(eq*4 >= S*3){ pdqsort(a,a+n); return 0;}
  if(dom <= residual_policy::COUNT_DOMAIN_MAX){ if(try_count_sort(a,n,mn,mx)) return 0;}
  if(u >= (S*50)/100 && inv*10 <= S){ pdqsort(a,a+n); return 0;}
  if(u <= 32){ if(dom <= residual_policy::COUNT_DOMAIN_MAX && try_count_sort(a,n,mn,mx)) return 0; return pure_fn(a,n);}
  if(u <= 128 && inv*2 >= S && inv*2 <= S) return pure_fn(a,n);
  if(desc_runs >= 3 && inv*5 >= S*3) return pure_fn(a,n);
  if(inv * residual_policy::LOW_INV_MULT <= S){
    if(dom <= (uint64_t)n*2ull){ size_t dinv=dense_inv(a,n); size_t ds=dense_inv_samples(n);
      if(ds==0 || dinv*50 <= ds) return pure_fn(a,n); pdqsort(a,a+n); return 0;}
    pdqsort(a,a+n); return 0;}

  // Border HE band (exact policy predicate — still calibrated to S=512)
  if (residual_policy::is_border_he(u, inv, S)) {
#ifdef CLASSICAL_RESIDUAL
    ska_sort(a,a+n); return 0;
#else
    if (residual_policy::is_strong_he(u, inv, S)) {
      ska_sort(a,a+n); return 0;
    }
    // Phase 2 pure specialist prefer (untouched)
    if (dual_owned) {
      switch (residual_talent) {
        case residual_policy::ResidualTalent::T1:
          if constexpr (std::is_same_v<T, int64_t>) {
            if (residual_few_wide::should_try_few_wide(a, n) &&
                residual_few_wide::residual_few_wide_i64(a, n)) return 0;
            if (residual_low_disorder::should_try_low_disorder(a, n) &&
                residual_low_disorder::residual_low_disorder_i64(a, n)) return 0;
            if (residual_sparse::residual_sparse_i64(a, n) == 0) return 0;
          }
          break;
        case residual_policy::ResidualTalent::T2:
          if constexpr (std::is_same_v<T, int32_t>) {
            if (residual_push_middle_i32::try_push_middle(a, n)) return 0;
          }
          break;
        case residual_policy::ResidualTalent::T3:
          if constexpr (std::is_same_v<T, int64_t>) {
            if (residual_adversarial::residual_adversarial_i64(a, n) == 0) return 0;
            if (residual_he::residual_he_msd_i64(a, n) == 0) return 0;
          }
          break;
        default: break;
      }
    }
    if (dual_owned && (residual_talent == residual_policy::ResidualTalent::T3 ||
                       residual_talent == residual_policy::ResidualTalent::T1)) {
      ska_sort(a, a + n); return 0;
    }
    pdqsort(a, a + n); return 0;
#endif
  }

  { size_t dinv = dense_inv(a,n);
    if (dom <= (uint64_t)n*2ull && dinv >= 100) {
      if (residual_policy::is_strong_he(u, inv, S) && residual_policy::is_border_he(u, inv, S))
        ska_sort(a,a+n);
      else
        pdqsort(a,a+n);
      return 0;
    }
  }
  pdqsort(a,a+n); return 0;
}

inline int sort_i64(int64_t *a, size_t n){
  return dispatch(a,n,[](int64_t *p,size_t nn){ return pure_residual::sort_i64(p,nn); });}
inline int sort_i32(int32_t *a, size_t n){
  return dispatch(a,n,[](int32_t *p,size_t nn){ return pure_residual::sort_i32(p,nn); });}
inline int sort_u32(uint32_t *a, size_t n){
  return dispatch(a,n,[](uint32_t *p,size_t nn){ return pure_residual::sort_u32(p,nn); });}

} // namespace hybrid_residual
