#pragma once
/*
 * secondary_parity.hpp — Secondary Parity Signature (σ / σ_Δ)
 * EXTERNAL-clean. Visible metrics only. Flag-gated use in residual menus.
 * THE BEASTIE BOYZ 2026-08-13
 *
 * Formal definition locked in SECONDARY_PARITY_SIGNATURE_PHASE0.md
 * Event logic matches the verified Python generator (Claim A GREEN on F1/F2).
 */
#include <cstddef>
#include <cstdint>
#include <cmath>
#include <algorithm>

#ifndef SECONDARY_PARITY_MAX_EVENTS
#define SECONDARY_PARITY_MAX_EVENTS 512
#endif

namespace secondary_parity {

struct Sigma {
    float even_mean = 0.f;
    float odd_mean  = 0.f;
    float delta     = 0.f;  // even_mean - odd_mean
    int   n_odd_events = 0;
};

/** Compute σ from a continuous stream of local disorder metrics.
 *  stream[0..T), threshold c.
 *  Events = threshold crossings (or sign changes when c≈0).
 *  Restrict to odd-indexed events; partition those by ordinal parity;
 *  return contrast of means.
 */
inline Sigma compute(const float* stream, int T, float c = 0.05f) {
    Sigma s;
    if (T < 4) return s;

    int events[SECONDARY_PARITY_MAX_EVENTS];
    int ne = 0;
    bool above = stream[0] > c;
    for (int t = 1; t < T && ne < SECONDARY_PARITY_MAX_EVENTS; ++t) {
        bool now = stream[t] > c;
        if (now != above) {
            events[ne++] = t;
            above = now;
        }
    }
    if (ne < 4) return s;

    // Odd-indexed events of the full event list (0-based even indices = 1st, 3rd, ...)
    int odd_events[SECONDARY_PARITY_MAX_EVENTS / 2];
    int no = 0;
    for (int i = 0; i < ne; i += 2) {
        if (no < (int)(sizeof(odd_events)/sizeof(odd_events[0])))
            odd_events[no++] = events[i];
    }
    if (no < 2) return s;

    float sum_even = 0.f, sum_odd = 0.f;
    int n_even = 0, n_odd = 0;
    for (int i = 0; i < no; ++i) {
        float val = stream[odd_events[i]];
        if ((i % 2) == 0) { sum_even += val; ++n_even; }  // even ordinal among odd-events
        else              { sum_odd  += val; ++n_odd;  }
    }
    s.even_mean = n_even ? sum_even / n_even : 0.f;
    s.odd_mean  = n_odd  ? sum_odd  / n_odd  : 0.f;
    s.delta     = s.even_mean - s.odd_mean;
    s.n_odd_events = no;
    return s;
}

/** Build a short polarity stream from successive comparisons on an array.
 *  polarity[i] = +1 if a[i+1] > a[i], -1 if a[i+1] < a[i], 0 if equal.
 *  Returns number of polarity samples written (≤ max_out).
 */
template <class T>
inline int polarity_stream(const T* a, size_t n, float* out, int max_out) {
    int Tlen = 0;
    for (size_t i = 0; i + 1 < n && Tlen < max_out; ++i) {
        if (a[i + 1] > a[i])      out[Tlen++] = 1.f;
        else if (a[i + 1] < a[i]) out[Tlen++] = -1.f;
        else                      out[Tlen++] = 0.f;
    }
    return Tlen;
}

/** Strided polarity stream (cheaper for large n). */
template <class T>
inline int polarity_stream_strided(const T* a, size_t n, size_t stride,
                                   float* out, int max_out) {
    int Tlen = 0;
    if (stride < 1) stride = 1;
    for (size_t i = 0; i + stride < n && Tlen < max_out; i += stride) {
        if (a[i + stride] > a[i])      out[Tlen++] = 1.f;
        else if (a[i + stride] < a[i]) out[Tlen++] = -1.f;
        else                           out[Tlen++] = 0.f;
    }
    return Tlen;
}

/** Dual-evidence predicate: classical evidence + second solid (σ_Δ polarity).
 *  Returns true iff both classical and second solid confirm (OWNED).
 *  Single classical without second solid → PREPARED / abatable (caller abates).
 */
inline bool dual_confirm(bool classical, float sigma_delta,
                         float expected_polarity = 0.f,
                         float floor = 0.005f) {
    if (!classical) return false;
    if (std::fabs(sigma_delta) < floor) return false;  // weak signal → no second solid
    if (expected_polarity == 0.f)
        return true;  // any strong σ_Δ counts as second solid
    return (sigma_delta * expected_polarity) > 0.f;
}

} // namespace secondary_parity
