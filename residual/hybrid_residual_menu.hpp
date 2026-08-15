#pragma once
/* hybrid_residual_menu v28 — DEFAULT dual residual production path.
 * Residual talent drives borderline HE by default.
 * Escape hatch: -DCLASSICAL_RESIDUAL restores unconditional ska on mid-band HE.
 * Track 3 thresholds ct=0.04 sf=0.005. EXTERNAL-clean. THE BEASTIE BOYZ */
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
namespace hybrid_residual {
template<typename T> inline bool is_sorted_asc(const T *a, size_t n) {
  for (size_t i=1;i<n;++i) if (a[i]<a[i-1]) return false; return true;}
template<typename T> inline bool is_sorted_desc(const T *a, size_t n) {
  for (size_t i=1;i<n;++i) if (a[i]>a[i-1]) return false; return true;}
template<typename T> inline void sample_full(const T *a, size_t n, size_t &inv, size_t &eq, size_t &u, size_t &desc_runs, T &mn, T &mx) {
  const size_t S=512; inv=eq=desc_runs=0; T samp[512]; mn=mx=a[0]; bool in_desc=false;
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
  if(range>=(1ull<<20)||range+1>=(uint64_t)n) return false;
  size_t *cnt=(size_t*)std::calloc((size_t)range+1,sizeof(size_t)); if(!cnt) return false;
  for(size_t i=0;i<n;++i){ uint64_t idx; if constexpr(std::is_unsigned<T>::value) idx=(uint64_t)a[i]-(uint64_t)amin;
    else idx=(uint64_t)((int64_t)a[i]-(int64_t)amin); cnt[idx]++;}
  size_t p=0; for(uint64_t v=0;v<=range;++v) for(size_t c=cnt[v];c;--c) a[p++]=(T)((int64_t)v+(int64_t)amin);
  std::free(cnt); return true;}
template<typename T, typename PureFn> inline int dispatch(T *a, size_t n, PureFn pure_fn){
  if(n<2) return 0; if(is_sorted_asc(a,n)) return 0; if(is_sorted_desc(a,n)){ std::reverse(a,a+n); return 0;}
  if(residual_reverse_segments::try_reverse_segments(a,n)) return 0;
  if(residual_mixed_blocks::try_mixed_blocks(a,n)) return 0;
  size_t inv,eq,u,desc_runs; T mn,mx; sample_full(a,n,inv,eq,u,desc_runs,mn,mx);
  const size_t S=512; uint64_t dom=domain_of(mn,mx);

  // Dual residual always computed (production default). CLASSICAL_RESIDUAL is escape hatch only.
  float stream[512]; int Tlen=0;
  { size_t step=(n>1024)?(n/512):1; if(step<1) step=1;
    for(size_t i=0;i+step<n && Tlen<512;i+=step){
      if(a[i+step]>a[i]) stream[Tlen++]=1.f; else if(a[i+step]<a[i]) stream[Tlen++]=-1.f; else stream[Tlen++]=0.f;}}
  secondary_parity::Sigma sig=secondary_parity::compute(stream,Tlen,0.f);
  const float sigma_delta=sig.delta;
  const bool second_solid=secondary_parity::dual_confirm(true,sigma_delta,0.f,0.005f);
  float classical_score=0.f; if(Tlen>0){ float sum=0.f; for(int i=0;i<Tlen;++i) sum+=std::fabs(stream[i]); classical_score=sum/(float)Tlen;}
  const bool classical_owned=classical_score>0.04f;
  const bool dual_owned=classical_owned&&second_solid;
  int residual_talent=0;
  if(dual_owned){ if(std::fabs(sigma_delta)>0.1f) residual_talent=2; else if(classical_score>0.2f) residual_talent=3; else residual_talent=1;}

  if(eq*4>=S*3){ pdqsort(a,a+n); return 0;}
  if(dom<=65536ull){ if(try_count_sort(a,n,mn,mx)) return 0;}
  if(u>=(S*50)/100 && inv*10<=S){ pdqsort(a,a+n); return 0;}
  if(u<=32){ if(dom<=65536ull && try_count_sort(a,n,mn,mx)) return 0; return pure_fn(a,n);}
  if(u<=128 && inv*2>=S && inv*2<=S) return pure_fn(a,n);
  if(desc_runs>=3 && inv*5>=S*3) return pure_fn(a,n);
  if(inv*20<=S){
    if(dom<=(uint64_t)n*2ull){ size_t dinv=dense_inv(a,n); size_t ds=dense_inv_samples(n);
      if(ds==0||dinv*50<=ds) return pure_fn(a,n); pdqsort(a,a+n); return 0;}
    pdqsort(a,a+n); return 0;}
  if(u>=(S*50)/100 && inv*5>=S*2){
#ifdef CLASSICAL_RESIDUAL
    // Escape hatch: old classical unconditional ska on mid/high unique HE band
    ska_sort(a,a+n); return 0;
#else
    // DEFAULT production: dual residual talent drive on borderline HE
    const bool strong_he=(u>=(S*70)/100)||(inv*3>=S*2);
    if(strong_he){ ska_sort(a,a+n); return 0;}
    if(dual_owned && (residual_talent==3||residual_talent==1)){ ska_sort(a,a+n); return 0;}
    pdqsort(a,a+n); return 0;
#endif
  }
  { size_t dinv=dense_inv(a,n);
    if(dom<=(uint64_t)n*2ull && dinv>=100){
      if(u>=(S*70)/100 && inv*5>=S*2) ska_sort(a,a+n); else pdqsort(a,a+n); return 0;}}
  pdqsort(a,a+n); return 0;}
inline int sort_i64(int64_t *a, size_t n){ return dispatch(a,n,[](int64_t *p,size_t nn){ return pure_residual::sort_i64(p,nn); });}
inline int sort_i32(int32_t *a, size_t n){ return dispatch(a,n,[](int32_t *p,size_t nn){ return pure_residual::sort_i32(p,nn); });}
inline int sort_u32(uint32_t *a, size_t n){ return dispatch(a,n,[](uint32_t *p,size_t nn){ return pure_residual::sort_u32(p,nn); });}
} // namespace hybrid_residual
