/*
 * PhotonicSort Phase 0 Field Suite Harness — multi-type
 * Real-world + adversarial + classic + n-matrix support.
 * Baselines: std::sort, pdqsort, ska_sort.
 * EXTERNAL-clean pure residual. Not field-level.
 * THE BEASTIE BOYZ — path (a) evidence 2026-08-12
 *
 * Build: g++ -O3 -std=c++17 -DNDEBUG -I. -Iresidual -Ibaselines -o harness_phase0 harness_phase0.cpp
 * Run:   ./harness_phase0 --type i32 --n 1000000 --reps 5 --out results
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
#include <functional>
#include <numeric>

#include "baselines/pdqsort.h"
#include "baselines/ska_sort.hpp"
#include "residual/pure_residual_menu_i32.hpp"
#include "residual/pure_residual_menu_u32.hpp"

using Clock = std::chrono::steady_clock;

template<typename F>
double median_ms(F f, int reps) {
    std::vector<double> t;
    t.reserve(reps);
    for (int r = 0; r < reps; ++r) {
        auto a = Clock::now(); f(); auto b = Clock::now();
        t.push_back(std::chrono::duration<double, std::milli>(b - a).count());
    }
    std::sort(t.begin(), t.end());
    return t[reps / 2];
}

template<typename T>
std::vector<T> gen_sorted(size_t n) {
    std::vector<T> v(n); for (size_t i = 0; i < n; ++i) v[i] = (T)i; return v;
}
template<typename T>
std::vector<T> gen_reverse(size_t n) {
    std::vector<T> v(n); for (size_t i = 0; i < n; ++i) v[i] = (T)(n - 1 - i); return v;
}
template<typename T>
std::vector<T> gen_organpipe(size_t n) {
    std::vector<T> v(n);
    for (size_t i = 0; i < n / 2; ++i) v[i] = (T)i;
    for (size_t i = n / 2; i < n; ++i) v[i] = (T)(n - 1 - i);
    return v;
}
template<typename T>
std::vector<T> gen_sawtooth(size_t n) {
    std::vector<T> v(n); for (size_t i = 0; i < n; ++i) v[i] = (T)(i % 64); return v;
}
template<typename T>
std::vector<T> gen_almost_sorted(size_t n) {
    std::vector<T> v(n); for (size_t i = 0; i < n; ++i) v[i] = (T)i;
    std::mt19937 rng(42);
    std::uniform_int_distribution<size_t> idx(0, n - 1);
    size_t swaps = std::max<size_t>(1, n / 1000);
    for (size_t s = 0; s < swaps; ++s) std::swap(v[idx(rng)], v[idx(rng)]);
    return v;
}
template<typename T>
std::vector<T> gen_uniform(size_t n) {
    std::vector<T> v(n);
    std::mt19937_64 rng(42);
    if constexpr (std::is_same_v<T, int32_t>) {
        std::uniform_int_distribution<int32_t> d(INT32_MIN, INT32_MAX);
        for (auto &x : v) x = d(rng);
    } else {
        std::uniform_int_distribution<uint32_t> d(0, UINT32_MAX);
        for (auto &x : v) x = d(rng);
    }
    return v;
}
template<typename T>
std::vector<T> gen_two_values(size_t n) {
    std::vector<T> v(n); for (size_t i = 0; i < n; ++i) v[i] = (i & 1) ? (T)1 : (T)0; return v;
}
template<typename T>
std::vector<T> gen_few_k16_dense(size_t n) {
    std::vector<T> v(n); for (size_t i = 0; i < n; ++i) v[i] = (T)(i % 16); return v;
}
template<typename T>
std::vector<T> gen_few_k16_wide(size_t n) {
    std::vector<T> v(n);
    std::mt19937 rng(42);
    std::uniform_int_distribution<int> pick(0, 15);
    for (size_t i = 0; i < n; ++i) v[i] = (T)(pick(rng) * 1000003);
    return v;
}
template<typename T>
std::vector<T> gen_zipf_k16(size_t n) {
    std::vector<T> v(n);
    std::mt19937 rng(42);
    std::vector<double> w(16);
    double sum = 0;
    for (int i = 0; i < 16; ++i) { w[i] = 1.0 / (i + 1); sum += w[i]; }
    for (auto &x : w) x /= sum;
    std::discrete_distribution<int> dist(w.begin(), w.end());
    for (size_t i = 0; i < n; ++i) v[i] = (T)dist(rng);
    return v;
}
template<typename T>
std::vector<T> gen_push_middle(size_t n) {
    std::vector<T> v(n); for (size_t i = 0; i < n; ++i) v[i] = (T)i;
    std::mt19937 r(42);
    size_t mid = n / 2, island = n / 20, lo = mid - island / 2;
    std::shuffle(v.begin() + (long)lo, v.begin() + (long)(lo + island), r);
    return v;
}
template<typename T>
std::vector<T> gen_db_pk(size_t n) {
    std::vector<T> v(n);
    std::mt19937 r(42);
    std::uniform_int_distribution<int> gap(0, 3);
    T cur = 1000;
    for (size_t i = 0; i < n; ++i) { cur = (T)(cur + 1 + gap(r)); v[i] = cur; }
    std::uniform_int_distribution<size_t> idx(0, n - 1);
    for (size_t s = 0; s < n / 200; ++s) std::swap(v[idx(r)], v[idx(r)]);
    return v;
}
template<typename T>
std::vector<T> gen_timestamps(size_t n) {
    std::vector<T> v(n);
    std::mt19937 r(42);
    std::uniform_int_distribution<int> jitter(0, 5);
    std::uniform_real_distribution<double> jump(0.0, 1.0);
    T cur = 1000000;
    for (size_t i = 0; i < n; ++i) {
        if (jump(r) < 0.002) cur = (T)(cur + 10000);
        else cur = (T)(cur + 1 + jitter(r));
        v[i] = cur;
    }
    return v;
}
template<typename T>
std::vector<T> gen_equal_heavy(size_t n) {
    std::vector<T> v(n);
    std::mt19937 r(42);
    for (size_t b = 0; b < 8; ++b) {
        size_t lo = (b * n) / 8, hi = ((b + 1) * n) / 8;
        for (size_t i = lo; i < hi; ++i) v[i] = (T)b;
    }
    return v;
}
template<typename T>
std::vector<T> gen_adversarial_pipe(size_t n) {
    auto v = gen_organpipe<T>(n);
    std::mt19937 r(42);
    std::uniform_int_distribution<size_t> idx(0, n - 1);
    for (size_t s = 0; s < n / 500; ++s) std::swap(v[idx(r)], v[idx(r)]);
    return v;
}
template<typename T>
std::vector<T> gen_reverse_segments_8(size_t n) {
    std::vector<T> v(n);
    for (size_t i = 0; i < n; ++i) v[i] = (T)i;
    size_t seg = n / 8;
    for (size_t s = 0; s < 8; ++s) {
        size_t lo = s * seg, hi = (s + 1 == 8) ? n : (s + 1) * seg;
        std::reverse(v.begin() + (long)lo, v.begin() + (long)hi);
    }
    return v;
}
template<typename T>
std::vector<T> gen_nearly_sorted_blocks(size_t n) {
    size_t bs = (size_t)std::sqrt((double)n); if (bs < 16) bs = 16;
    size_t nb = (n + bs - 1) / bs;
    std::vector<T> v(n);
    for (size_t i = 0; i < n; ++i) v[i] = (T)i;
    std::vector<size_t> order(nb);
    std::iota(order.begin(), order.end(), 0);
    std::mt19937 r(42);
    std::shuffle(order.begin(), order.end(), r);
    std::vector<T> out(n);
    size_t pos = 0;
    for (size_t b : order) {
        size_t lo = b * bs, hi = std::min(lo + bs, n);
        for (size_t i = lo; i < hi; ++i) out[pos++] = v[i];
    }
    return out;
}
template<typename T>
std::vector<T> gen_gauss_mix(size_t n) {
    std::vector<T> v(n);
    std::mt19937 r(42);
    std::normal_distribution<double> d0(0.0, 1000.0);
    std::normal_distribution<double> d1(100000.0, 2000.0);
    std::normal_distribution<double> d2(1000000.0, 5000.0);
    std::uniform_int_distribution<int> pick(0, 2);
    for (size_t i = 0; i < n; ++i) {
        double x;
        switch (pick(r)) {
            case 0: x = d0(r); break;
            case 1: x = d1(r); break;
            default: x = d2(r); break;
        }
        v[i] = (T)(int64_t)x;
    }
    return v;
}

struct Pattern {
    const char *name;
    std::function<std::vector<int32_t>(size_t)> gen_i32;
    std::function<std::vector<uint32_t>(size_t)> gen_u32;
};

static Pattern PATTERNS[] = {
    {"sorted", gen_sorted<int32_t>, gen_sorted<uint32_t>},
    {"reverse", gen_reverse<int32_t>, gen_reverse<uint32_t>},
    {"organpipe", gen_organpipe<int32_t>, gen_organpipe<uint32_t>},
    {"sawtooth", gen_sawtooth<int32_t>, gen_sawtooth<uint32_t>},
    {"almost_sorted", gen_almost_sorted<int32_t>, gen_almost_sorted<uint32_t>},
    {"uniform", gen_uniform<int32_t>, gen_uniform<uint32_t>},
    {"two_values", gen_two_values<int32_t>, gen_two_values<uint32_t>},
    {"few_k16_dense", gen_few_k16_dense<int32_t>, gen_few_k16_dense<uint32_t>},
    {"few_k16_wide", gen_few_k16_wide<int32_t>, gen_few_k16_wide<uint32_t>},
    {"zipf_k16", gen_zipf_k16<int32_t>, gen_zipf_k16<uint32_t>},
    {"push_middle", gen_push_middle<int32_t>, gen_push_middle<uint32_t>},
    {"db_pk", gen_db_pk<int32_t>, gen_db_pk<uint32_t>},
    {"timestamps", gen_timestamps<int32_t>, gen_timestamps<uint32_t>},
    {"equal_heavy", gen_equal_heavy<int32_t>, gen_equal_heavy<uint32_t>},
    {"adversarial_pipe", gen_adversarial_pipe<int32_t>, gen_adversarial_pipe<uint32_t>},
    {"reverse_segments_8", gen_reverse_segments_8<int32_t>, gen_reverse_segments_8<uint32_t>},
    {"nearly_sorted_blocks", gen_nearly_sorted_blocks<int32_t>, gen_nearly_sorted_blocks<uint32_t>},
    {"gauss_mix", gen_gauss_mix<int32_t>, gen_gauss_mix<uint32_t>},
};
static const size_t N_PATTERNS = sizeof(PATTERNS) / sizeof(PATTERNS[0]);

struct Row {
    std::string pattern;
    double photonic_ms, std_ms, pdq_ms, ska_ms;
    double vs_std, vs_pdq, vs_ska;
    int ok;
};

template<typename T>
Row bench_one(const char *name, const std::vector<T> &base, int reps,
              int (*photonic)(T *, size_t)) {
    auto a = base, b = base, c = base, d = base;
    size_t n = base.size();
    double p = median_ms([&] { a = base; photonic(a.data(), n); }, reps);
    double s = median_ms([&] { b = base; std::sort(b.begin(), b.end()); }, reps);
    double q = median_ms([&] { c = base; pdqsort(c.begin(), c.end()); }, reps);
    double k = median_ms([&] { d = base; ska_sort(d.begin(), d.end()); }, reps);
    int ok = (std::is_sorted(a.begin(), a.end()) && a == c) ? 1 : 0;
    return {name, p, s, q, k, p / s, p / q, p / k, ok};
}

static double geo_mean(const std::vector<double> &xs) {
    double s = 0; for (double x : xs) s += std::log(x);
    return std::exp(s / xs.size());
}

int main(int argc, char **argv) {
    std::string type = "i32";
    size_t n = 1000000;
    int reps = 5;
    std::string outdir = "results";
    for (int i = 1; i < argc; ++i) {
        std::string a = argv[i];
        if (a == "--type" && i + 1 < argc) type = argv[++i];
        else if (a == "--n" && i + 1 < argc) n = std::stoull(argv[++i]);
        else if (a == "--reps" && i + 1 < argc) reps = std::stoi(argv[++i]);
        else if (a == "--out" && i + 1 < argc) outdir = argv[++i];
    }
    std::string cmd = "mkdir -p " + outdir;
    (void)std::system(cmd.c_str());

    std::printf("Phase0 harness type=%s n=%zu reps=%d patterns=%zu\n",
                type.c_str(), n, reps, N_PATTERNS);

    std::vector<Row> rows;
    for (size_t i = 0; i < N_PATTERNS; ++i) {
        std::printf("  %s ... ", PATTERNS[i].name); std::fflush(stdout);
        Row row;
        if (type == "i32") {
            auto base = PATTERNS[i].gen_i32(n);
            row = bench_one<int32_t>(PATTERNS[i].name, base, reps, pure_residual::sort_i32);
        } else {
            auto base = PATTERNS[i].gen_u32(n);
            row = bench_one<uint32_t>(PATTERNS[i].name, base, reps, pure_residual::sort_u32);
        }
        rows.push_back(row);
        std::printf("done (vs_pdq=%.3f ok=%d)\n", row.vs_pdq, row.ok);
    }

    std::vector<double> vs_std, vs_pdq, vs_ska;
    for (auto &r : rows) {
        vs_std.push_back(r.vs_std);
        vs_pdq.push_back(r.vs_pdq);
        vs_ska.push_back(r.vs_ska);
    }
    double g_std = geo_mean(vs_std), g_pdq = geo_mean(vs_pdq), g_ska = geo_mean(vs_ska);

    std::string md_path = outdir + "/gap_map_" + type + "_n" + std::to_string(n) + ".md";
    std::ofstream md(md_path);
    md << "# Phase 0 Gap Map (" << type << ")\n\n";
    md << "**n=" << n << "** · reps=" << reps << " (median) · patterns=" << N_PATTERNS << "\n\n";
    md << "Ratios = Photonic / baseline (lower is better for Photonic).\n\n";
    md << "| Pattern | vs std | vs pdq | vs ska | ok |\n";
    md << "|---------|-------:|-------:|-------:|:--:|\n";
    for (auto &r : rows) {
        md << "| " << r.pattern
           << " | " << std::fixed << std::setprecision(3) << r.vs_std << "×"
           << " | " << r.vs_pdq << "×"
           << " | " << r.vs_ska << "×"
           << " | " << r.ok << " |\n";
    }
    md << "\n**Geo-mean Photonic/std = " << std::setprecision(3) << g_std
       << "× · Photonic/pdq = " << g_pdq
       << "× · Photonic/ska = " << g_ska << "×**\n\n";
    md << "Phase 0 path-(a) evidence. EXTERNAL-clean. **Not field-level.**\n\n";
    md << "**THE BEASTIE BOYZ**\n";
    md.close();

    std::string csv_path = outdir + "/gap_map_" + type + "_n" + std::to_string(n) + ".csv";
    std::ofstream csv(csv_path);
    csv << "pattern,photonic_ms,std_ms,pdq_ms,ska_ms,vs_std,vs_pdq,vs_ska,ok\n";
    for (auto &r : rows) {
        csv << r.pattern << "," << r.photonic_ms << "," << r.std_ms << ","
            << r.pdq_ms << "," << r.ska_ms << "," << r.vs_std << ","
            << r.vs_pdq << "," << r.vs_ska << "," << r.ok << "\n";
    }
    csv.close();

    std::printf("\nGeo-mean Photonic/std=%.4f Photonic/pdq=%.4f Photonic/ska=%.4f\n",
                g_std, g_pdq, g_ska);
    std::printf("Wrote %s and %s\n", md_path.c_str(), csv_path.c_str());
    return 0;
}
