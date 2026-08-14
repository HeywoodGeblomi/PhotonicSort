#pragma once
/*
 * dual_evidence.hpp — Dual-Evidence Residual Commitment (standalone)
 *
 * Continuous stream → odd-event restriction → σ_Δ → second solid before
 * irreversible residual choice. EXTERNAL-clean. Visible metrics only.
 *
 * Sort-domain instance: residual/secondary_parity.hpp (PhotonicSort).
 * THE BEASTIE BOYZ / Blam 2026-08-14
 */
#include <cstddef>
#include <cmath>
#include <algorithm>

#ifndef DUAL_EVIDENCE_MAX_EVENTS
#define DUAL_EVIDENCE_MAX_EVENTS 512
#endif

namespace dual_evidence {

struct Sigma {
    float even_mean = 0.f;
    float odd_mean  = 0.f;
    float delta     = 0.f;  // even_mean - odd_mean
    int   n_odd_events = 0;
};

/** σ from continuous scalar stream.
 *  Events = threshold crossings (or sign changes when threshold ≈ 0).
 *  Restrict to odd-indexed events; partition by ordinal parity; contrast means.
 */
inline Sigma compute(const float* stream, int T, float threshold = 0.05f) {
    Sigma s;
    if (T < 4) return s;

    int events[DUAL_EVIDENCE_MAX_EVENTS];
    int ne = 0;
    bool above = stream[0] > threshold;
    for (int t = 1; t < T && ne < DUAL_EVIDENCE_MAX_EVENTS; ++t) {
        bool now = stream[t] > threshold;
        if (now != above) {
            events[ne++] = t;
            above = now;
        }
    }
    if (ne < 4) return s;

    // Odd-indexed events of the full event list (0-based: 0,2,4,... = 1st,3rd,...)
    int odd_events[DUAL_EVIDENCE_MAX_EVENTS / 2];
    int no = 0;
    for (int i = 0; i < ne; i += 2) {
        if (no < (int)(sizeof(odd_events) / sizeof(odd_events[0])))
            odd_events[no++] = events[i];
    }
    if (no < 2) return s;

    float sum_even = 0.f, sum_odd = 0.f;
    int n_even = 0, n_odd = 0;
    for (int i = 0; i < no; ++i) {
        float val = stream[odd_events[i]];
        if ((i % 2) == 0) { sum_even += val; ++n_even; }
        else              { sum_odd  += val; ++n_odd;  }
    }
    s.even_mean = n_even ? sum_even / n_even : 0.f;
    s.odd_mean  = n_odd  ? sum_odd  / n_odd  : 0.f;
    s.delta     = s.even_mean - s.odd_mean;
    s.n_odd_events = no;
    return s;
}

/** Second solid: |σ_Δ| above floor (and optional polarity match). */
inline bool second_solid(float sigma_delta, float expected_polarity = 0.f,
                         float floor = 0.005f) {
    if (std::fabs(sigma_delta) < floor) return false;
    if (expected_polarity == 0.f) return true;
    return (sigma_delta * expected_polarity) > 0.f;
}

/** Dual-evidence commit predicate.
 *  classical ∧ second_solid → OWNED (true).
 *  classical only / weak σ_Δ → PREPARED / abate (false).
 */
inline bool dual_confirm(bool classical, float sigma_delta,
                         float expected_polarity = 0.f,
                         float floor = 0.005f) {
    if (!classical) return false;
    return second_solid(sigma_delta, expected_polarity, floor);
}

} // namespace dual_evidence
