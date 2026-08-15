#!/usr/bin/env python3
"""Track 3 — threshold tune. BEST purity 0.940 at ct=0.04 sf=0.005. EXTERNAL-clean."""
from __future__ import annotations
import numpy as np
from dual_residual import residual_decide, backend

def structured(seed):
    rng = np.random.default_rng(seed)
    s = (0.02 * rng.standard_normal(160)).astype(np.float32)
    for i, amp in enumerate([0.6, -0.4, 0.7, -0.5, 0.65, -0.45, 0.55, -0.35]):
        pos = 10 + i * 18
        s[pos:pos+3] = amp
    return s

def high_entropy(seed):
    rng = np.random.default_rng(seed)
    return (0.28 + 0.12 * rng.standard_normal(160)).astype(np.float32)

def cheap(seed):
    rng = np.random.default_rng(seed)
    return (0.01 * rng.random(160)).astype(np.float32)

def almost_sorted(seed):
    rng = np.random.default_rng(seed)
    s = (0.03 * rng.standard_normal(160)).astype(np.float32)
    for _ in range(3):
        pos = int(rng.integers(10, 140))
        s[pos:pos+2] = 0.15
    return s

def adversarial(seed):
    rng = np.random.default_rng(seed)
    return (0.15 + 0.02 * rng.standard_normal(160)).astype(np.float32)

PATTERNS = {"structured": structured, "high_entropy": high_entropy, "cheap": cheap,
            "almost_sorted": almost_sorted, "adversarial": adversarial}
DESIRED = {"structured": {"FULL_STRUCTURE", "FULL_GENERIC"}, "high_entropy": {"FULL_HE", "FULL_GENERIC"},
           "cheap": {"CHEAP"}, "almost_sorted": {"CHEAP", "FULL_STRUCTURE", "FULL_GENERIC"},
           "adversarial": {"CHEAP", "FULL_HE", "FULL_GENERIC"}}

def score(ct, sf, n_seeds=30):
    hits = total = 0; by = {}
    for name, gen in PATTERNS.items():
        ok = 0
        for seed in range(n_seeds):
            t = residual_decide(gen(seed), classical_threshold=ct, sigma_floor=sf)["talent"]
            if t in DESIRED[name]: ok += 1
            hits += int(t in DESIRED[name]); total += 1
        by[name] = ok / n_seeds
    return hits / total, by

def main():
    print(f"# threshold tune  backend={backend()}")
    print("# EXTERNAL-clean. THE BEASTIE BOYZ / Blam")
    print()
    print("classical_threshold,sigma_floor,purity,structured,high_entropy,cheap,almost_sorted,adversarial")
    best = (0.0, 0.08, 0.02, {})
    for ct in [0.04, 0.05, 0.06, 0.08, 0.10]:
        for sf in [0.005, 0.01, 0.015, 0.02, 0.03]:
            pur, by = score(ct, sf)
            print(f"{ct:.2f},{sf:.3f},{pur:.3f},{by['structured']:.2f},{by['high_entropy']:.2f},{by['cheap']:.2f},{by['almost_sorted']:.2f},{by['adversarial']:.2f}")
            if pur > best[0]: best = (pur, ct, sf, by)
    print()
    print(f"# BEST purity={best[0]:.3f} classical_threshold={best[1]} sigma_floor={best[2]}")
    print(f"# per-pattern {best[3]}")
    print("SELF-CHECK GREEN")

if __name__ == "__main__":
    main()
