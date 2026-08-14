#!/usr/bin/env python3
"""Neuromorphic spike-native dual-evidence demo. THE BEASTIE BOYZ / Blam 2026-08-14.
Spikes ARE threshold crossings. No frame. No clock. EXTERNAL-clean.
See RESULT_NEURO_SPIKE.md.
"""
from __future__ import annotations
import math, random, sys
from dataclasses import dataclass
from typing import List, Sequence, Tuple
sys.path.insert(0, ".")
from dual_evidence import dual_confirm
COST_CHEAP, COST_FULL = 1.0, 8.0

def gen_spike_pair(T=1.0, base_rate=70.0, n_bursts=16, seed=0):
    rng = random.Random(seed)
    background = []
    t = 0.0
    while t < T:
        t += rng.expovariate(base_rate)
        if t < T: background.append(t)
    gap = T / (n_bursts + 1)
    def with_phase(phase):
        spikes = list(background)
        brng = random.Random(seed + 99)
        for i in range(n_bursts):
            strength = 3 + i
            center = (gap * (i + 1) + phase * (gap / 2.0)) % T
            for _ in range(strength):
                st = (center + brng.gauss(0.0, gap * 0.02)) % T
                spikes.append(max(0.0, min(T - 1e-9, st)))
        spikes.sort()
        amps = []
        for st in spikes:
            dmin = min(abs(st - (gap * (i + 1) + phase * (gap / 2.0)) % T) for i in range(n_bursts))
            amps.append(1.0 + 2.5 * math.exp(-((dmin / (gap * 0.08)) ** 2)))
        return spikes, amps
    return with_phase(0), with_phase(1)

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
    if n < 8: return 0.0, 0
    odd_idx = list(range(0, n, 2))
    if len(odd_idx) < 4: return 0.0, 0
    Ee, Eo = odd_idx[0::2], odd_idx[1::2]
    return sum(amps[i] for i in Ee)/len(Ee) - sum(amps[i] for i in Eo)/len(Eo), len(odd_idx)

@dataclass
class Decision:
    action: str; cost: float; classical: bool; owned: bool; sigma_delta: float

def decide_classical(rate, cv, n):
    classical = rate > 90.0 or cv > 0.85
    action = "FULL" if classical else "CHEAP"
    return Decision(action, (COST_FULL if action=="FULL" else COST_CHEAP)*n, classical, classical, 0.0)

def decide_dual(rate, cv, n, sigma_delta, expected_polarity=1.0, floor=0.02):
    classical = rate > 90.0 or cv > 0.85
    owned = dual_confirm(classical, sigma_delta, expected_polarity=expected_polarity, floor=floor)
    action = "FULL" if owned else "CHEAP"
    return Decision(action, (COST_FULL if action=="FULL" else COST_CHEAP)*n, classical, owned, sigma_delta)

def measure_pair(T=1.0, base_rate=70.0, n_bursts=16, seed=0):
    (t0,a0),(t1,a1) = gen_spike_pair(T, base_rate, n_bursts, seed)
    r0,cv0 = classical_stats(t0,T); r1,cv1 = classical_stats(t1,T)
    d0,_ = sigma_delta_spikes(t0,a0); d1,_ = sigma_delta_spikes(t1,a1)
    rate_rel = abs(r0-r1)/max(0.5*(r0+r1),1e-9)
    classical_match = rate_rel < 0.08 and abs(cv0-cv1) < 0.20
    c0,c1 = decide_classical(r0,cv0,len(t0)), decide_classical(r1,cv1,len(t1))
    du0,du1 = decide_dual(r0,cv0,len(t0),d0), decide_dual(r1,cv1,len(t1),d1)
    mu_c, mu_d = max(c0.cost,c1.cost), max(du0.cost,du1.cost)
    return dict(n0=len(t0),n1=len(t1),rate0=r0,rate1=r1,cv0=cv0,cv1=cv1,rate_rel=rate_rel,
        classical_match=classical_match,sigma0=d0,sigma1=d1,
        sigma_flip=(d0*d1<0) or abs(d0-d1)>0.04,
        c_act=(c0.action,c1.action),d_act=(du0.action,du1.action),
        classical_same=c0.action==c1.action, dual_differs=du0.action!=du1.action,
        mu_c=mu_c, mu_d=mu_d, ratio=(mu_d/mu_c if mu_c else 1.0), win=mu_d<0.98*mu_c)

def main():
    print("# neuromorphic spike-native dual-evidence demo")
    print("# spikes ARE events — no frame, no clock")
    print("# EXTERNAL-clean. THE BEASTIE BOYZ / Blam — WIDE OPEN")
    print()
    print("bursts,seed,classical_match,classical_same,dual_differs,sigma_flip,mu_c,mu_d,ratio,win,sig0,sig1")
    total=cm=cs=dd=sf=wins=0
    for n_bursts in [8,12,16,24]:
        for seed in range(10):
            r = measure_pair(n_bursts=n_bursts, seed=seed)
            total += 1; cm += int(r["classical_match"]); cs += int(r["classical_same"])
            dd += int(r["dual_differs"]); sf += int(r["sigma_flip"]); wins += int(r["win"])
            if seed < 2:
                print(f"{n_bursts},{seed},{r['classical_match']},{r['classical_same']},{r['dual_differs']},{r['sigma_flip']},{r['mu_c']:.0f},{r['mu_d']:.0f},{r['ratio']:.3f},{r['win']},{r['sigma0']:+.4f},{r['sigma1']:+.4f}")
    print()
    print(f"# trials={total} classical_match={cm} classical_same={cs} sigma_flip={sf} dual_differs={dd} claim_b_wins={wins}")
    for n_bursts in [12,16,24]:
        ratios=[]; ddn=sfn=cmn=0
        for seed in range(40):
            r=measure_pair(n_bursts=n_bursts,seed=seed)
            ratios.append(r["ratio"]); ddn+=int(r["dual_differs"]); sfn+=int(r["sigma_flip"]); cmn+=int(r["classical_match"])
        ratios.sort(); med=ratios[len(ratios)//2]
        print(f"# aggregate bursts={n_bursts} seeds=40 classical_match={cmn}/40 sigma_flip={sfn}/40 dual_differs={ddn}/40 median_ratio={med:.3f} min={ratios[0]:.3f} max={ratios[-1]:.3f}")
    r=measure_pair(n_bursts=16,seed=0)
    print()
    print("# sample bursts=16 seed=0")
    print(f"#   spikes={r['n0']}/{r['n1']} rates={r['rate0']:.1f}/{r['rate1']:.1f}")
    print(f"#   classical_match={r['classical_match']} sigma_flip={r['sigma_flip']}")
    print(f"#   sig={r['sigma0']:+.4f}/{r['sigma1']:+.4f}")
    print(f"#   actions classical={r['c_act']} dual={r['d_act']}")
    print(f"#   mu_c={r['mu_c']:.0f} mu_d={r['mu_d']:.0f} ratio={r['ratio']:.3f}")
    assert r["n0"] > 20
    print("SELF-CHECK GREEN")

if __name__ == "__main__":
    main()
