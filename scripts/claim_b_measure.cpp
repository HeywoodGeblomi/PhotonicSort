/*
 * Claim B measurement: minimax residual wall-clock on F1/F2 phase pairs
 * classical hybrid vs -DSECONDARY_PARITY dual-evidence hybrid.
 * EXTERNAL-clean. THE BEASTIE BOYZ / Blam 2026-08-14
 *
 * Build twice:
 *   g++ -O3 -DNDEBUG -o claim_b_classical ... (no SECONDARY_PARITY)
 *   g++ -O3 -DNDEBUG -DSECONDARY_PARITY -o claim_b_dual ...
 * Compare μ = max(phase0, phase1) across gates.
 */
#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <chrono>
#include <algorithm>

#include "hybrid_residual_menu.hpp"
#include "pdqsort.h"

static bool is_sorted_i64(const int64_t *a, size_t n) {
    for (size_t i = 1; i < n; ++i)
        if (a[i] < a[i - 1]) return false;
    return true;
}

static void gen_f1(int64_t *a, size_t n, int n_spikes, int phase) {
    for (size_t i = 0; i < n; ++i) a[i] = (int64_t)i;
    size_t base_gap = n / (size_t)(n_spikes + 1);
    if (base_gap < 4) base_gap = 4;
    for (int i = 0; i < n_spikes; ++i) {
        int amp = 4 + i * 3;
        if (amp > (int)(base_gap / 3)) amp = (int)(base_gap / 3);
        if (amp > 64) amp = 64;
        if (amp < 2) amp = 2;
        size_t pos = base_gap * (size_t)(i + 1) + (size_t)(phase * (int)(base_gap / 2));
        pos = pos % (n - (size_t)amp - 1);
        for (int L = 0, R = amp - 1; L < R; ++L, --R) {
            int64_t t = a[pos + (size_t)L];
            a[pos + (size_t)L] = a[pos + (size_t)R];
            a[pos + (size_t)R] = t;
        }
    }
}

static void gen_organpipe(int64_t *a, size_t n) {
    size_t half = n / 2;
    for (size_t i = 0; i < half; ++i) a[i] = (int64_t)i;
    for (size_t i = half; i < n; ++i)
        a[i] = (int64_t)(n - half - 1 - (i - half));
}

static void gen_f2_org(int64_t *a, size_t n, int n_spikes, int phase) {
    gen_organpipe(a, n);
    size_t base_gap = n / (size_t)(n_spikes + 1);
    if (base_gap < 4) base_gap = 4;
    for (int i = 0; i < n_spikes; ++i) {
        int amp = 4 + i * 3;
        if (amp > (int)(base_gap / 3)) amp = (int)(base_gap / 3);
        if (amp > 48) amp = 48;
        if (amp < 2) amp = 2;
        size_t pos = base_gap * (size_t)(i + 1) + (size_t)(phase * (int)(base_gap / 2));
        pos = pos % (n - (size_t)amp - 1);
        for (int L = 0, R = amp - 1; L < R; ++L, --R) {
            int64_t t = a[pos + (size_t)L];
            a[pos + (size_t)L] = a[pos + (size_t)R];
            a[pos + (size_t)R] = t;
        }
    }
}

static void gen_reverse_segments(int64_t *a, size_t n, size_t B = 256) {
    for (size_t i = 0; i < n; ++i) a[i] = (int64_t)i;
    for (size_t start = 0; start + B <= n; start += B) {
        for (size_t L = 0, R = B - 1; L < R; ++L, --R) {
            int64_t t = a[start + L];
            a[start + L] = a[start + R];
            a[start + R] = t;
        }
    }
}

static void gen_f2_rseg(int64_t *a, size_t n, int n_spikes, int phase) {
    gen_reverse_segments(a, n, 256);
    size_t base_gap = n / (size_t)(n_spikes + 1);
    if (base_gap < 4) base_gap = 4;
    for (int i = 0; i < n_spikes; ++i) {
        int amp = 4 + i * 2;
        if (amp > 32) amp = 32;
        if (amp < 2) amp = 2;
        size_t pos = base_gap * (size_t)(i + 1) + (size_t)(phase * (int)(base_gap / 2));
        pos = pos % (n - (size_t)amp - 1);
        for (int L = 0, R = amp - 1; L < R; ++L, --R) {
            int64_t t = a[pos + (size_t)L];
            a[pos + (size_t)L] = a[pos + (size_t)R];
            a[pos + (size_t)R] = t;
        }
    }
}

static double time_hybrid(int64_t *src, size_t n, int reps, bool *ok) {
    std::vector<int64_t> buf(n);
    double best = 1e300;
    *ok = true;
    for (int r = 0; r < reps; ++r) {
        std::memcpy(buf.data(), src, n * sizeof(int64_t));
        auto t0 = std::chrono::steady_clock::now();
        hybrid_residual::sort_i64(buf.data(), n);
        auto t1 = std::chrono::steady_clock::now();
        double ms = std::chrono::duration<double, std::milli>(t1 - t0).count();
        if (ms < best) best = ms;
        if (!is_sorted_i64(buf.data(), n)) *ok = false;
    }
    return best;
}

static double time_pdq(int64_t *src, size_t n, int reps) {
    std::vector<int64_t> buf(n);
    double best = 1e300;
    for (int r = 0; r < reps; ++r) {
        std::memcpy(buf.data(), src, n * sizeof(int64_t));
        auto t0 = std::chrono::steady_clock::now();
        pdqsort(buf.data(), buf.data() + n);
        auto t1 = std::chrono::steady_clock::now();
        double ms = std::chrono::duration<double, std::milli>(t1 - t0).count();
        if (ms < best) best = ms;
    }
    return best;
}

struct PairResult {
    const char *family;
    size_t n;
    double t0, t1, mu;
    double pdq0, pdq1;
    int ok0, ok1;
};

static void run_pair(const char *family, size_t n, int spikes,
                     void (*gen)(int64_t *, size_t, int, int),
                     int reps, PairResult *out) {
    std::vector<int64_t> x0(n), x1(n);
    gen(x0.data(), n, spikes, 0);
    gen(x1.data(), n, spikes, 1);
    bool ok0, ok1;
    out->family = family;
    out->n = n;
    out->t0 = time_hybrid(x0.data(), n, reps, &ok0);
    out->t1 = time_hybrid(x1.data(), n, reps, &ok1);
    out->mu = std::max(out->t0, out->t1);
    out->pdq0 = time_pdq(x0.data(), n, reps);
    out->pdq1 = time_pdq(x1.data(), n, reps);
    out->ok0 = ok0 ? 1 : 0;
    out->ok1 = ok1 ? 1 : 0;
}

int main(int argc, char **argv) {
    int reps = 7;
    if (argc > 1) reps = std::atoi(argv[1]);
#ifdef SECONDARY_PARITY
    const char *gate = "dual";
#else
    const char *gate = "classical";
#endif
    std::printf("# claim_b_measure gate=%s reps=%d\n", gate, reps);
    std::printf("gate,family,n,phase0_ms,phase1_ms,mu_ms,pdq0_ms,pdq1_ms,ok0,ok1\n");

    struct Spec {
        const char *name;
        size_t n;
        int spikes;
        void (*gen)(int64_t *, size_t, int, int);
    };
    Spec specs[] = {
        {"f1", 4096, 16, gen_f1},
        {"f1", 8192, 20, gen_f1},
        {"f1", 32768, 40, gen_f1},
        {"f1", 100000, 40, gen_f1},
        {"f1", 1000000, 64, gen_f1},
        {"f2_organpipe", 8192, 24, gen_f2_org},
        {"f2_organpipe", 32768, 40, gen_f2_org},
        {"f2_organpipe", 100000, 48, gen_f2_org},
        {"f2_organpipe", 1000000, 64, gen_f2_org},
        {"f2_rseg", 8192, 24, gen_f2_rseg},
        {"f2_rseg", 32768, 40, gen_f2_rseg},
        {"f2_rseg", 100000, 48, gen_f2_rseg},
        {"f2_rseg", 1000000, 64, gen_f2_rseg},
    };

    for (auto &s : specs) {
        PairResult r;
        run_pair(s.name, s.n, s.spikes, s.gen, reps, &r);
        std::printf("%s,%s,%zu,%.4f,%.4f,%.4f,%.4f,%.4f,%d,%d\n",
                    gate, r.family, r.n, r.t0, r.t1, r.mu, r.pdq0, r.pdq1, r.ok0, r.ok1);
        std::fflush(stdout);
    }
    return 0;
}
