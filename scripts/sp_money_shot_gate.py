#!/usr/bin/env python3
"""SP charged-surface money-shot gate (point estimate).

Reads summary CSV from expanded_field_bench_hybrid:
  arch,type,pattern,n,reps,menu_ms,pdq_ms,ska_ms,std_ms,best_ms,ratio_best,ok

Soft = menu_ms/pdq_ms > threshold on charged patterns.
Exit 0 iff charged soft=0, major=0, ok=1 full suite.

EXTERNAL-clean. THE BEASTIE BOYZ 2026-08-14
"""
from __future__ import annotations
import argparse, csv, sys

CHARGED = {
    "sorted", "reverse", "almost_sorted",
    "few_k4_dense", "few_k16_dense",
    "organpipe", "sawtooth", "push_middle", "runs_noise",
    "random", "gaussianish", "adversarial",
    "db_pk", "timestamps",
    "reverse_segments", "equal_heavy",  # v0.4 expansion (soft-attack closed)
}

def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("csv_path")
    ap.add_argument("--threshold", type=float, default=1.20)
    ap.add_argument("--major", type=float, default=1.50)
    args = ap.parse_args()

    soft, major, fails = [], [], []
    print(f"# sp_money_shot_gate threshold={args.threshold} charged={sorted(CHARGED)}")
    with open(args.csv_path, newline="") as f:
        for r in csv.DictReader(f):
            try:
                ok = int(r["ok"])
                menu = float(r["menu_ms"])
                pdq = float(r["pdq_ms"])
                ratio = menu / max(1e-12, pdq)
                pat = r["pattern"]
                name = f"{r['arch']}/{r['type']}/{pat}"
            except Exception as e:
                print("parse fail", r, e, file=sys.stderr)
                return 1
            if ok != 1:
                fails.append(name)
            if pat in CHARGED:
                flag = ""
                if ratio > args.threshold:
                    soft.append((name, ratio)); flag = " SOFT"
                if ratio > args.major:
                    major.append((name, ratio)); flag += " MAJOR"
                print(f"{name} ratio_pdq={ratio:.4f}{flag}")
    print(f"# charged_soft={len(soft)} major={len(major)} fails={len(fails)}")
    if fails:
        print("CORRECTNESS FAIL", fails, file=sys.stderr); return 1
    if soft or major:
        print("SP MONEY-SHOT FAIL", file=sys.stderr); return 1
    print("SP MONEY-SHOT GREEN charged soft=0 major=0 ok=1")
    return 0

if __name__ == "__main__":
    raise SystemExit(main())
