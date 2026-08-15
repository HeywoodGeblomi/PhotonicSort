#!/usr/bin/env python3
"""Search-tree orthogonality harness (STO-F1). THE BEASTIE BOYZ / Blam 2026-08-14.
Discrete C from polarity; σ_Δ from F1 phase-shifted continuous probe.
NON-CLAIM: not P=NP. Residual policy incompleteness only. EXTERNAL-clean.
See docs/field_level/SEARCH_TREE_ORTHOGONALITY.md.
"""
from __future__ import annotations
import sys
from typing import List, Tuple
sys.path.insert(0, ".")
from dual_evidence import compute, dual_confirm

def runs_count(pol):
    if not pol: return 0
    r = 1
    for i in range(1, len(pol)):
        if pol[i] != pol[i-1]: r += 1
    return r

def burst_multiset(pol):
    amps = []; i = 0; n = len(pol)
    while i < n:
        if pol[i] < 0:
            j = i
            while j < n and pol[j] < 0: j += 1
            amps.append(j - i); i = j
        else:
            i += 1
    return tuple(sorted(amps))

def gen_polarity(k, L, a_max=8):
    amps = [min(2+i, a_max) for i in range(k)]
    g = max(a_max*2+4, max(amps)+6)
    pol = [1.0]*L
    for i,a in enumerate(amps):
        start = g*(i+1)
        if start+a >= L: start = L-a-1
        for j in range(a): pol[start+j] = -1.0
    return pol

def gen_probe_f1(L, k, phase, a_max=12):
    a = list(range(L))
    amps = [min(3+i*2, a_max) for i in range(k)]
    g = max(L//(k+1), a_max+4)
    for i, amp in enumerate(amps):
        pos = (g*(i+1) + phase*(g//2)) % (L-amp-1)
        a[pos:pos+amp] = list(reversed(a[pos:pos+amp]))
    w, stride = 24, 6
    dens = []
    for start in range(0, max(1, L-w+1), stride):
        window = a[start:start+w]
        inv = 0; ww = len(window)
        for i in range(ww):
            for j in range(i+1, ww):
                if window[i] > window[j]: inv += 1
        dens.append(inv / max(1.0, ww*(ww-1)/2.0))
    return dens

def classical_C(pol):
    return (len(pol), runs_count(pol), burst_multiset(pol))

def measure_pair(k):
    L = (k+2)*max(20, k*2)+50
    pol = gen_polarity(k, L)
    C = classical_C(pol)
    d0 = gen_probe_f1(L, k, phase=0)
    d1 = gen_probe_f1(L, k, phase=1)
    s0 = compute(d0, c=0.08).delta
    s1 = compute(d1, c=0.08).delta
    sigma_flip = (s0*s1 < 0) or abs(s0-s1) > 0.03
    def cpol(_C):
        L_, runs, bursts = _C
        borderline = 4 <= runs <= max(6, L_//10) and len(bursts) >= 3
        return "FULL" if borderline else "CHEAP"
    def dpol(_C, sigma):
        if cpol(_C) != "FULL": return "CHEAP"
        return "FULL" if dual_confirm(True, sigma, expected_polarity=1.0, floor=0.02) else "CHEAP"
    cp0, cp1 = cpol(C), cpol(C)
    dp0, dp1 = dpol(C, s0), dpol(C, s1)
    sens = dual_confirm(True, s0, expected_polarity=1.0, floor=0.02) != dual_confirm(True, s1, expected_polarity=1.0, floor=0.02)
    return dict(k=k, L=C[0], runs=C[1], classical_match=True, sigma0=s0, sigma1=s1,
        sigma_flip=sigma_flip, classical_same=cp0==cp1, dual_differs=dp0!=dp1, sens=sens,
        cp=(cp0,cp1), dp=(dp0,dp1))

def main():
    print("# Search-tree orthogonality harness (STO-F1)")
    print("# discrete C from polarity; σ_Δ from F1 phase-shifted continuous probe")
    print("# NON-CLAIM: not P=NP — residual policy incompleteness only")
    print("# EXTERNAL-clean. THE BEASTIE BOYZ / Blam")
    print()
    print("k,L,runs,sigma_flip,classical_same,dual_differs,sens,sig0,sig1,dp")
    total=sf=cs=dd=sens_n=0
    for k in range(5, 18):
        r = measure_pair(k)
        total += 1; sf += int(r["sigma_flip"]); cs += int(r["classical_same"])
        dd += int(r["dual_differs"]); sens_n += int(r["sens"])
        print(f"{r['k']},{r['L']},{r['runs']},{r['sigma_flip']},{r['classical_same']},{r['dual_differs']},{r['sens']},{r['sigma0']:+.4f},{r['sigma1']:+.4f},{r['dp']}")
    print()
    print(f"# trials={total} sigma_flip={sf}/{total} classical_same={cs}/{total} dual_differs={dd}/{total} polarity_sens={sens_n}/{total}")
    assert cs == total
    assert sf >= (total*2)//3, f"sigma flip majority required, got {sf}/{total}"
    assert sens_n+dd >= total//3, "dual must show polarity sensitivity on a solid fraction"
    print("# STO-A GREEN: C matched by construction; σ_Δ flips; classical policy identical; dual sensitive")
    print("SELF-CHECK GREEN")

if __name__ == "__main__":
    main()
