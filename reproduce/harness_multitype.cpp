/*
 * PhotonicSort multi-type harness (Wave 2)
 * Measures pure residual on int32 / uint32 vs std::sort / pdqsort / ska_sort.
 *
 * Wave 0 reproduce/harness.cpp remains the int64 independent-reproduction path.
 * This harness is additive for Barrier 4 (multi-type).
 *
 * Build:  g++ -O3 -std=c++17 -I../residual -Ibaselines -o harness_multitype harness_multitype.cpp
 * Run:    ./harness_multitype --type i32 --n 1000000 --reps 5 --out results_i32
 *
 * EXTERNAL-clean preference. Not field-level.
 * THE BEASTIE BOYZ — Wave 2 multi-type
 */
#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <limits>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <string>
#include <type_traits>
#include <vector>

#include "baselines/pdqsort.h"
#include "baselines/ska_sort.hpp"
#include "pure_residual_menu_i32.hpp"
// sort_u32 follows in a later PR; for now only i32 is live.

using Clock = std::chrono::steady_clock;

template<typename T>
static std::vector<T> gen_sorted(size_t n) {
    std::vector<T> v(n);
    for (size_t i = 0; i < n; ++i) v[i] = (T)i;
    return v;
}
template<typename T>
static std::vector<T> gen_reverse(size_t n) {
    std::vector<T> v(n);
    for (size_t i = 0; i < n; ++i) v[i] = (T)(n - 1 - i);
    return v;
}
template<typename T>
static std::vector<T> gen_organpipe(size_t n) {
    std::vector<T> v(n);
    for (size_t i = 0; i < n / 2; ++i) v[i] = (T)i;
    for (size_t i = n / 2; i < n; ++i) v[i] = (T)(n - 1 - i);
    return v;
}
template<typename T>
static std::vector<T> gen_sawtooth(size_t n) {
    std::vector<T> v(n);
    for (size_t i = 0; i < n; ++i) v[i] = (T)(i % 64);
    return v;
}
template<typename T>
static std::vector<T> gen_almost_sorted(size_t n) {
    std::vector<T> v(n);
    for (size_t i = 0; i < n; ++i) v[i] = (T)i;
    std::mt19937 rng(42);
    std::uniform_int_distribution<size_t> idx(0, n - 1);
    size_t swaps = std::max<size_t>(1, n / 1000);
    for (size_t s = 0; s < swaps; ++s) std::swap(v[idx(rng)], v[idx(rng)]);
    return v;
}
template<typename T>
static std::vector<T> gen_uniform(size_t n) {
    std::vector<T> v(n);
    std::mt19937 rng(42);
    if constexpr (std::is_signed_v<T>) {
        std::uniform_int_distribution<T> dist(std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
        for (size_t i = 0; i < n; ++i) v[i] = dist(rng);
    } else {
        std::uniform_int_distribution<T> dist(0, std::numeric_limits<T>::max());
        for (size_t i = 0; i < n; ++i) v[i] = dist(rng);
    }
    return v;
}
template<typename T>
static std::vector<T> gen_two_values(size_t n) {
    std::vector<T> v(n);
    std::mt19937 rng(42);
    std::uniform_int_distribution<int> coin(0, 1);
    for (size_t i = 0; i < n; ++i) v[i] = coin(rng) ? (T)1 : (T)0;
    return v;
}
template<typename T>
static std::vector<T> gen_few_k16_wide(size_t n) {
    std::vector<T> v(n);
    std::mt19937 rng(42);
    std::uniform_int_distribution<int> pick(0, 15);
    // scale wide keys for 32-bit
    const T scale = (T)100000000; // 1e8
    for (size_t i = 0; i < n; ++i) v[i] = (T)(pick(rng) * scale);
    return v;
}
template<typename T>
static std::vector<T> gen_few_k16_dense(size_t n) {
    std::vector<T> v(n);
    for (size_t i = 0; i < n; ++i) v[i] = (T)(i % 16);
    return v;
}
template<typename T>
static std::vector<T> gen_zipf_k16(size_t n) {
    std::vector<T> v(n);
    std::mt19937 rng(42);
    double weights[16], sum = 0;
    for (int k = 0; k < 16; ++k) { weights[k] = 1.0 / (k + 1); sum += weights[k]; }
    for (int k = 0; k < 16; ++k) weights[k] /= sum;
    std::uniform_real_distribution<double> u(0, 1);
    for (size_t i = 0; i < n; ++i) {
        double r = u(rng), c = 0;
        int chosen = 15;
        for (int k = 0; k < 16; ++k) { c += weights[k]; if (r <= c) { chosen = k; break; } }
        v[i] = (T)chosen;
    }
    return v;
}
template<typename T>
static std::vector<T> gen_push_middle(size_t n) {
    std::vector<T> v(n);
    for (size_t i = 0; i < n; ++i) v[i] = (T)i;
    std::mt19937 rng(42);
    size_t mid = n / 2, island = n / 20, lo = mid - island / 2;
    std::shuffle(v.begin() + (long)lo, v.begin() + (long)(lo + island), rng);
    return v;
}
template<typename T>
static std::vector<T> gen_db_pk(size_t n) {
    std::vector<T> v(n);
    std::mt19937 rng(42);
    std::uniform_int_distribution<int> gap(0, 3);
    T cur = 1000;
    for (size_t i = 0; i < n; ++i) { cur = (T)(cur + 1 + gap(rng)); v[i] = cur; }
    std::uniform_int_distribution<size_t> idx(0, n - 1);
    for (size_t s = 0; s < n / 200; ++s) std::swap(v[idx(rng)], v[idx(rng)]);
    return v;
}

template<typename T>
struct Pattern { const char *name; std::vector<T> (*gen)(size_t n); };

template<typename T>
static bool is_sorted_arr(const T *a, size_t n) {
    for (size_t i = 1; i < n; ++i) if (a[i] < a[i - 1]) return false;
    return true;
}
static double median_ns(std::vector<double> &xs) {
    std::sort(xs.begin(), xs.end());
    return xs[xs.size() / 2];
}

template<typename T>
static void run_photonic(T *a, size_t n) {
    if constexpr (std::is_same_v<T, int32_t>) pure_residual::sort_i32(a, n);
    else if constexpr (std::is_same_v<T, uint32_t>) {
        // sort_u32 not yet landed — temporary std::sort so harness compiles
        std::sort(a, a + n);
    } else static_assert(sizeof(T) == 0, "unsupported type");
}

template<typename T>
static int run_suite(const char *type_tag, size_t n, int reps, const std::string &out_dir, const std::string &arch) {
    Pattern<T> suite[] = {
        {"sorted", gen_sorted<T>}, {"reverse", gen_reverse<T>}, {"organpipe", gen_organpipe<T>},
        {"sawtooth", gen_sawtooth<T>}, {"almost_sorted", gen_almost_sorted<T>},
        {"uniform", gen_uniform<T>}, {"two_values", gen_two_values<T>},
        {"few_k16_wide", gen_few_k16_wide<T>}, {"few_k16_dense", gen_few_k16_dense<T>},
        {"zipf_k16", gen_zipf_k16<T>}, {"push_middle", gen_push_middle<T>}, {"db_pk", gen_db_pk<T>},
    };
    constexpr size_t N_PATTERNS = sizeof(suite) / sizeof(suite[0]);

    std::printf("PhotonicSort multi-type harness type=%s n=%zu reps=%d patterns=%zu\n", type_tag, n, reps, N_PATTERNS);

    struct Row { std::string name; double photonic_ms, std_ms, pdq_ms, ska_ms; int ok; };
    std::vector<Row> rows;

    for (size_t p = 0; p < N_PATTERNS; ++p) {
        const char *name = suite[p].name;
        std::printf("  %s ... ", name); std::fflush(stdout);
        auto master = suite[p].gen(n);
        auto bench = [&](auto fn) -> std::pair<double, int> {
            std::vector<double> samples;
            int ok = 1;
            for (int r = 0; r < reps; ++r) {
                auto buf = master;
                if (r == 0) { auto w = master; fn(w.data(), n); }
                auto t0 = Clock::now();
                fn(buf.data(), n);
                auto t1 = Clock::now();
                samples.push_back(std::chrono::duration<double, std::nano>(t1 - t0).count());
                if (!is_sorted_arr(buf.data(), n)) ok = 0;
            }
            return {median_ns(samples) / 1e6, ok};
        };
        auto [ph_ms, ph_ok] = bench([](T *a, size_t nn){ run_photonic(a, nn); });
        auto [st_ms, st_ok] = bench([](T *a, size_t nn){ std::sort(a, a + nn); });
        auto [pd_ms, pd_ok] = bench([](T *a, size_t nn){ pdqsort(a, a + nn); });
        auto [sk_ms, sk_ok] = bench([](T *a, size_t nn){ ska_sort(a, a + nn); });
        rows.push_back({name, ph_ms, st_ms, pd_ms, sk_ms, ph_ok && st_ok && pd_ok && sk_ok});
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
        std::ofstream csv(out_dir + "/gap_map_" + type_tag + ".csv");
        csv << "pattern,photonic_ms,std_ms,pdq_ms,ska_ms,vs_std,vs_pdq,vs_ska,ok\n";
        csv << std::fixed << std::setprecision(4);
        for (auto &r : rows) {
            csv << r.name << "," << r.photonic_ms << "," << r.std_ms << "," << r.pdq_ms << ","
                << r.ska_ms << "," << (r.photonic_ms / r.std_ms) << "," << (r.photonic_ms / r.pdq_ms)
                << "," << (r.photonic_ms / r.ska_ms) << "," << r.ok << "\n";
        }
    }
    {
        std::ofstream md(out_dir + "/gap_map_" + type_tag + ".md");
        md << "# Multi-type Gap Map (" << type_tag << ")\n\n";
        md << "**n=" << n << "** · reps=" << reps << " (median) · arch=" << arch << "\n\n";
        md << "Ratios = Photonic / baseline (lower is better for Photonic).\n\n";
        md << "| Pattern | vs std | vs pdq | vs ska | ok |\n";
        md << "|---------|-------:|-------:|-------:|:--:|\n";
        md << std::fixed << std::setprecision(3);
        for (auto &r : rows) {
            md << "| " << r.name << " | " << (r.photonic_ms / r.std_ms) << "× | "
               << (r.photonic_ms / r.pdq_ms) << "× | " << (r.photonic_ms / r.ska_ms)
               << "× | " << r.ok << " |\n";
        }
        md << "\n**Geo-mean Photonic/std = " << std::setprecision(3) << geo_std
           << "× · Photonic/pdq = " << geo_pdq << "× · Photonic/ska = " << geo_ska << "×**\n\n";
        md << "Wave 2 multi-type. Not field-level. EXTERNAL-clean preference.\n\n";
        md << "**THE BEASTIE BOYZ**\n";
    }
    std::printf("Wrote %s/gap_map_%s.*\n", out_dir.c_str(), type_tag);
    return 0;
}

int main(int argc, char **argv) {
    size_t n = 1000000;
    int reps = 5;
    std::string out_dir = "results_multitype";
    std::string arch = "local";
    std::string type = "i32";
    for (int i = 1; i < argc; ++i) {
        std::string a = argv[i];
        if (a == "--n" && i + 1 < argc) n = (size_t)std::stoull(argv[++i]);
        else if (a == "--reps" && i + 1 < argc) reps = std::stoi(argv[++i]);
        else if (a == "--out" && i + 1 < argc) out_dir = argv[++i];
        else if (a == "--arch-tag" && i + 1 < argc) arch = argv[++i];
        else if (a == "--type" && i + 1 < argc) type = argv[++i];
        else if (a == "--help") {
            std::printf("Usage: ./harness_multitype [--type i32|u32] [--n N] [--reps R] [--out DIR]\n");
            return 0;
        }
    }
    {
        std::string cmd = "mkdir -p " + out_dir;
        std::system(cmd.c_str());
    }
    if (type == "i32") return run_suite<int32_t>("i32", n, reps, out_dir, arch);
    if (type == "u32") return run_suite<uint32_t>("u32", n, reps, out_dir, arch);
    std::fprintf(stderr, "unknown --type %s (use i32 or u32)\n", type.c_str());
    return 1;
}
