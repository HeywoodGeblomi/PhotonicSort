# Statistical CI Packaging — Secondary Parity / Path-(a) Hybrid

**Status:** PROTOCOL LOCKED 2026-08-14  
**Squad:** THE BEASTIE BOYZ / Blam  
**Depends on:** Multi-arch money-shot LOCKED GREEN (`MULTI_ARCH_MONEY_SHOT_LOCKED.md`)

## Why

Point estimates (median of 3–5 timed runs) closed the multi-arch money-shot. Formal Field-Level packaging requires a **statistical soft-gate**: confidence intervals on per-pattern ratios, not only a single median.

## Primary ratio

$$
\rho = \frac{T_{\mathrm{menu}}}{T_{\mathrm{pdq}}}
$$

- Matches the charged-surface SP soft-gate (vs **pdq**, not vs best specialized).
- Soft threshold: **1.20**. Major: **1.50**.
- Charged surface: same as `sp-multi-arch.yml` after PR #129 (equal_heavy + mixed_blocks excluded from hard-fail).

## Trial protocol

1. **Warmup:** 1 untimed call per algorithm.
2. **Inner trials:** $R \ge 11$ independent timed runs on a fresh copy of the same generated input.
3. **Pairing:** each trial records $(T_{\mathrm{menu}}^{(i)}, T_{\mathrm{pdq}}^{(i)})$ → $\rho_i = T_{\mathrm{menu}}^{(i)} / T_{\mathrm{pdq}}^{(i)}$.
4. **Point estimate:** $\mathrm{median}(\rho_i)$.
5. **95% CI:** percentile bootstrap of the **median** ($B \ge 2000$ resamples).

Harness:

```bash
g++ -O3 -std=c++17 -DNDEBUG -DSECONDARY_PARITY \
  -I. -Iresidual -Ibaselines \
  -o suite_hybrid scripts/expanded_field_bench_hybrid.cpp

./suite_hybrid --n 1000000 --reps 11 \
  --out results.csv --raw-out results_raw.csv
```

Gate:

```bash
python3 scripts/sp_stat_sig_gate.py results_raw.csv \
  --threshold 1.20 --bootstrap 2000 --min-reps 11
```

Exit 0 iff charged soft (CI upper > 1.20) = 0 and major = 0 and no thin cells.

## Geo mean (secondary)

`scripts/field_metrics.py` remains the geo-mean + bootstrap CI helper on the **summary** CSV (`ratio_best` vs specialized). It does **not** replace the charged soft-gate.

## CI integration (next step)

- Optional `stat-sig` job on `sp-multi-arch.yml` (or thin sibling workflow) with `--reps 11 --raw-out` + `sp_stat_sig_gate.py`.
- Cost: ~2–4× wall time of the point-estimate money-shot (R=11 vs R=3).
- Fail-soft: keep money-shot job as fast gate; stat-sig as formal packaging job.

## Non-claims

- Does not claim measurement noise is the only uncertainty source (generator seeds are fixed; CI is over timing noise).
- Does not enlarge the charged surface.
- Not third-party reproduce (still open).
- EXTERNAL-clean. No χ.

**THE BEASTIE BOYZ**
