/*
 * PhotonicSort independent reproduction harness
 * Measures pure residual menu vs std::sort / pdqsort / ska_sort
 * on the Phase 0 field standard 18-pattern suite.
 *
 * Build:  make   (from reproduce/; uses ../residual/)
 * Run:    ./harness --n 1000000 --reps 5 --out results
 *
 * EXTERNAL-clean pure residual only. Not field-level.
 * THE BEASTIE BOYZ — Wave 0 independent reproduction
 */
#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <string>
#include <vector>

#include "pdqsort.h"
#include "ska_sort.hpp"
#include "pure_residual_menu.hpp"

using Clock = std::chrono::steady_clock;

static std::vector<int64_t> gen_sorted(size_t n) {
    std::vector<int64_t> v(n);
    for (size_t i = 0; i < n; ++i) v[i] = (int64_t)i;
    return v;
}
static std::vector<int64_t> gen_reverse(size_t n) {
    std::vector<int64_t> v(n);
    for (size_t i = 0; i < n; ++i) v[i] = (int64_t)(n - 1 - i);
    return v;
}
static std::vector<int64_t> gen_organpipe(size_t n) {
    std::vector<int64_t> v(n);
    for (size_t i = 0; i < n / 2; ++i) v[i] = (int64_t)i;
    for (size_t i = n / 2; i < n; ++i) v[i] = (int64_t)(n - 1 - i);
    return v;
}
static std::vector<int64_t> gen_sawtooth(size_t n) {
    std::vector<int64_t> v(n);
    for (size_t i = 0; i < n; ++i) v[i] = (int64_t)(i % 64);
    return v;
}
static std::vector<int64_t> gen_almost_sorted(size_t n) {
    std::vector<int64_t> v(n);
    for (size_t i = 0; i < n; ++i) v[i] = (int64_t)i;
    std::mt19937 rng(42);
    std::uniform_int_distribution<size_t> idx(0, n - 1);
    size_t swaps = std::max<size_t>(1, n / 1000);
    for (size_t s = 0; s < swaps; ++s) std::swap(v[idx(rng)], v[idx(rng)]);
    return v;
}
static std::vector<int64_t> gen_uniform(size_t n) {
    std::vector<int64_t> v(n);
    std::mt19937_64 rng(42);
    std::uniform_int_distribution<int64_t> dist(INT64_MIN, INT64_MAX);
    for (size_t i = 0; i < n; ++i) v[i] = dist(rng);
    return v;
}
static std::vector<int64_t> gen_gauss_nearflat(size_t n) {
    std::vector<int64_t> v(n);
    std::mt19937_64 rng(42);
    std::normal_distribution<double> dist(0.0, 1e15);
    for (size_t i = 0; i < n; ++i) v[i] = (int64_t)dist(rng);
    return v;
}
static std::vector<int64_t> gen_two_values(size_t n) {
    std::vector<int64_t> v(n);
    std::mt19937 rng(42);
    std::uniform_int_distribution<int> coin(0, 1);
    for (size_t i = 0; i < n; ++i) v[i] = coin(rng) ? 1 : 0;
    return v;
}
static std::vector<int64_t> gen_few_k4_wide(size_t n) {
    std::vector<int64_t> v(n);
    std::mt19937_64 rng(42);
    const int64_t keys[4] = {0, (int64_t)1e15, (int64_t)-1e15, INT64_MAX / 2};
    std::uniform_int_distribution<int> pick(0, 3);
    for (size_t i = 0; i < n; ++i) v[i] = keys[pick(rng)];
    return v;
}
static std::vector<int64_t> gen_few_k16_wide(size_t n) {
    std::vector<int64_t> v(n);
    std::mt19937_64 rng(42);
    std::uniform_int_distribution<int> pick(0, 15);
    for (size_t i = 0; i < n; ++i) v[i] = (int64_t)pick(rng) * (int64_t)1e14;
    return v;
}
static std::vector<int64_t> gen_few_k16_dense(size_t n) {
    std::vector<int64_t> v(n);
    for (size_t i = 0; i < n; ++i) v[i] = (int64_t)(i % 16);
    return v;
}
static std::vector<int64_t> gen_zipf_k16(size_t n) {
    std::vector<int64_t> v(n);
    std::mt19937 rng(42);
    double weights[16], sum = 0;
    for (int k = 0; k < 16; ++k) { weights[k] = 1.0 / (k + 1); sum += weights[k]; }
    for (int k = 0; k < 16; ++k) weights[k] /= sum;
    std::uniform_real_distribution<double> u(0, 1);
    for (size_t i = 0; i < n; ++i) {
        double r = u(rng), c = 0;
        int chosen = 15;
        for (int k = 0; k < 16; ++k) { c += weights[k]; if (r <= c) { chosen = k; break; } }
        v[i] = chosen;
    }
    return v;
}
static std::vector<int64_t> gen_reverse_segments_8(size_t n) {
    std::vector<int64_t> v(n);
    for (size_t i = 0; i < n; ++i) v[i] = (int64_t)i;
    size_t segs = 8, len = n / segs;
    for (size_t s = 0; s < segs; ++s) {
        size_t lo = s * len, hi = (s + 1 == segs) ? n : (s + 1) * len;
        std::reverse(v.begin() + (long)lo, v.begin() + (long)hi);
    }
    return v;
}
static std::vector<int64_t> gen_push_middle(size_t n) {
    std::vector<int64_t> v(n);
    for (size_t i = 0; i < n; ++i) v[i] = (int64_t)i;
    std::mt19937 rng(42);
    size_t mid = n / 2, island = n / 20, lo = mid - island / 2;
    std::shuffle(v.begin() + (long)lo, v.begin() + (long)(lo + island), rng);
    return v;
}
static std::vector<int64_t> gen_equal_heavy(size_t n) {
    std::vector<int64_t> v(n);
    std::mt19937 rng(42);
    std::uniform_int_distribution<int> coin(0, 9);
    for (size_t i = 0; i < n; ++i) v[i] = (coin(rng) < 8) ? 0 : (int64_t)(i % 100);
    return v;
}
static std::vector<int64_t> gen_adversarial_pipe(size_t n) {
    std::vector<int64_t> v(n);
    std::mt19937_64 rng(42);
    std::uniform_int_distribution<int64_t> dist(0, (int64_t)n * 2);
    for (size_t i = 0; i < n; ++i) v[i] = dist(rng);
    return v;
}
static std::vector<int64_t> gen_db_pk(size_t n) {
    std::vector<int64_t> v(n);
    std::mt19937_64 rng(42);
    std::uniform_int_distribution<int> gap(0, 3);
    int64_t cur = 1000;
    for (size_t i = 0; i < n; ++i) { cur += 1 + gap(rng); v[i] = cur; }
    std::uniform_int_distribution<size_t> idx(0, n - 1);
    for (size_t s = 0; s < n / 200; ++s) std::swap(v[idx(rng)], v[idx(rng)]);
    return v;
}
static std::vector<int64_t> gen_timestamps(size_t n) {
    std::vector<int64_t> v(n);
    std::mt19937_64 rng(42);
    std::normal_distribution<double> jitter(0.0, 2.0);
    int64_t base = 1700000000000LL;
    for (size_t i = 0; i < n; ++i) { base += 10; v[i] = base + (int64_t)jitter(rng); }
    return v;
}

struct Pattern { const char *name; std::vector<int64_t> (*gen)(size_t n); };
static Pattern standard_suite[] = {
    {"sorted", gen_sorted}, {"reverse", gen_reverse}, {"organpipe", gen_organpipe},
    {"sawtooth", gen_sawtooth}, {"almost_sorted", gen_almost_sorted},
    {"uniform_i64", gen_uniform}, {"gauss_nearflat", gen_gauss_nearflat},
    {"two_values", gen_two_values}, {"few_k4_wide", gen_few_k4_wide},
    {"few_k16_wide", gen_few_k16_wide}, {"few_k16_dense", gen_few_k16_dense},
    {"zipf_k16", gen_zipf_k16}, {"reverse_segments_8", gen_reverse_segments_8},
    {"push_middle", gen_push_middle}, {"equal_heavy", gen_equal_heavy},
    {"adversarial_pipe", gen_adversarial_pipe}, {"db_pk", gen_db_pk},
    {"timestamps", gen_timestamps},
};
static constexpr size_t N_PATTERNS = sizeof(standard_suite) / sizeof(standard_suite[0]);

static bool is_sorted(const int64_t *a, size_t n) {
    for (size_t i = 1; i < n; ++i) if (a[i] < a[i - 1]) return false;
    return true;
}
static double median_ns(std::vector<double> &xs) {
    std::sort(xs.begin(), xs.end());
    return xs[xs.size() / 2];
}
enum class Baseline { Photonic, Std, Pdq, Ska };
static void run_baseline(Baseline b, int64_t *a, size_t n) {
    switch (b) {
    case Baseline::Photonic: pure_residual::sort_i64(a, n); break;
    case Baseline::Std: std::sort(a, a + n); break;
    case Baseline::Pdq: pdqsort(a, a + n); break;
    case Baseline::Ska: ska_sort(a, a + n); break;
    }
}
struct Row {
    std::string name;
    double photonic_ms, std_ms, pdq_ms, ska_ms;
    int ok;
};

int main(int argc, char **argv) {
    size_t n = 1000000;
    int reps = 5;
    std::string out_dir = "results";
    std::string arch = "local";
    for (int i = 1; i < argc; ++i) {
        std::string a = argv[i];
        if (a == "--n" && i + 1 < argc) n = (size_t)std::stoull(argv[++i]);
        else if (a == "--reps" && i + 1 < argc) reps = std::stoi(argv[++i]);
        else if (a == "--out" && i + 1 < argc) out_dir = argv[++i];
        else if (a == "--arch-tag" && i + 1 < argc) arch = argv[++i];
        else if (a == "--help") {
            std::printf("Usage: ./harness [--n N] [--reps R] [--out DIR] [--arch-tag TAG]\n");
            return 0;
        }
    }
    (void)std::system(("mkdir -p " + out_dir).c_str());
    std::printf("PhotonicSort reproduce harness n=%zu reps=%d patterns=%zu\n", n, reps, N_PATTERNS);

    std::vector<Row> rows;
    for (size_t p = 0; p < N_PATTERNS; ++p) {
        const char *name = standard_suite[p].name;
        std::printf("  %s ... ", name); std::fflush(stdout);
        auto master = standard_suite[p].gen(n);
        auto bench = [&](Baseline b) -> std::pair<double, int> {
            std::vector<double> samples;
            int ok = 1;
            for (int r = 0; r < reps; ++r) {
                auto buf = master;
                if (r == 0) { auto w = master; run_baseline(b, w.data(), n); }
                auto t0 = Clock::now();
                run_baseline(b, buf.data(), n);
                auto t1 = Clock::now();
                samples.push_back(std::chrono::duration<double, std::nano>(t1 - t0).count());
                if (!is_sorted(buf.data(), n)) ok = 0;
            }
            return {median_ns(samples) / 1e6, ok};
        };
        auto [ph_ms, ph_ok] = bench(Baseline::Photonic);
        auto [st_ms, st_ok] = bench(Baseline::Std);
        auto [pd_ms, pd_ok] = bench(Baseline::Pdq);
        auto [sk_ms, sk_ok] = bench(Baseline::Ska);
        Row row{name, ph_ms, st_ms, pd_ms, sk_ms, ph_ok && st_ok && pd_ok && sk_ok};
        rows.push_back(row);
        std::printf("done\n");
    }

    double log_std = 0, log_pdq = 0, log_ska = 0;
    for (auto &r : rows) {
        log_std += std::log(r.photonic_ms / r.std_ms);
        log_pdq += std::log(r.photonic_ms / r.pdq_ms);
        log_ska += std::log(r.photonic_ms / r.ska_ms);
    }
    double geo_std = std::exp(log_std / rows.size());
    double geo_pdq = std::exp(log_pdq / rows.size());
    double geo_ska = std::exp(log_ska / rows.size());
    std::printf("\nGeo-mean Photonic/std=%.6f Photonic/pdq=%.6f Photonic/ska=%.6f\n",
                geo_std, geo_pdq, geo_ska);

    {
        std::ofstream csv(out_dir + "/gap_map.csv");
        csv << "pattern,photonic_ms,std_ms,pdq_ms,ska_ms,vs_std,vs_pdq,vs_ska,ok\n";
        csv << std::fixed << std::setprecision(4);
        for (auto &r : rows) {
            csv << r.name << "," << r.photonic_ms << "," << r.std_ms << "," << r.pdq_ms << ","
                << r.ska_ms << "," << (r.photonic_ms / r.std_ms) << "," << (r.photonic_ms / r.pdq_ms)
                << "," << (r.photonic_ms / r.ska_ms) << "," << r.ok << "\n";
        }
    }
    {
        std::ofstream md(out_dir + "/gap_map.md");
        md << "# Independent Reproduction Gap Map\n\n";
        md << "**n=" << n << "** · reps=" << reps << " (median) · arch=" << arch << "\n\n";
        md << "Ratios = Photonic / baseline (lower is better for Photonic).\n\n";
        md << "| Pattern | vs std | vs pdq | vs ska | photonic ok |\n";
        md << "|---------|-------:|-------:|-------:|:-----------:|\n";
        md << std::fixed << std::setprecision(3);
        for (auto &r : rows) {
            md << "| " << r.name << " | " << (r.photonic_ms / r.std_ms) << "× | "
               << (r.photonic_ms / r.pdq_ms) << "× | " << (r.photonic_ms / r.ska_ms)
               << "× | " << r.ok << " |\n";
        }
        md << "\n**Geo-mean Photonic/std = " << std::setprecision(3) << geo_std
           << "× · Photonic/pdq = " << geo_pdq << "× · Photonic/ska = " << geo_ska << "×**\n\n";
        md << "Not field-level. Independent reproduction only. EXTERNAL-clean pure residual.\n\n";
        md << "**THE BEASTIE BOYZ**\n";
    }
    std::printf("Wrote %s/gap_map.csv and %s/gap_map.md\n", out_dir.c_str(), out_dir.c_str());
    return 0;
}
