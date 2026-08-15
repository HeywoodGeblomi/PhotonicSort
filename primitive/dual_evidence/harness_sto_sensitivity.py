#!/usr/bin/env python3
"""STO sensitivity pressure — dual_differs + residual minimax under polarity cost.
THE BEASTIE BOYZ / Blam. Sensitivity is the weapon. NOT P=NP. EXTERNAL-clean.
See RESULT_STO_SENSITIVITY.md.
"""
from __future__ import annotations
import sys
from typing import List, Tuple
sys.path.insert(0, ".")
from dual_evidence import compute, dual_confirm
COST_CHEAP, COST_FULL_CORRECT, COST_FULL_WRONG = 1.0, 2.0, 12.0

def runs_count(pol):
    if not pol: return 0
    r = 1
    for i in range(1, len(pol)):
        if pol[i] != pol[i-1]: r += 1
    return r

def burst_multiset(pol):
    amps, i, n = [], 0, len(pol)
    while i < n:
        if pol[i] < 0:
            j = i
            while j < n and pol[j] < 0: j += 1
            amps.append(j-i); i = j
        else: i += 1
    return tuple(sorted(amps))

def gen_polarity(k, L):
    amps = [min(2+i, 10) for i in range(k)]
    g = max(24, max(amps)+8)
    pol = [1.0]*L
    for i,a in enumerate(amps):
        start = min(g*(i+1), L-a-1)
        for j in range(a): pol[start+j] = -1.0
    return pol

def gen_probe_f1(L, k, phase):
    a = list(range(L))
    amps = [5 + i*4 for i in range(k)]
    g = max(L//(k+1), max(amps)+10)
    for i, amp in enumerate(amps):
        amp = min(amp, L//6)
        pos = (g*(i+1) + phase*(g//2)) % max(1, L-amp-1)
        a[pos:pos+amp] = list(reversed(a[pos:pos+amp]))
    dens = []
    w, stride = 16, 3
    for start in range(0, max(1, L-w+1), stride):
        window = a[start:start+w]
        inv = sum(1 for i in range(len(window)) for j in range(i+1,len(window)) if window[i]>window[j])
        dens.append(inv / max(1.0, len(window)*(len(window)-1)/2.0))
    return dens

def classical_C(pol):
    return (len(pol), runs_count(pol), burst_multiset(pol))

def is_borderline(C):
    L, runs, bursts = C
    return runs >= 4 and len(bursts) >= 3

def classical_policy(C):
    return "FULL" if is_borderline(C) else "CHEAP"

def dual_policy(C, sigma, floor=0.015):
    if not is_borderline(C): return "CHEAP"
    return "FULL" if dual_confirm(True, sigma, expected_polarity=1.0, floor=floor) else "CHEAP"

def residual_cost(action, sigma, true_polarity=1.0):
    if action == "CHEAP": return COST_CHEAP
    return COST_FULL_CORRECT if sigma*true_polarity > 0 else COST_FULL_WRONG

def measure_pair(k):
    L = (k+2)*max(22,k*2)+60
    pol = gen_polarity(k, L)
    C = classical_C(pol)
    s0 = compute(gen_probe_f1(L,k,0), c=0.05).delta
    s1 = compute(gen_probe_f1(L,k,1), c=0.05).delta
    sigma_flip = (s0*s1 < 0) or abs(s0-s1) > 0.025
    cp0, cp1 = classical_policy(C), classical_policy(C)
    dp0, dp1 = dual_policy(C, s0), dual_policy(C, s1)
    dual_differs = dp0 != dp1
    sens = dual_confirm(True, s0, expected_polarity=1.0, floor=0.015) != dual_confirm(True, s1, expected_polarity=1.0, floor=0.015)
    true_pol = 1.0
    mu_c = max(residual_cost(cp0,s0,true_pol), residual_cost(cp1,s1,true_pol))
    mu_d = max(residual_cost(dp0,s0,true_pol), residual_cost(dp1,s1,true_pol))
    return dict(k=k, L=C[0], runs=C[1], sigma0=s0, sigma1=s1, sigma_flip=sigma_flip,
        classical_same=cp0==cp1, dual_differs=dual_differs, sens=sens,
        cp=(cp0,cp1), dp=(dp0,dp1), mu_c=mu_c, mu_d=mu_d,
        ratio=(mu_d/mu_c if mu_c else 1.0), win=mu_d<0.98*mu_c)

def main():
    print("# STO sensitivity pressure — dual_differs + residual minimax")
    print("# Sensitivity is the weapon. NON-CLAIM: not P=NP")
    print("# EXTERNAL-clean. THE BEASTIE BOYZ / Blam")
    print()
    print("k,L,sigma_flip,dual_differs,sens,mu_c,mu_d,ratio,win,sig0,sig1,dp")
    total=sf=dd=sens_n=wins=0; ratios=[]
    for k in range(5, 22):
        r = measure_pair(k)
        total += 1; sf += int(r["sigma_flip"]); dd += int(r["dual_differs"])
        sens_n += int(r["sens"]); wins += int(r["win"]); ratios.append(r["ratio"])
        print(f"{r['k']},{r['L']},{r['sigma_flip']},{r['dual_differs']},{r['sens']},{r['mu_c']:.1f},{r['mu_d']:.1f},{r['ratio']:.3f},{r['win']},{r['sigma0']:+.4f},{r['sigma1']:+.4f},{r['dp']}")
    ratios.sort(); med = ratios[len(ratios)//2]
    print()
    print(f"# trials={total} sigma_flip={sf}/{total} dual_differs={dd}/{total} sens={sens_n}/{total} minimax_wins={wins}/{total} median_ratio={med:.3f} min={ratios[0]:.3f} max={ratios[-1]:.3f}")
    assert sf >= total//2, "sigma flip majority"
    assert dd+sens_n >= total//2, "sensitivity must show on ≥ half"
    print("# SENSITIVITY PRESSURE GREEN")
    print("SELF-CHECK GREEN")

if __name__ == "__main__":
    main()
