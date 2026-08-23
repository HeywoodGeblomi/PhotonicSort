#pragma once
// residual_policy.hpp — explicit decision table for hybrid residual path
// Extracted from hybrid_residual_menu.hpp v28. EXTERNAL-clean.
// THE BEASTIE BOYZ 2026-08-23

#include <cstddef>
#include <cmath>
#include <cstdint>

namespace residual_policy {

constexpr size_t SAMPLE_SIZE           = 512;
constexpr size_t STREAM_MAX            = 512;
constexpr size_t COARSE_STRIDE_DIV     = 64;
constexpr size_t DENSE_CONFIRM_MAX     = 512;

constexpr float  CLASSICAL_OWNED_THRESH = 0.04f;
constexpr float  SECOND_SOLID_FLOOR     = 0.005f;
constexpr float  TALENT2_SIGMA_THRESH   = 0.1f;
constexpr float  TALENT3_CLASSICAL      = 0.2f;

constexpr float  STRONG_HE_U_FRAC       = 0.70f;
constexpr float  BORDER_HE_U_FRAC       = 0.50f;

constexpr int    STRONG_INV_MULT        = 3;
constexpr int    BORDER_INV_MULT        = 5;
constexpr int    LOW_INV_MULT           = 20;

constexpr uint64_t COUNT_DOMAIN_MAX     = 65536ull;
constexpr uint64_t COUNT_RANGE_MAX      = 1ull << 20;

enum class ResidualTalent : int { None = 0, T1 = 1, T2 = 2, T3 = 3 };

// Decision table (locked, matches live v28):
// dual_owned = classical_score > CLASSICAL_OWNED_THRESH && dual_confirm(..., SECOND_SOLID_FLOOR)
// if (!dual_owned) → None
// else if (|σ_Δ| > TALENT2_SIGMA_THRESH) → T2
// else if (classical_score > TALENT3_CLASSICAL) → T3
// else → T1
inline ResidualTalent compute_talent(float classical_score, float sigma_delta, bool dual_owned) {
    if (!dual_owned) return ResidualTalent::None;
    if (std::fabs(sigma_delta) > TALENT2_SIGMA_THRESH) return ResidualTalent::T2;
    if (classical_score > TALENT3_CLASSICAL) return ResidualTalent::T3;
    return ResidualTalent::T1;
}

inline bool is_strong_he(size_t u, size_t inv, size_t S = SAMPLE_SIZE) {
    return (u >= size_t(S * STRONG_HE_U_FRAC)) || (inv * STRONG_INV_MULT >= S * 2);
}

inline bool is_border_he(size_t u, size_t inv, size_t S = SAMPLE_SIZE) {
    // Matches the HE-band entry condition u >= 50% && inv*5 >= S*2
    return (u >= size_t(S * BORDER_HE_U_FRAC)) && (inv * BORDER_INV_MULT >= S * 2);
}

inline bool needs_dense_confirm(float coarse_score, float coarse_sigma) {
    return (coarse_score > CLASSICAL_OWNED_THRESH * 0.5f && coarse_score < CLASSICAL_OWNED_THRESH * 2.f)
        || (std::fabs(coarse_sigma) > SECOND_SOLID_FLOOR * 0.5f && std::fabs(coarse_sigma) < SECOND_SOLID_FLOOR * 2.f);
}

} // namespace residual_policy
