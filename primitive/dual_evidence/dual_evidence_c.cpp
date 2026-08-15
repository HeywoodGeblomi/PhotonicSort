/* dual_evidence_c.cpp — C API implementation */
#include "dual_evidence_c.h"
#include "spike_agent_gate.hpp"

using dual_evidence::spike_agent::Action;
using dual_evidence::spike_agent::ChannelSense;

extern "C" {

float de_sigma_amps(const float* amps, int n) {
    return dual_evidence::spike_agent::sigma_from_amps(amps, n);
}

int de_classical_choose(float L_rate, float L_cv, float R_rate, float R_cv) {
    ChannelSense L, R;
    L.rate = L_rate; L.cv = L_cv;
    R.rate = R_rate; R.cv = R_cv;
    return static_cast<int>(dual_evidence::spike_agent::classical_choose(L, R));
}

int de_dual_choose(float L_rate, float L_sigma, float R_rate, float R_sigma, float floor) {
    ChannelSense L, R;
    L.rate = L_rate; L.sigma = L_sigma;
    R.rate = R_rate; R.sigma = R_sigma;
    return static_cast<int>(dual_evidence::spike_agent::dual_choose(L, R, floor));
}

int de_dual_confirm_choose(float L_rate, float L_cv, float L_sigma,
                           float R_rate, float R_cv, float R_sigma,
                           float expected_polarity, float floor) {
    ChannelSense L, R;
    L.rate = L_rate; L.cv = L_cv; L.sigma = L_sigma;
    R.rate = R_rate; R.cv = R_cv; R.sigma = R_sigma;
    return static_cast<int>(
        dual_evidence::spike_agent::dual_confirm_choose(L, R, expected_polarity, floor));
}

int de_second_solid(float sigma_delta, float expected_polarity, float floor) {
    return dual_evidence::second_solid(sigma_delta, expected_polarity, floor) ? 1 : 0;
}

}  // extern "C"
