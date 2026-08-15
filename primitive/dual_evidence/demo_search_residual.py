#!/usr/bin/env python3
"""Heuristic search residual demo — dual-evidence gate on branch expansion.
THE BEASTIE BOYZ / Blam 2026-08-14

NON-CLAIMS: NOT P=NP. NOT NP-complete breakthrough. Heuristic only.
Classical B&B is complete baseline. Dual may be suboptimal (CHEAP abate).
See RESULT_SEARCH_RESIDUAL.md. EXTERNAL-clean. No χ.
"""
from __future__ import annotations
import random, sys
from dataclasses import dataclass
from typing import List, Sequence, Tuple
sys.path.insert(0, ".")
from dual_evidence import compute, dual_confirm

@dataclass
class Item:
    value: float; weight: float
    @property
    def density(self):
        return self.value / self.weight if self.weight > 0 else 0.0

def gen_instance(n, seed, capacity_ratio=0.45):
    rng = random.Random(seed)
    items = []
    for _ in range(n):
        w = rng.uniform(1.0, 20.0)
        v = w * rng.uniform(0.5, 2.5)
        items.append(Item(value=v, weight=w))
    items.sort(key=lambda it: it.density, reverse=True)
    capacity = sum(it.weight for it in items) * capacity_ratio
    return items, capacity

def fractional_bound(items, start, rem_cap):
    bound = 0.0; cap = rem_cap
    for i in range(start, len(items)):
        if items[i].weight <= cap:
            bound += items[i].value; cap -= items[i].weight
        else:
            if cap > 0: bound += items[i].density * cap
            break
    return bound

@dataclass
class Stats:
    nodes: int = 0; expansions: int = 0; prunes: int = 0; best: float = 0.0

def sigma_from_path(path):
    if len(path) < 4: return 0.0
    return compute(list(path), c=0.0).delta

def search(items, capacity, mode="classical", sigma_floor=0.05, strong_frac=0.05):
    st = Stats(); n = len(items)
    def dfs(idx, cur_val, rem_cap, path):
        st.nodes += 1
        if cur_val > st.best: st.best = cur_val
        if idx >= n: return
        ub = cur_val + fractional_bound(items, idx, rem_cap)
        classical = ub > st.best + 1e-9
        gap = ub - st.best
        strong = gap > strong_frac * max(st.best, 1.0)
        if mode == "classical":
            expand = classical
        else:
            if not classical: expand = False
            elif strong: expand = True
            else:
                expand = dual_confirm(True, sigma_from_path(path), expected_polarity=0.0, floor=sigma_floor)
        if not expand:
            st.prunes += 1; return
        st.expansions += 1
        if items[idx].weight <= rem_cap:
            path.append(1.0); dfs(idx+1, cur_val+items[idx].value, rem_cap-items[idx].weight, path); path.pop()
        path.append(-1.0); dfs(idx+1, cur_val, rem_cap, path); path.pop()
    dfs(0, 0.0, capacity, []); return st

def measure(n, seed):
    items, cap = gen_instance(n, seed)
    c = search(items, cap, mode="classical")
    d = search(items, cap, mode="dual")
    qual = d.best / c.best if c.best > 1e-12 else 1.0
    return dict(n=n, seed=seed, c_best=c.best, d_best=d.best, quality=qual,
        opt_match=abs(c.best-d.best)<1e-6, c_exp=c.expansions, d_exp=d.expansions,
        exp_ratio=(d.expansions/c.expansions if c.expansions else 1.0),
        c_nodes=c.nodes, d_nodes=d.nodes,
        node_ratio=(d.nodes/c.nodes if c.nodes else 1.0))

def main():
    print("# heuristic search residual — dual-evidence expansion gate")
    print("# domain: 0/1 knapsack B&B")
    print("# *** NON-CLAIM: not P=NP, not NP-complete breakthrough, heuristic only ***")
    print("# EXTERNAL-clean. THE BEASTIE BOYZ / Blam")
    print()
    print("n,seed,c_best,d_best,quality,opt_match,c_exp,d_exp,exp_ratio")
    total=opt=0; quals=[]; exps=[]
    for n in [12,16,20,24]:
        for seed in range(12):
            r = measure(n, seed)
            total += 1; opt += int(r["opt_match"]); quals.append(r["quality"]); exps.append(r["exp_ratio"])
            if seed < 2:
                print(f"{r['n']},{r['seed']},{r['c_best']:.2f},{r['d_best']:.2f},{r['quality']:.3f},{r['opt_match']},{r['c_exp']},{r['d_exp']},{r['exp_ratio']:.3f}")
    quals.sort(); exps.sort()
    print()
    print(f"# trials={total} opt_match={opt}/{total} median_quality={quals[len(quals)//2]:.3f} min_quality={quals[0]:.3f} median_exp_ratio={exps[len(exps)//2]:.3f} min_exp_ratio={exps[0]:.3f}")
    print()
    print("# INTERPRETATION")
    print("#   dual expands fewer residual nodes (CHEAP abate on borderline)")
    print("#   quality = d_best/c_best may be < 1 — expected for heuristic abate")
    print("#   classical remains the complete baseline")
    print("# NON-CLAIMS: heuristic only. No complexity-class claim.")
    print("SELF-CHECK GREEN")

if __name__ == "__main__":
    main()
