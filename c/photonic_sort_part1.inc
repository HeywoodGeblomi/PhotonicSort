/* PhotonicSort C11 -- adaptive hybrid sort
 * Brand contract: probe -> structure early-exit -> residual talent menu
 *
 * Residual menu (Geblomi + GyroRank -- Plan A):
 *   - GyroRank pilot gate (observe -> abort on clear random)
 *   - LOW_CARD counting residual (few distinct / small range)
 *   - LOW_DISORDER insertion/pdq (almost sorted; cache-local insertion)
 *   - Joint merge gate + capacity-checked run merge (<=32 runs)
 *   - pdqsort-class introsort + LSD radix (int64)
 *   - force_collapse -> stable mergesort
 *
 * Version 1.3.2-c -- Plan A + Aggressive/ForceHole opt-in modes  MIT
 * Contributors: Heywood Geblomi * Grok (xAI)
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
    default: return "UNKNOWN";
    }
}

/* Threshold table keyed by mode.
 * NORMAL     = production baseline (moderate safety-first)
 * AGGRESSIVE = widened early-exit band
 * FORCE_HOLE = maximum hole-in-one attempt (STRUCTURE verify still mandatory)
 */
typedef struct ps_thresh {
    double pilot_ld_inv_hi;   /* pilot LOW_DISORDER inv upper */
    double pilot_ld_dir;      /* pilot LOW_DISORDER dir_rate upper */
    double full_ld_inv;       /* full-probe LOW_DISORDER inv */
    double full_ld_sort;      /* full-probe LOW_DISORDER sortedness */
    size_t low_card_unique;   /* unique_est ceiling for LOW_CARD */
    uint64_t low_card_range;  /* dense range ceiling */
} ps_thresh_t;

static const ps_thresh_t ps_thresh_for[] = {
    /* NORMAL */     { 0.08, 0.12, 0.06, 0.92, 256,  4096 },
    /* AGGRESSIVE */ { 0.14, 0.22, 0.12, 0.88, 512,  8192 },
    /* FORCE_HOLE */ { 0.20, 0.30, 0.16, 0.82, 1024, 16384 },
};

static const ps_thresh_t *ps_get_thresh(void) {
    int m = (int)g_ps_mode;
    if (m < 0 || m > 2) m = 0;
    return &ps_thresh_for[m];
}

/* ---- GyroRank pilot (cheap structure probe) ---- */
typedef struct {
    double inv_ratio;
    double sortedness;
    double dir_rate;
    size_t max_run;
    size_t equal_count;
    size_t unique_est;
    int direction_changes;
    int monotone_sign;
    int confidence;
    int route; /* 0=STRUCTURE, 1=LOW_DISORDER, 2=LOW_CARD, 3=RANDOM */
} gyro_out_t;

static void gyro_pilot(const int64_t *a, size_t n, gyro_out_t *out) {
    memset(out, 0, sizeof(*out));
    if (n < 2) { out->route = 0; out->confidence = 100; return; }
    size_t sample = n < 64 ? n : 64;
    size_t step = n / sample;
    if (step < 1) step = 1;
    size_t inversions = 0, pairs = 0, equal_count = 0;
    int dir_changes = 0, last_sign = 0, monotone_sign = 0;
    size_t max_run = 1, cur_run = 1;
    int64_t prev = a[0];
    for (size_t i = step; i < n; i += step) {
        int64_t x = a[i - step], y = a[i];
        pairs++;
        if (y < x) inversions++;
        if (y == x) equal_count++;
        int sign = (y > x) ? 1 : (y < x) ? -1 : 0;
        if (sign != 0) {
            if (last_sign != 0 && sign != last_sign) dir_changes++;
            last_sign = sign;
            if (monotone_sign == 0) monotone_sign = sign;
            else if (sign != monotone_sign) monotone_sign = 0;
        }
        if (y >= prev) { cur_run++; if (cur_run > max_run) max_run = cur_run; }
        else cur_run = 1;
        prev = y;
    }
    out->inv_ratio = pairs ? (double)inversions / pairs : 0.0;
    out->sortedness = 1.0 - out->inv_ratio;
    out->dir_rate = pairs ? (double)dir_changes / pairs : 0.0;
    out->max_run = max_run;
    out->equal_count = equal_count;
    out->direction_changes = dir_changes;
    out->monotone_sign = monotone_sign;
    out->unique_est = (size_t)(n * (1.0 - (double)equal_count / (pairs + 1)));
    /* confidence: high when extreme */
    if (out->inv_ratio < 0.02 || out->inv_ratio > 0.45) out->confidence = 90;
    else if (out->inv_ratio < 0.08 || out->inv_ratio > 0.30) out->confidence = 70;
    else out->confidence = 40;
    const ps_thresh_t *th = ps_get_thresh();
    if (out->inv_ratio <= th->pilot_ld_inv_hi && out->dir_rate <= th->pilot_ld_dir)
        out->route = 1; /* LOW_DISORDER candidate */
    else if (out->unique_est <= th->low_card_unique)
        out->route = 2; /* LOW_CARD candidate */
    else if (monotone_sign && !dir_changes)
        out->route = 0; /* STRUCTURE */
    else
        out->route = 3; /* RANDOM / residual */
}

/* ---- Run detection for merge residual ---- */
static size_t detect_runs_i64(int64_t *a, size_t n, size_t *starts, size_t max_runs) {
    size_t run_count = 0;
    size_t i = 0;
    while (i < n && run_count < max_runs) {
        size_t j = i + 1;
        if (j < n && a[j] >= a[i]) {
            while (j + 1 < n && a[j + 1] >= a[j]) ++j;
            ++j;
        } else if (j < n) {
            while (j + 1 < n && a[j + 1] < a[j]) ++j;
            ++j;
            /* reverse descending run */
            for (size_t k = i, m = j - 1; k < m; ++k, --m) {
                int64_t t = a[k]; a[k] = a[m]; a[m] = t;
            }
        }
        starts[++run_count] = j; i = j;
    }
    if (run_count <= 1) return 0;
    size_t buf_cap = n / 2 + 1;
