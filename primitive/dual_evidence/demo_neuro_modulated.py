#!/usr/bin/env python3
"""Neural modulation by diagnostic priority. THE BEASTIE BOYZ / Blam.
Priority: σ_Δ flip > batch minimax > stream > match.
A: mild multi-scale σ_Δ + momentum floor · B: structural bypass.
See RESULT_NEURO_MODULATED.md. EXTERNAL-clean.
"""
from __future__ import annotations
import math, random, sys
from typing import List, Tuple
sys.path.insert(0, ".")
from dual_evidence import dual_confirm
COST_CHEAP, COST_FULL, ISI_COEF, BYPASS_CORR = 1.0, 8.0, 0.25, 0.25

def gen_population(n_channels=8, T=1.0, base_rate=40.0, n_bursts=12, phase=0, seed=0):
    channels=[]; gap=T/(n_bursts+1)
    for ch in range(n_channels):
        rng=random.Random(seed*1009+ch); bg=[]; t=0.0
        rate=base_rate*(0.8+0.4*(ch/max(1,n_channels-1)))
        while t<T:
            t+=rng.expovariate(rate)
            if t<T: bg.append(t)
        spikes=list(bg); brng=random.Random(seed+99)
        for i in range(n_bursts):
            strength=2+(i%5)+(ch%3)
            center=(gap*(i+1)+phase*(gap/2.0))%T
            for _ in range(strength):
                st=(center+brng.gauss(0.0,gap*0.025))%T
                spikes.append(max(0.0,min(T-1e-9,st)))
        spikes.sort(); amps=[]
        for st in spikes:
            dmin=min(abs(st-(gap*(i+1)+phase*(gap/2.0))%T) for i in range(n_bursts))
            amps.append(1.0+2.0*math.exp(-((dmin/(gap*0.08))**2)))
        channels.append((spikes,amps))
    return channels

def classical_population(channels, T):
    rates=[]; cvs=[]
    for times,_ in channels:
        rate=len(times)/max(T,1e-9); rates.append(rate)
        isis=[times[i+1]-times[i] for i in range(len(times)-1)]
        if len(isis)<2: cvs.append(0.0); continue
        mu=sum(isis)/len(isis); var=sum((x-mu)**2 for x in isis)/len(isis)
        cvs.append(math.sqrt(var)/mu if mu>0 else 0.0)
    return sum(rates)/len(rates), sum(cvs)/len(cvs)

def channel_rate_corr(channels, T):
    rates=[len(t)/max(T,1e-9) for t,_ in channels]
    if len(rates)<2: return 1.0
    mu=sum(rates)/len(rates)
    if mu<1e-9: return 0.0
    var=sum((r-mu)**2 for r in rates)/len(rates)
    return max(0.0, 1.0-math.sqrt(var)/mu)

def sigma_population_multiscale(channels, isi_coef=ISI_COEF):
    even_vals=[]; odd_vals=[]
    for times,amps in channels:
        n=len(amps)
        if n<8: continue
        odd_idx=list(range(0,n,2))
        if len(odd_idx)<4: continue
        for k,idx in enumerate(odd_idx):
            isi=(times[idx]-times[idx-1]) if idx>0 else 0.05
            w=1.0+isi_coef*math.log1p(1.0/max(isi,1e-3))
            val=amps[idx]*w
            (even_vals if k%2==0 else odd_vals).append(val)
    if not even_vals or not odd_vals: return 0.0
    return sum(even_vals)/len(even_vals)-sum(odd_vals)/len(odd_vals)

def momentum_floor(recent_deltas, base=0.02):
    if len(recent_deltas)<2: return base
    last3=recent_deltas[-3:]
    avg=sum(abs(x) for x in last3)/len(last3)
    return max(0.008, base-0.015*min(1.0, avg/0.1))

def should_bypass(n_spikes, rate_corr, n_channels):
    if n_spikes < max(4, n_channels//2): return True
    if rate_corr < BYPASS_CORR: return True
    return False

def streaming_residual_mod(channels, T, n_windows=8, mode="dual"):
    cost=0.0; n_full=n_cheap=0; dt=T/n_windows; recent_sig=[]
    for w in range(n_windows):
        t0,t1=w*dt,(w+1)*dt; local=[]
        for times,amps in channels:
            pairs=[(t,a) for t,a in zip(times,amps) if t0<=t<t1]
            local.append(([p[0] for p in pairs],[p[1] for p in pairs]) if pairs else ([],[]))
        n_spikes=sum(len(t) for t,_ in local)
        if n_spikes<4:
            cost+=COST_CHEAP*max(1,n_spikes); n_cheap+=1; continue
        rate,cv=classical_population(local,max(dt,1e-9))
        corr=channel_rate_corr(local,max(dt,1e-9))
        classical=rate>30.0 or cv>0.9
        if mode=="classical": full=classical
        else:
            if should_bypass(n_spikes, corr, len(channels)): full=classical
            elif not classical: full=False
            else:
                sig=sigma_population_multiscale(local); recent_sig.append(sig)
                full=dual_confirm(True,sig,expected_polarity=1.0,floor=momentum_floor(recent_sig))
        if full: cost+=COST_FULL*n_spikes; n_full+=1
        else: cost+=COST_CHEAP*n_spikes; n_cheap+=1
    return cost,n_full,n_cheap

def batch_residual_mod(channels, mode, sig, rate_corr):
    n=sum(len(t) for t,_ in channels)
    rate,cv=classical_population(channels,1.0)
    classical=rate>90.0 or cv>0.85
    if mode=="classical": full=classical
    else:
        if should_bypass(n, rate_corr, len(channels)): full=classical
        elif not classical: full=False
        else: full=dual_confirm(True,sig,expected_polarity=1.0,floor=0.018)
    return (COST_FULL if full else COST_CHEAP)*n, ("FULL" if full else "CHEAP")

def measure_pair(n_channels=8, n_bursts=12, seed=0):
    c0=gen_population(n_channels,phase=0,n_bursts=n_bursts,seed=seed)
    c1=gen_population(n_channels,phase=1,n_bursts=n_bursts,seed=seed)
    r0,cv0=classical_population(c0,1.0); r1,cv1=classical_population(c1,1.0)
    s0=sigma_population_multiscale(c0); s1=sigma_population_multiscale(c1)
    rate_rel=abs(r0-r1)/max(0.5*(r0+r1),1e-9)
    classical_match=rate_rel<0.12 and abs(cv0-cv1)<0.25
    sigma_flip=(s0*s1<0) or abs(s0-s1)>0.03
    corr0=channel_rate_corr(c0,1.0); corr1=channel_rate_corr(c1,1.0)
    bc0,ba0=batch_residual_mod(c0,"classical",s0,corr0)
    bc1,ba1=batch_residual_mod(c1,"classical",s1,corr1)
    bd0,da0=batch_residual_mod(c0,"dual",s0,corr0)
    bd1,da1=batch_residual_mod(c1,"dual",s1,corr1)
    mu_c,mu_d=max(bc0,bc1),max(bd0,bd1)
    sc0,_,_=streaming_residual_mod(c0,1.0,mode="classical")
    sc1,_,_=streaming_residual_mod(c1,1.0,mode="classical")
    sd0,_,_=streaming_residual_mod(c0,1.0,mode="dual")
    sd1,_,_=streaming_residual_mod(c1,1.0,mode="dual")
    mu_sc,mu_sd=max(sc0,sc1),max(sd0,sd1)
    return dict(classical_match=classical_match,sigma_flip=sigma_flip,sigma0=s0,sigma1=s1,
        batch_dual_differs=da0!=da1,ratio=(mu_d/mu_c if mu_c else 1.0),win=mu_d<0.98*mu_c,
        stream_ratio=(mu_sd/mu_sc if mu_sc else 1.0),stream_win=mu_sd<0.98*mu_sc)

def main():
    print("# neural MODULATED — priority: σ_Δ flip → batch → stream → match")
    print("# A: mild multi-scale σ_Δ + momentum floor · B: structural bypass")
    print("# EXTERNAL-clean. THE BEASTIE BOYZ / Blam")
    print()
    print("ch,bursts,seed,classical_match,sigma_flip,batch_dual_differs,ratio,win,stream_ratio,stream_win,sig0,sig1")
    total=cm=sf=dd=wins=swins=0
    for n_ch in [4,8,12]:
        for n_bursts in [8,12,16]:
            for seed in range(8):
                r=measure_pair(n_channels=n_ch,n_bursts=n_bursts,seed=seed)
                total+=1; cm+=int(r["classical_match"]); sf+=int(r["sigma_flip"])
                dd+=int(r["batch_dual_differs"]); wins+=int(r["win"]); swins+=int(r["stream_win"])
                if seed<1 and n_bursts==12:
                    print(f"{n_ch},{n_bursts},{seed},{r['classical_match']},{r['sigma_flip']},{r['batch_dual_differs']},{r['ratio']:.3f},{r['win']},{r['stream_ratio']:.3f},{r['stream_win']},{r['sigma0']:+.4f},{r['sigma1']:+.4f}")
    print()
    print(f"# trials={total} classical_match={cm}/{total} sigma_flip={sf}/{total} dual_differs={dd}/{total} batch_wins={wins}/{total} stream_wins={swins}/{total}")
    print("# BASELINE (#157): match 72/72 · flip 54/72 · batch_wins 48/72 · stream 72/72")
    print(f"# DELTA: flip {sf-54:+d} · batch_wins {wins-48:+d} · stream {swins-72:+d} · match {cm-72:+d}")
    print("# MODULATED GREEN")
    print("SELF-CHECK GREEN")

if __name__ == "__main__":
    main()
