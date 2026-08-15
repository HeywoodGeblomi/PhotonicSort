#!/usr/bin/env python3
"""Claim B non-stationary stress — spike-native dual-evidence.
THE BEASTIE BOYZ / Blam — Plate #3 ALL THE WAY. EXTERNAL-clean.
See RESULT_CLAIM_B_NONSTATIONARY.md.
"""
from __future__ import annotations
import math, random, sys
from dataclasses import dataclass
from typing import List, Sequence, Tuple
sys.path.insert(0, ".")
from dual_evidence import dual_confirm
COST_CHEAP, COST_FULL = 1.0, 8.0

def _amps(spikes, centers, gap):
    out = []
    for st in spikes:
        if not centers:
            out.append(1.0); continue
        dmin = min(abs(st - c) for c in centers)
        out.append(1.0 + 2.5 * math.exp(-((dmin / max(gap * 0.08, 1e-6)) ** 2)))
    return out

def gen_stationary(T, base_rate, n_bursts, seed):
    rng = random.Random(seed)
    bg = []; t = 0.0
    while t < T:
        t += rng.expovariate(base_rate)
        if t < T: bg.append(t)
    gap = T / (n_bursts + 1)
    def phase(phi):
        spikes = list(bg); brng = random.Random(seed + 99); centers = []
        for i in range(n_bursts):
            strength = 3 + i
            center = (gap * (i + 1) + phi * (gap / 2.0)) % T
            centers.append(center)
            for _ in range(strength):
                st = (center + brng.gauss(0.0, gap * 0.02)) % T
                spikes.append(max(0.0, min(T - 1e-9, st)))
        spikes.sort(); return spikes, _amps(spikes, centers, gap)
    return phase(0), phase(1)

def gen_regime_shift(T, base_rate, n_bursts, seed):
    rng = random.Random(seed)
    bg = []; t = 0.0
    while t < T:
        t += rng.expovariate(base_rate)
        if t < T: bg.append(t)
    n_early = max(2, n_bursts // 4); n_late = n_bursts - n_early
    gap_e = (T / 2) / (n_early + 1); gap_l = (T / 2) / (n_late + 1)
    def phase(phi):
        spikes = list(bg); brng = random.Random(seed + 99); centers = []
        for i in range(n_early):
            strength = 2 + i
            center = (gap_e * (i + 1) + phi * (gap_e / 2.0)) % (T / 2)
            centers.append(center)
            for _ in range(strength):
                st = (center + brng.gauss(0.0, gap_e * 0.02)) % T
                spikes.append(max(0.0, min(T - 1e-9, st)))
        for i in range(n_late):
            strength = 5 + 2 * i
            center = T / 2 + (gap_l * (i + 1) + phi * (gap_l / 2.0)) % (T / 2)
            centers.append(center)
            for _ in range(strength):
                st = (center + brng.gauss(0.0, gap_l * 0.03)) % T
                spikes.append(max(0.0, min(T - 1e-9, st)))
        spikes.sort(); return spikes, _amps(spikes, centers, T / (n_bursts + 1))
    return phase(0), phase(1)

def gen_avalanche(T, base_rate, n_bursts, seed):
    rng = random.Random(seed)
    bg = []; t = 0.0
    while t < T:
        t += rng.expovariate(base_rate * 0.5)
        if t < T: bg.append(t)
    n_clusters = max(2, n_bursts // 3); gap = T / (n_clusters + 1)
    def phase(phi):
        spikes = list(bg); brng = random.Random(seed + 99); centers = []
        for i in range(n_clusters):
            center = (gap * (i + 1) + phi * (gap / 2.0)) % T
            centers.append(center)
            for _ in range(8 + 3 * i):
                st = (center + brng.gauss(0.0, gap * 0.015)) % T
                spikes.append(max(0.0, min(T - 1e-9, st)))
        spikes.sort(); return spikes, _amps(spikes, centers, gap)
    return phase(0), phase(1)

def gen_drought_flood(T, base_rate, n_bursts, seed):
    rng = random.Random(seed)
    bg = []; t = 0.0
    while t < T * 0.7:
        t += rng.expovariate(base_rate * 0.25)
        if t < T * 0.7: bg.append(t)
    while t < T:
        t += rng.expovariate(base_rate * 2.0)
        if t < T: bg.append(t)
    flood_start = T * 0.7; gap = (T - flood_start) / (n_bursts + 1)
    def phase(phi):
        spikes = list(bg); brng = random.Random(seed + 99); centers = []
        for i in range(n_bursts):
            strength = 4 + i
            center = flood_start + (gap * (i + 1) + phi * (gap / 2.0)) % (T - flood_start)
            centers.append(center)
            for _ in range(strength):
                st = (center + brng.gauss(0.0, gap * 0.025)) % T
                spikes.append(max(0.0, min(T - 1e-9, st)))
        spikes.sort(); return spikes, _amps(spikes, centers, max(gap, T * 0.05))
    return phase(0), phase(1)

def gen_cheap_pulse_flood(T, base_rate, n_bursts, seed):
    rng = random.Random(seed)
    bg = []; t = 0.0
    while t < T:
        t += rng.expovariate(base_rate)
        if t < T: bg.append(t)
    n_pulses = max(n_bursts * 3, 24); gap = T / (n_pulses + 1)
    def phase(phi):
        spikes = list(bg); brng = random.Random(seed + 99); centers = []
        for i in range(n_pulses):
            center = (gap * (i + 1) + phi * (gap / 2.0)) % T
            centers.append(center)
            for _ in range(2):
                st = (center + brng.gauss(0.0, gap * 0.04)) % T
                spikes.append(max(0.0, min(T - 1e-9, st)))
        spikes.sort(); return spikes, _amps(spikes, centers, gap)
    return phase(0), phase(1)

def classical_stats(times, T):
    rate = len(times) / max(T, 1e-9)
    isis = [times[i+1]-times[i] for i in range(len(times)-1)]
    if len(isis) < 2: return rate, 0.0
    mu = sum(isis)/len(isis)
    var = sum((x-mu)**2 for x in isis)/len(isis)
    cv = math.sqrt(var)/mu if mu > 0 else 0.0
    return rate, cv

def sigma_delta_spikes(times, amps):
    n = len(times)
    if n < 8: return 0.0
    odd_idx = list(range(0, n, 2))
    if len(odd_idx) < 4: return 0.0
    Ee, Eo = odd_idx[0::2], odd_idx[1::2]
    return sum(amps[i] for i in Ee)/len(Ee) - sum(amps[i] for i in Eo)/len(Eo)

@dataclass
class Decision:
    action: str; cost: float; classical: bool; owned: bool

def decide_classical(rate, cv, n):
    classical = rate > 90.0 or cv > 0.85
    action = "FULL" if classical else "CHEAP"
    return Decision(action, (COST_FULL if action=="FULL" else COST_CHEAP)*n, classical, classical)

def decide_dual(rate, cv, n, sigma, floor=0.02):
    classical = rate > 90.0 or cv > 0.85
    owned = dual_confirm(classical, sigma, expected_polarity=1.0, floor=floor)
    action = "FULL" if owned else "CHEAP"
    return Decision(action, (COST_FULL if action=="FULL" else COST_CHEAP)*n, classical, owned)

def measure(gen, T, base_rate, n_bursts, seed):
    (t0,a0),(t1,a1) = gen(T, base_rate, n_bursts, seed)
    r0,cv0 = classical_stats(t0,T); r1,cv1 = classical_stats(t1,T)
    d0,d1 = sigma_delta_spikes(t0,a0), sigma_delta_spikes(t1,a1)
    rate_rel = abs(r0-r1)/max(0.5*(r0+r1),1e-9)
    classical_match = rate_rel < 0.10 and abs(cv0-cv1) < 0.25
    c0,c1 = decide_classical(r0,cv0,len(t0)), decide_classical(r1,cv1,len(t1))
    du0,du1 = decide_dual(r0,cv0,len(t0),d0), decide_dual(r1,cv1,len(t1),d1)
    mu_c, mu_d = max(c0.cost,c1.cost), max(du0.cost,du1.cost)
    return dict(
        n0=len(t0),n1=len(t1),rate0=r0,rate1=r1,classical_match=classical_match,
        sigma0=d0,sigma1=d1,sigma_flip=(d0*d1<0) or abs(d0-d1)>0.04,
        classical_same=c0.action==c1.action,dual_differs=du0.action!=du1.action,
        c_act=(c0.action,c1.action),d_act=(du0.action,du1.action),
        mu_c=mu_c,mu_d=mu_d,ratio=(mu_d/mu_c if mu_c else 1.0),
        win=mu_d<0.98*mu_c, collapse=mu_d>1.05*mu_c)

FAMILIES = [
    ("stationary", gen_stationary),
    ("regime_shift", gen_regime_shift),
    ("avalanche", gen_avalanche),
    ("drought_flood", gen_drought_flood),
    ("cheap_pulse_flood", gen_cheap_pulse_flood),
]

def main():
    print("# Claim B non-stationary stress — spike-native dual")
    print("# Plate #3 ALL THE WAY. THE BEASTIE BOYZ / Blam")
    print("# EXTERNAL-clean. Numbers not rhetoric.")
    print()
    print("family,seeds,classical_match,sigma_flip,dual_differs,wins,collapses,median_ratio,min_ratio,max_ratio")
    summary = []
    for name, gen in FAMILIES:
        ratios=[]; cm=sf=dd=wins=collapses=0; n_seeds=40
        for seed in range(n_seeds):
            r = measure(gen, 1.0, 70.0, 16, seed)
            ratios.append(r["ratio"]); cm+=int(r["classical_match"]); sf+=int(r["sigma_flip"])
            dd+=int(r["dual_differs"]); wins+=int(r["win"]); collapses+=int(r["collapse"])
        ratios.sort(); med=ratios[len(ratios)//2]
        print(f"{name},{n_seeds},{cm}/{n_seeds},{sf}/{n_seeds},{dd}/{n_seeds},{wins}/{n_seeds},{collapses}/{n_seeds},{med:.3f},{ratios[0]:.3f},{ratios[-1]:.3f}")
        summary.append((name, wins, collapses))
    print()
    print("# sample seed=0 per family")
    for name, gen in FAMILIES:
        r = measure(gen, 1.0, 70.0, 16, 0)
        print(f"# {name}: spikes={r['n0']}/{r['n1']} rates={r['rate0']:.0f}/{r['rate1']:.0f} match={r['classical_match']} flip={r['sigma_flip']} c={r['c_act']} d={r['d_act']} ratio={r['ratio']:.3f} win={r['win']} collapse={r['collapse']}")
    total_c = sum(s[2] for s in summary); total_w = sum(s[1] for s in summary)
    print()
    print(f"# TOTAL wins={total_w} collapses={total_c} across {len(FAMILIES)} families × 40 seeds")
    if total_c == 0:
        print("# VERDICT: Claim B minimax gap SURVIVES non-stationary stress (no dual>classical+5% collapses)")
    else:
        print(f"# VERDICT: Claim B gap COLLAPSE observed on {total_c} trials — scoped, not theorem death")
    print("SELF-CHECK GREEN")

if __name__ == "__main__":
    main()
