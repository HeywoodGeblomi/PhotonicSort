#pragma once
/* hybrid_residual_menu v28+ — DEFAULT dual residual production path.
 * Residual talent drives borderline HE by default.
 * Escape hatch: -DCLASSICAL_RESIDUAL restores unconditional ska on mid-band HE.
 * Track 3 thresholds via residual_policy. EXTERNAL-clean. THE BEASTIE BOYZ
 * Dual Residual Deepening 2026-08-23 — extraction-only wire.
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

  size_t inv,eq,u,desc_runs; T mn,mx;
  sample_full(a,n,inv,eq,u,desc_runs,mn,mx);
  const size_t S = residual_policy::SAMPLE_SIZE;
  uint64_t dom = domain_of(mn,mx);

  // ── Dual Residual Deepening extraction ──────────────────────────────
  // Single first-class dual_evidence call. All thresholds + talent from residual_policy.
  // Surface-identical to locked Field-Level Claim v0.5. No pure-specialist prefer yet.
  auto de = secondary_parity::dual_evidence(a, n);
  const bool dual_owned          = de.confirmed;
  const float sigma_delta        = de.sigma_delta;
  const float classical_score    = de.classical_score;
  const residual_policy::ResidualTalent residual_talent = de.suggested;
  // ────────────────────────────────────────────────────────────────────

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

  // Border HE band (exact policy predicate)
  if (residual_policy::is_border_he(u, inv, S)) {
#ifdef CLASSICAL_RESIDUAL
    // Escape hatch: unconditional ska on mid/high unique HE band
    ska_sort(a,a+n); return 0;
#else
    // DEFAULT production: dual residual talent drive on borderline HE
    if (residual_policy::is_strong_he(u, inv, S)) {
      ska_sort(a,a+n); return 0;
    }
    // Extraction-only: keep existing talent → ska routing (T1/T3). Pure-specialist prefer is next deepening.
    if (dual_owned && (residual_talent == residual_policy::ResidualTalent::T3 ||
                       residual_talent == residual_policy::ResidualTalent::T1)) {
      ska_sort(a,a+n); return 0;
    }
    pdqsort(a,a+n); return 0;
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
