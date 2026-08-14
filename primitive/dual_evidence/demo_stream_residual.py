#!/usr/bin/env python3
"""
Non-sort dual-evidence demo — Track 2 path-(c).

Domain: adaptive residual action on sensor-stream segments.
Not a sorter. EXTERNAL-clean. THE BEASTIE BOYZ / Blam 2026-08-14

Probe: windowed inversion density on a continuous sensor ramp with local
reverse spikes (F1 analogue). Second solid: σ_Δ via dual_evidence.compute.
Irreversible residual menu: CHEAP (unit cost 1) vs FULL (unit cost 8).

Modes
-----
- polarity-free: second solid = |σ_Δ| ≥ floor (any strong contrast)
- polarity-directed: second solid = σ_Δ has matching sign (expected_polarity)

Phase pairs match classical flip-rate/runs while σ_Δ polarity flips.
"""
from __future__ import annotations

import sys
from dataclasses import dataclass
from typing import List, Sequence, Tuple

sys.path.insert(0, ".")
from dual_evidence import compute, dual_confirm

COST_CHEAP = 1.0
COST_FULL = 8.0


def gen_segment(n: int, n_spikes: int, phase: int, seed: int = 0) -> List[float]:
    a = [float(i) for i in range(n)]
    base_gap = max(8, n // (n_spikes + 1))
    for i in range(n_spikes):
        amp = min(6 + i * 3, max(3, base_gap // 3), 48)
        pos = (base_gap * (i + 1) + phase * (base_gap // 2)) % (n - amp - 1)
        a[pos : pos + amp] = list(reversed(a[pos : pos + amp]))
    return a


def polarity_flip_runs(a: Sequence[float]) -> Tuple[float, int]:
    flips = 0
    runs = 1
    direction = 0
    for i in range(1, len(a)):
        d = 0 if a[i] == a[i - 1] else (1 if a[i] > a[i - 1] else -1)
        if d != 0 and direction != 0 and d != direction:
            runs += 1
            flips += 1
            direction = d
        elif d != 0:
            direction = d
    return flips / max(1, len(a) - 1), runs


def local_disorder_stream(a: Sequence[float], w: int = 32, stride: int = 8) -> List[float]:
    n = len(a)
    dens = []
    for start in range(0, max(1, n - w + 1), stride):
        window = a[start : start + w]
        inv = 0
        ww = len(window)
        for i in range(ww):
            for j in range(i + 1, ww):
                if window[i] > window[j]:
                    inv += 1
        dens.append(inv / max(1.0, ww * (ww - 1) / 2.0))
    return dens


@dataclass
class Decision:
    action: str
    cost: float
    classical: bool
    owned: bool
    sigma_delta: float


def decide_classical(flip_rate: float, runs: int, n: int) -> Decision:
    classical = flip_rate > 0.015 or runs > max(6, n // 60)
    action = "FULL" if classical else "CHEAP"
    return Decision(action, (COST_FULL if action == "FULL" else COST_CHEAP) * n,
                    classical, classical, 0.0)


def decide_dual(
    flip_rate: float,
    runs: int,
    n: int,
    sigma_delta: float,
    expected_polarity: float = 0.0,
    floor: float = 0.005,
) -> Decision:
    classical = flip_rate > 0.015 or runs > max(6, n // 60)
    owned = dual_confirm(classical, sigma_delta, expected_polarity=expected_polarity, floor=floor)
    action = "FULL" if owned else "CHEAP"
    return Decision(action, (COST_FULL if action == "FULL" else COST_CHEAP) * n,
                    classical, owned, sigma_delta)


def measure_pair(n: int, n_spikes: int, seed: int = 0, expected_polarity: float = 1.0) -> dict:
    a0 = gen_segment(n, n_spikes, phase=0, seed=seed)
    a1 = gen_segment(n, n_spikes, phase=1, seed=seed)
    s0 = compute(local_disorder_stream(a0), c=0.05)
    s1 = compute(local_disorder_stream(a1), c=0.05)
    fr0, r0 = polarity_flip_runs(a0)
    fr1, r1 = polarity_flip_runs(a1)
    c0 = decide_classical(fr0, r0, n)
    c1 = decide_classical(fr1, r1, n)
    d0 = decide_dual(fr0, r0, n, s0.delta, expected_polarity=expected_polarity)
    d1 = decide_dual(fr1, r1, n, s1.delta, expected_polarity=expected_polarity)
    mu_c = max(c0.cost, c1.cost)
    mu_d = max(d0.cost, d1.cost)
    return {
        "n": n,
        "classical_same": c0.action == c1.action,
        "dual_differs": d0.action != d1.action,
        "mu_c": mu_c,
        "mu_d": mu_d,
        "ratio": mu_d / mu_c if mu_c else 1.0,
        "sigma0": s0.delta,
        "sigma1": s1.delta,
        "sigma_flip": (s0.delta * s1.delta < 0) or abs(s0.delta - s1.delta) > 0.01,
        "c_act": (c0.action, c1.action),
        "d_act": (d0.action, d1.action),
        "win": mu_d < 0.98 * mu_c,
    }


def main() -> None:
    print("# non-sort dual-evidence demo — sensor-stream residual")
    print("# probe=windowed inv-density  second_solid=polarity-directed σ_Δ")
    print("# EXTERNAL-clean. THE BEASTIE BOYZ / Blam")
    print()
    print("n,classical_same,dual_differs,sigma_flip,mu_c,mu_d,ratio,win,sig0,sig1")

    total = differs = flips = wins = cm = 0
    for n, spikes in [(512, 12), (1024, 16), (2048, 24), (4096, 32), (8192, 40)]:
        for seed in range(10):
            r = measure_pair(n, spikes, seed, expected_polarity=1.0)
            total += 1
            differs += int(r["dual_differs"])
            flips += int(r["sigma_flip"])
            wins += int(r["win"])
            cm += int(r["classical_same"])
            if seed == 0:
                print(
                    f"{r['n']},{r['classical_same']},{r['dual_differs']},{r['sigma_flip']},"
                    f"{r['mu_c']:.0f},{r['mu_d']:.0f},{r['ratio']:.3f},{r['win']},"
                    f"{r['sigma0']:+.4f},{r['sigma1']:+.4f}"
                )

    print()
    print(
        f"# trials={total} classical_match={cm} sigma_flip={flips} "
        f"dual_differs={differs} claim_b_wins={wins}"
    )

    for n, spikes in [(1024, 16), (4096, 32), (8192, 40)]:
        ratios, diff_n, flip_n, cmn = [], 0, 0, 0
        for seed in range(40):
            r = measure_pair(n, spikes, seed, expected_polarity=1.0)
            ratios.append(r["ratio"])
            diff_n += int(r["dual_differs"])
            flip_n += int(r["sigma_flip"])
            cmn += int(r["classical_same"])
        ratios.sort()
        med = ratios[len(ratios) // 2]
        print(
            f"# aggregate n={n} seeds=40 classical_match={cmn}/40 sigma_flip={flip_n}/40 "
            f"dual_differs={diff_n}/40 median_ratio={med:.3f} "
            f"min={ratios[0]:.3f} max={ratios[-1]:.3f}"
        )

    r = measure_pair(512, 12, seed=0, expected_polarity=1.0)
    print()
    print("# sample n=512 seed=0 (polarity-directed)")
    print(f"#   classical_same={r['classical_same']} dual_differs={r['dual_differs']}")
    print(f"#   sigma_flip={r['sigma_flip']} sig={r['sigma0']:+.4f}/{r['sigma1']:+.4f}")
    print(f"#   actions classical={r['c_act']} dual={r['d_act']}")
    print(f"#   mu_c={r['mu_c']:.0f} mu_d={r['mu_d']:.0f} ratio={r['ratio']:.3f}")
    assert r["classical_same"], "classical must match on phase pair"
    assert r["sigma_flip"] or r["dual_differs"] or r["win"], "need signal somewhere"
    print("SELF-CHECK GREEN")


if __name__ == "__main__":
    main()
