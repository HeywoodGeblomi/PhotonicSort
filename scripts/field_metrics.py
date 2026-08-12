#!/usr/bin/env python3
"""A0 locked metric helper: geo mean + bootstrap CI from harness CSV.

CSV columns:
  arch,type,pattern,n,reps,menu_ms,pdq_ms,ska_ms,std_ms,best_ms,ratio_best,ok

Usage:
  python3 scripts/field_metrics.py results.csv
  python3 scripts/field_metrics.py results.csv --bootstrap 1000
"""
from __future__ import annotations

import argparse
import csv
import math
import random
import sys
from collections import defaultdict


def geo_mean(xs: list[float]) -> float:
    if not xs:
        return float("nan")
    return math.exp(sum(math.log(x) for x in xs) / len(xs))


def bootstrap_geo(xs: list[float], b: int, seed: int = 42) -> tuple[float, float, float]:
    rng = random.Random(seed)
    n = len(xs)
    samples = []
    for _ in range(b):
        draw = [xs[rng.randrange(n)] for _ in range(n)]
        samples.append(geo_mean(draw))
    samples.sort()
    lo = samples[max(0, int(0.025 * b))]
    hi = samples[min(b - 1, int(0.975 * b))]
    return geo_mean(xs), lo, hi


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("csv_path")
    ap.add_argument("--bootstrap", type=int, default=1000)
    ap.add_argument("--soft", type=float, default=1.15)
    args = ap.parse_args()

    rows = []
    with open(args.csv_path, newline="") as f:
        for r in csv.DictReader(f):
            if r.get("ok") not in ("1", "true", "True"):
                print("CORRECTNESS FAIL:", r, file=sys.stderr)
                return 1
            rows.append(r)

    by_key: dict[tuple, list[float]] = defaultdict(list)
    for r in rows:
        key = (r["arch"], r["type"], r["n"])
        by_key[key].append(float(r["ratio_best"]))

    print("# field_metrics (A0 locked)")
    print(f"# soft_threshold={args.soft} bootstrap={args.bootstrap}")
    for key, ratios in sorted(by_key.items()):
        g, lo, hi = bootstrap_geo(ratios, args.bootstrap)
        soft = sum(1 for x in ratios if x > args.soft)
        mx = max(ratios)
        print(
            f"arch={key[0]} type={key[1]} n={key[2]} "
            f"geo={g:.4f} ci95=[{lo:.4f},{hi:.4f}] max={mx:.4f} soft={soft} patterns={len(ratios)}"
        )
        if g > 0.95 and lo <= 1.0 <= hi:
            print("  KILL_HINT: geo>0.95 and CI overlaps 1.0 — specialized claim freeze")
        if soft:
            print(f"  SOFT: {soft} patterns > {args.soft}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
