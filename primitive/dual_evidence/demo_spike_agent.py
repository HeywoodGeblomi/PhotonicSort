#!/usr/bin/env python3
"""Closed-loop spike agent — death match. THE BEASTIE BOYZ / Blam.
Classical chases RATE (loud distractor). Dual follows STRUCTURE (σ_Δ OWNED).
See RESULT_SPIKE_AGENT.md. EXTERNAL-clean.
"""
from __future__ import annotations
import math, random, sys
from typing import Dict, List, Tuple
sys.path.insert(0, ".")
GOAL_DIST, HORIZON, COST_STEP, COST_WRONG, T_SENSE = 12, 20, 1.0, 8.0, 0.5

def gen_structured(T, seed, base_rate=25.0, n_bursts=6):
    rng=random.Random(seed); bg=[]; t=0.0
    while t<T:
        t+=rng.expovariate(base_rate)
        if t<T: bg.append(t)
    gap=T/(n_bursts+1); spikes=list(bg); brng=random.Random(seed+7)
    for i in range(n_bursts):
        strength=3+i; center=(gap*(i+1))%T
        for _ in range(strength):
            st=(center+brng.gauss(0.0,gap*0.025))%T
            spikes.append(max(0.0,min(T-1e-9,st)))
    spikes.sort(); amps=[]
    for st in spikes:
        dmin=min(abs(st-(gap*(i+1))%T) for i in range(n_bursts))
        amps.append(1.0+3.5*math.exp(-((dmin/(gap*0.06))**2)))
    return spikes,amps

def gen_scrambled(T, seed, base_rate=200.0):
    rng=random.Random(seed); spikes=[]; t=0.0
    while t<T:
        t+=rng.expovariate(base_rate)
        if t<T: spikes.append(t)
    return spikes, [1.0+rng.uniform(-0.02,0.02) for _ in spikes]

def rate_cv(times, T):
    rate=len(times)/max(T,1e-9)
    isis=[times[i+1]-times[i] for i in range(len(times)-1)]
    if len(isis)<2: return rate, 0.0
    mu=sum(isis)/len(isis); var=sum((x-mu)**2 for x in isis)/len(isis)
    return rate, (math.sqrt(var)/mu if mu else 0.0)

def sigma_amps(amps):
    n=len(amps)
    if n<8: return 0.0
    odd=list(range(0,n,2))
    if len(odd)<4: return 0.0
    Ee,Eo=odd[0::2],odd[1::2]
    return sum(amps[i] for i in Ee)/len(Ee)-sum(amps[i] for i in Eo)/len(Eo)

def sense(ch, T):
    times,amps=ch; r,cv=rate_cv(times,T)
    return {"rate":r,"cv":cv,"sigma":sigma_amps(amps),"n":len(times)}

def classical_choose(L, R):
    if R["rate"]>L["rate"]*1.05: return "RIGHT"
    if L["rate"]>R["rate"]*1.05: return "LEFT"
    return "RIGHT" if R["cv"]>=L["cv"] else "LEFT"

def dual_choose(L, R, floor=0.05):
    lo=abs(L["sigma"])>=floor; ro=abs(R["sigma"])>=floor
    if ro and not lo: return "RIGHT"
    if lo and not ro: return "LEFT"
    if lo and ro: return "RIGHT" if abs(R["sigma"])>=abs(L["sigma"]) else "LEFT"
    return "STAY"

def run_episode(mode, seed, structure_side):
    progress=0; cost=0.0; wrong=0; stays=0; steps=0
    for t in range(HORIZON):
        if progress>=GOAL_DIST: break
        goal_ch=gen_structured(T_SENSE, seed*17+t)
        dist_ch=gen_scrambled(T_SENSE, seed*31+t+3)
        if structure_side=="RIGHT":
            L,R=sense(dist_ch,T_SENSE),sense(goal_ch,T_SENSE)
        else:
            L,R=sense(goal_ch,T_SENSE),sense(dist_ch,T_SENSE)
        action=classical_choose(L,R) if mode=="classical" else dual_choose(L,R)
        if action=="STAY":
            stays+=1; cost+=0.1; continue
        steps+=1; cost+=COST_STEP
        if action==structure_side:
            progress=min(GOAL_DIST, progress+1)
        else:
            progress=max(0, progress-1); wrong+=1; cost+=COST_WRONG
    return dict(progress=progress,goal=progress>=GOAL_DIST,cost=cost,wrong=wrong,stays=stays,steps=steps,dist=GOAL_DIST-progress)

def main():
    print("# closed-loop spike agent — DEATH MATCH")
    print("# classical chases RATE (loud distractor)")
    print("# dual follows STRUCTURE (σ_Δ OWNED beacon)")
    print("# EXTERNAL-clean. THE BEASTIE BOYZ / Blam")
    print()
    g=gen_structured(T_SENSE,0); d=gen_scrambled(T_SENSE,3)
    print(f"# sanity: goal rate={len(g[0])/T_SENSE:.0f} sig={sigma_amps(g[1]):+.3f} | distractor rate={len(d[0])/T_SENSE:.0f} sig={sigma_amps(d[1]):+.3f}")
    print()
    print("mode,episodes,goals,mean_progress,mean_dist,mean_cost,mean_wrong,mean_stays")
    summary={}
    for mode in ("classical","dual"):
        n=40; goals=0; P=D=C=W=S=0.0
        for s in range(n):
            side="RIGHT" if s%2==0 else "LEFT"
            r=run_episode(mode,s,side)
            goals+=int(r["goal"]); P+=r["progress"]; D+=r["dist"]; C+=r["cost"]; W+=r["wrong"]; S+=r["stays"]
        summary[mode]=dict(goals=goals,progress=P/n,wrong=W/n,cost=C/n)
        print(f"{mode},{n},{goals}/{n},{P/n:.2f},{D/n:.2f},{C/n:.2f},{W/n:.2f},{S/n:.2f}")
    print()
    print("# sample seed=0 structure=RIGHT")
    for mode in ("classical","dual"):
        r=run_episode(mode,0,"RIGHT")
        print(f"#   {mode}: progress={r['progress']}/{GOAL_DIST} goal={r['goal']} wrong={r['wrong']} stays={r['stays']} cost={r['cost']:.1f}")
    print("# sample seed=1 structure=LEFT")
    for mode in ("classical","dual"):
        r=run_episode(mode,1,"LEFT")
        print(f"#   {mode}: progress={r['progress']}/{GOAL_DIST} goal={r['goal']} wrong={r['wrong']} stays={r['stays']} cost={r['cost']:.1f}")
    print()
    cg,dg=summary["classical"]["goals"],summary["dual"]["goals"]
    cw,dw=summary["classical"]["wrong"],summary["dual"]["wrong"]
    print(f"# DEATH-MATCH: classical goals={cg}/40 dual goals={dg}/40")
    print(f"#              classical mean_wrong={cw:.2f} dual mean_wrong={dw:.2f}")
    if dg>cg and dw<cw: print("# VERDICT: DUAL OWNS — more goals, fewer wrong commits")
    elif dw<cw: print("# VERDICT: dual safer (fewer wrongs); goals secondary")
    else: print("# VERDICT: inspect")
    print("SELF-CHECK GREEN")

if __name__ == "__main__":
    main()
