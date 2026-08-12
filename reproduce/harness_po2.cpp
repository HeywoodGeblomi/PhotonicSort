/*
 * PO2 reference harness — multi-type pure residual vs pdq
 * One-command Docker ratios. EXTERNAL-clean. Not field-level.
 * THE BEASTIE BOYZ — PO2 2026-08-12
 */
#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <random>
#include <string>
#include <vector>
#include <functional>
#include "baselines/pdqsort.h"
#include "residual/pure_residual_menu_i32.hpp"
#include "residual/pure_residual_menu_u32.hpp"

using Clock = std::chrono::steady_clock;
template<typename F>
double med(F f, int R) {
    std::vector<double> t;
    for (int r = 0; r < R; ++r) {
        auto a = Clock::now(); f(); auto b = Clock::now();
        t.push_back(std::chrono::duration<double, std::milli>(b - a).count());
    }
    std::sort(t.begin(), t.end());
    return t[R / 2];
}

template<typename T, typename Gen>
void row(const char* type, const char* name, void(*sfn)(T*, size_t),
         Gen gen, size_t n, int reps) {
    auto base = gen(n);
    auto a = base, b = base;
    double p = med([&]{ a = base; sfn(a.data(), n); }, reps);
    double q = med([&]{ b = base; pdqsort(b.begin(), b.end()); }, reps);
    int ok = (std::is_sorted(a.begin(), a.end()) && a == b) ? 1 : 0;
    printf("%-4s %-16s %7.3f %8.3f %8.3f %d\n", type, name, p/q, p, q, ok);
}

void si(int32_t* a, size_t n) { pure_residual::sort_i32(a, n); }
void su(uint32_t* a, size_t n) { pure_residual::sort_u32(a, n); }

int main(int argc, char** argv) {
    size_t n = 1000000;
    int reps = 7;
    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == "--n" && i+1 < argc) n = std::atoll(argv[++i]);
        if (std::string(argv[i]) == "--reps" && i+1 < argc) reps = std::atoi(argv[++i]);
    }
    printf("# PhotonicSort PO2 reference ratios  n=%zu reps=%d\n", n, reps);
    printf("# type pattern          menu/pdq  menu_ms   pdq_ms ok\n");
    printf("# --- soft-spot / structured ---\n");
    auto pipe = [](size_t n) {
        std::vector<int32_t> v(n);
        for (size_t i = 0; i < n; ++i) v[i] = (int32_t)((i % 3 == 0) ? i : (n - i));
        return v;
    };
    auto pipe_u = [](size_t n) {
        std::vector<uint32_t> v(n);
        for (size_t i = 0; i < n; ++i) v[i] = (uint32_t)((i % 3 == 0) ? i : (n - i));
        return v;
    };
    row("i32", "pipe_sparse", si, pipe, n, reps);
    row("u32", "pipe_sparse", su, pipe_u, n, reps);

    auto almost = [](size_t n) {
        std::vector<int32_t> v(n);
        for (size_t i = 0; i < n; ++i) v[i] = (int32_t)i;
        std::mt19937_64 r(42);
        std::uniform_int_distribution<size_t> x(0, n - 1);
        for (size_t s = 0; s < n / 1000; ++s) std::swap(v[x(r)], v[x(r)]);
        return v;
    };
    row("i32", "almost_sorted", si, almost, n, reps);

    auto ts = [](size_t n) {
        std::vector<int32_t> v(n);
        std::mt19937 r(42);
        std::uniform_int_distribution<int> j(0, 5);
        std::uniform_real_distribution<double> jp(0, 1);
        int32_t c = 1000000;
        for (size_t i = 0; i < n; ++i) {
            if (jp(r) < 0.002) c += 10000;
            else c += 1 + j(r);
            v[i] = c;
        }
        return v;
    };
    row("i32", "timestamps", si, ts, n, reps);

    auto few_w = [](size_t n) {
        std::vector<int32_t> v(n);
        std::mt19937 r(42);
        std::uniform_int_distribution<int> p(0, 15);
        for (size_t i = 0; i < n; ++i) v[i] = (int32_t)(p(r) * 1000003);
        return v;
    };
    row("i32", "few_k16_wide", si, few_w, n, reps);

    auto db = [](size_t n) {
        std::vector<int32_t> v(n);
        std::mt19937 r(42);
        std::uniform_int_distribution<int> g(0, 3);
        int32_t c = 1000;
        for (size_t i = 0; i < n; ++i) { c = c + 1 + g(r); v[i] = c; }
        std::uniform_int_distribution<size_t> x(0, n - 1);
        for (size_t s = 0; s < n / 200; ++s) std::swap(v[x(r)], v[x(r)]);
        return v;
    };
    row("i32", "db_pk", si, db, n, reps);

    printf("# --- HE / residual quality ---\n");
    auto rnd = [](size_t n) {
        std::vector<int32_t> v(n);
        std::mt19937_64 r(42);
        std::uniform_int_distribution<int32_t> d(INT32_MIN, INT32_MAX);
        for (auto& x : v) x = d(r);
        return v;
    };
    row("i32", "random", si, rnd, n, reps);

    auto gauss = [](size_t n) {
        std::vector<int32_t> v(n);
        std::mt19937 r(42);
        std::normal_distribution<double> d(0, n / 8.0);
        for (size_t i = 0; i < n; ++i) v[i] = (int32_t)std::llround(d(r));
        return v;
    };
    row("i32", "gaussianish", si, gauss, n, reps);

    auto organ = [](size_t n) {
        std::vector<int32_t> v(n);
        for (size_t i = 0; i < n / 2; ++i) { v[i] = (int32_t)i; v[n - 1 - i] = (int32_t)i; }
        return v;
    };
    row("i32", "organpipe", si, organ, n, reps);

    auto saw = [](size_t n) {
        std::vector<int32_t> v(n);
        for (size_t i = 0; i < n; ++i) v[i] = (int32_t)(i % 1024);
        return v;
    };
    row("i32", "sawtooth", si, saw, n, reps);

    printf("# END ok=1 expected for all rows. Compare ratios not absolute ms.\n");
    printf("# Reference: docs/phase0_field/REFERENCE_RATIOS_PO2.md\n");
    return 0;
}
