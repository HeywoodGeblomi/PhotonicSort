/* dual_evidence_c.h — C API for Python ctypes / FFI bind
 * THE BEASTIE BOYZ / Blam · EXTERNAL-clean
 */
#ifndef DUAL_EVIDENCE_C_H
#define DUAL_EVIDENCE_C_H

#ifdef __cplusplus
extern "C" {
#endif

float de_sigma_amps(const float* amps, int n);
int   de_classical_choose(float L_rate, float L_cv, float R_rate, float R_cv);
int   de_dual_choose(float L_rate, float L_sigma, float R_rate, float R_sigma, float floor);
int   de_dual_confirm_choose(float L_rate, float L_cv, float L_sigma,
                             float R_rate, float R_cv, float R_sigma,
                             float expected_polarity, float floor);
int   de_second_solid(float sigma_delta, float expected_polarity, float floor);

#ifdef __cplusplus
}
#endif
#endif
