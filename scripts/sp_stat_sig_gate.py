#!/usr/bin/env python3
"""SP charged-surface statistical soft-gate.

Reads per-trial raw CSV from expanded_field_bench_hybrid --raw-out:

  arch,type,pattern,n,trial,menu_ms,pdq_ms,ratio_pdq

For each (arch,type,pattern) on the charged surface:
  - median ratio_pdq across trials
  - bootstrap 95% CI of the median
  - soft if CI upper > threshold (default 1.20)

Correctness is not re-checked here (harness already emits ok on summary CSV).

Usage:
  python3 scripts/sp_stat_sig_gate.py raw.csv
  python3 scripts/sp_stat_sig_gate.py raw.csv --threshold 1.20 --bootstrap 2000 --min-reps 11

Exit 0 only if charged soft@threshold (CI upper) = 0.
EXTERNAL-clean. THE BEASTIE BOYZ 2026-08-14
"""
from __future__ import annotations

import argparse
import csv
import random
import sys
from collections import defaultdict

# Matches SP multi-arch charged hard-fail surface (equal_heavy + mixed_blocks excluded)
CHARGED = {
    "sorted", "reverse", "almost_sorted",
    "few_k4_dense", "few_k16_dense",
    "organpipe", "sawtooth", "push_middle", "runs_noise",
    "random", "gaussianish", "adversarial",
    "db_pk", "timestamps",
}


def median(xs: list[float]) -> float:
    ys = sorted(xs)
    n = len(ys)
    if n == 0:
        return float("nan")
    if n % 2:
        return ys[n // 2]
    return 0.5 * (ys[n // 2 - 1] + ys[n // 2])


def bootstrap_median_ci(xs: list[float], b: int, seed: int = 42) -> tuple[float, float, float]:
    """Return (median, lo, hi) 95% CI via percentile bootstrap of the median."""
    rng = random.Random(seed)
    n = len(xs)
    med = median(xs)
    if n < 2:
        return med, med, med
    samples = []
    for _ in range(b):
        draw = [xs[rng.randrange(n)] for _ in range(n)]
        samples.append(median(draw))
    samples.sort()
    lo = samples[max(0, int(0.025 * b))]
    hi = samples[min(b - 1, int(0.975 * b))]
    return med, lo, hi


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("raw_csv")
    ap.add_argument("--threshold", type=float, default=1.20)
    ap.add_argument("--bootstrap", type=int, default=2000)
    ap.add_argument("--min-reps", type=int, default=11,
                    help="Minimum trials per cell; fail if fewer")
    ap.add_argument("--major", type=float, default=1.50)
    args = ap.parse_args()

    cells: dict[tuple, list[float]] = defaultdict(list)
    with open(args.raw_csv, newline="") as f:
        for r in csv.DictReader(f):
            pat = r["pattern"]
            if pat not in CHARGED:
                continue
            key = (r["arch"], r["type"], pat, r["n"])
            cells[key].append(float(r["ratio_pdq"]))

    if not cells:
        print("no charged cells in raw csv", file=sys.stderr)
        return 1

    soft = []
    major = []
    thin = []
    print(f"# sp_stat_sig_gate threshold={args.threshold} bootstrap={args.bootstrap} min_reps={args.min_reps}")
    print(f"# charged patterns={sorted(CHARGED)}")
    for key in sorted(cells):
        arch, typ, pat, n = key
        xs = cells[key]
        if len(xs) < args.min_reps:
            thin.append((key, len(xs)))
        med, lo, hi = bootstrap_median_ci(xs, args.bootstrap)
        flag = ""
        if hi > args.threshold:
            soft.append((key, med, lo, hi))
            flag = " SOFT"
        if med > args.major or hi > args.major:
            major.append((key, med, lo, hi))
            flag += " MAJOR"
        print(
            f"{arch} {typ} {pat} n={n} trials={len(xs)} "
            f"med={med:.4f} ci95=[{lo:.4f},{hi:.4f}]{flag}"
        )

    print(f"# soft_ci_upper={len(soft)} major={len(major)} thin={len(thin)}")
    if thin:
        print("THIN_TRIALS (need more --reps):", thin, file=sys.stderr)
        return 1
    if soft or major:
        print("SP STAT-SIG FAIL charged CI-upper soft or major", file=sys.stderr)
        return 1
    print("SP STAT-SIG GREEN charged CI-upper soft=0 major=0")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
