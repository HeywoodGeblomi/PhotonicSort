/* PhotonicSort C11 — adaptive hybrid sort
 * Brand contract: probe → structure early-exit → residual talent menu
 *
 * Residual menu (Geblomi + GyroRank — Plan A):
 *   - GyroRank pilot gate (observe → abort on clear random)
 *   - LOW_CARD counting residual (few distinct / small range)
 *   - LOW_DISORDER insertion/pdq (almost sorted; cache-local insertion)
 *   - Joint merge gate + capacity-checked run merge (≤32 runs)
 *   - pdqsort-class introsort + LSD radix (int64)
 *   - force_collapse → stable mergesort
 *
 * Version 1.3.2-c — Plan A + Aggressive/ForceHole opt-in modes  MIT
 * Contributors: Heywood Geblomi · Grok (xAI)
 */
#include "photonic_sort.h"

/* ---- Sort mode state (default NORMAL; ForceHole is opt-in) ---- */
static photonic_sort_mode_t g_ps_mode = PHOTONIC_MODE_NORMAL;

void photonic_sort_set_mode(photonic_sort_mode_t mode) {
    if (mode < PHOTONIC_MODE_NORMAL || mode > PHOTONIC_MODE_FORCE_HOLE)
        mode = PHOTONIC_MODE_NORMAL;
    g_ps_mode = mode;
}
photonic_sort_mode_t photonic_sort_get_mode(void) { return g_ps_mode; }

const char *photonic_sort_mode_name(photonic_sort_mode_t mode) {
    switch (mode) {
    case PHOTONIC_MODE_NORMAL:     return "NORMAL";
    case PHOTONIC_MODE_AGGRESSIVE: return "AGGRESSIVE";
    case PHOTONIC_MODE_FORCE_HOLE: return "FORCE_HOLE";
    default: return "NORMAL";
    }
}

/* NOTE: This was a truncated stub to test the channel.
 * The full verified body must replace this. See artifacts.
 */
