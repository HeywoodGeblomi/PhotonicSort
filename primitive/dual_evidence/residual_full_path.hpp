#pragma once
/*
 * residual_full_path.hpp — B3: FULL residual routes through Secondary Parity core
 * EXTERNAL-clean. THE BEASTIE BOYZ / Blam 2026-08-14
 */
#include "dual_evidence.hpp"
#include "spike_agent_gate.hpp"
#include <cstddef>
#include <cmath>

#if __has_include("../../residual/secondary_parity.hpp")
#include "../../residual/secondary_parity.hpp"
#define DE_HAS_SECONDARY_PARITY 1
#else
#define DE_HAS_SECONDARY_PARITY 0
#endif

namespace dual_evidence {
namespace residual_full {

enum class ResidualTalent : int {
    CHEAP = 0,
    FULL_GENERIC = 1,
    FULL_STRUCTURE = 2,
    FULL_HE = 3,
};

struct FullDecision {
    bool owned = false;
    ResidualTalent talent = ResidualTalent::CHEAP;
    float sigma_delta = 0.f;
    float classical_score = 0.f;
};

inline float classical_score(const float* stream, int T) {
    if (T <= 0) return 0.f;
    float s = 0.f;
    for (int i = 0; i < T; ++i) s += std::fabs(stream[i]);
    return s / static_cast<float>(T);
}

inline FullDecision decide(const float* stream, int T,
                           float classical_threshold = 0.08f,
                           float sigma_floor = 0.02f,
                           float expected_polarity = 0.f) {
    FullDecision d;
    d.classical_score = classical_score(stream, T);
#if DE_HAS_SECONDARY_PARITY
    auto sp = secondary_parity::compute(stream, T, classical_threshold);
    d.sigma_delta = sp.delta;
#else
    auto s = dual_evidence::compute(stream, T, classical_threshold);
    d.sigma_delta = s.delta;
#endif
    const bool classical = d.classical_score > classical_threshold;
    d.owned = classical && dual_evidence::second_solid(
        d.sigma_delta, expected_polarity, sigma_floor);
    if (!d.owned) {
        d.talent = ResidualTalent::CHEAP;
        return d;
    }
    if (std::fabs(d.sigma_delta) > 0.1f)
        d.talent = ResidualTalent::FULL_STRUCTURE;
    else if (d.classical_score > 0.2f)
        d.talent = ResidualTalent::FULL_HE;
    else
        d.talent = ResidualTalent::FULL_GENERIC;
    return d;
}

}  // namespace residual_full
}  // namespace dual_evidence
