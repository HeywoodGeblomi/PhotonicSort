#pragma once
/*
 * spike_agent_gate.hpp — C++ residual gate for closed-loop spike agents
 * Bridge surface: Python spike agents ↔ high-performance dual_evidence core.
 * EXTERNAL-clean. THE BEASTIE BOYZ / Blam 2026-08-14
 */
#include "dual_evidence.hpp"
#include <cstddef>
#include <cmath>

namespace dual_evidence {
namespace spike_agent {

enum class Action : int { STAY = 0, LEFT = 1, RIGHT = 2 };

struct ChannelSense {
    float rate = 0.f;
    float cv = 0.f;
    float sigma = 0.f;
    int n = 0;
};

inline float sigma_from_amps(const float* amps, int n) {
    if (n < 8) return 0.f;
    float sum_e = 0.f, sum_o = 0.f;
    int ne = 0, no = 0, odd_count = 0;
    for (int i = 0; i < n; i += 2) {
        if ((odd_count % 2) == 0) { sum_e += amps[i]; ++ne; }
        else { sum_o += amps[i]; ++no; }
        ++odd_count;
    }
    if (ne < 1 || no < 1) return 0.f;
    return (sum_e / ne) - (sum_o / no);
}

inline ChannelSense sense_channel(const float* times, const float* amps, int n, float T) {
    ChannelSense s;
    s.n = n;
    if (n <= 0 || T <= 0.f) return s;
    s.rate = static_cast<float>(n) / T;
    if (n >= 3) {
        float sum = 0.f, sum2 = 0.f;
        int m = n - 1;
        for (int i = 0; i < m; ++i) {
            float isi = times[i + 1] - times[i];
            sum += isi; sum2 += isi * isi;
        }
        float mu = sum / m;
        float var = sum2 / m - mu * mu;
        if (var < 0.f) var = 0.f;
        s.cv = (mu > 1e-12f) ? std::sqrt(var) / mu : 0.f;
    }
    s.sigma = sigma_from_amps(amps, n);
    return s;
}

inline Action classical_choose(const ChannelSense& L, const ChannelSense& R) {
    if (R.rate > L.rate * 1.05f) return Action::RIGHT;
    if (L.rate > R.rate * 1.05f) return Action::LEFT;
    return (R.cv >= L.cv) ? Action::RIGHT : Action::LEFT;
}

inline Action dual_choose(const ChannelSense& L, const ChannelSense& R, float floor = 0.05f) {
    const bool lo = std::fabs(L.sigma) >= floor;
    const bool ro = std::fabs(R.sigma) >= floor;
    if (ro && !lo) return Action::RIGHT;
    if (lo && !ro) return Action::LEFT;
    if (lo && ro) return (std::fabs(R.sigma) >= std::fabs(L.sigma)) ? Action::RIGHT : Action::LEFT;
    return Action::STAY;
}

inline Action dual_confirm_choose(const ChannelSense& L, const ChannelSense& R,
                                  float expected_polarity = 1.f, float floor = 0.05f) {
    Action pref = classical_choose(L, R);
    const ChannelSense& S = (pref == Action::RIGHT) ? R : L;
    const bool classical = (S.rate > 40.f) || (S.cv > 0.7f);
    if (!classical) return Action::STAY;
    if (second_solid(S.sigma, expected_polarity, floor)) return pref;
    return Action::STAY;
}

}  // namespace spike_agent
}  // namespace dual_evidence
