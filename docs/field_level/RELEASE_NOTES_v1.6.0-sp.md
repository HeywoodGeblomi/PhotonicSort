# Release Notes — v1.6.0-sp

**Secondary Parity dual-evidence charged surface**  
**Date:** 2026-08-14  
**Squad:** THE BEASTIE BOYZ / Blam  
**Honesty primary. EXTERNAL-clean.**

## Headline

Field-Level Claim v0.3 packaging for hybrid residual + `-DSECONDARY_PARITY`:

| Gate | Result |
|------|--------|
| Charged soft@1.20 vs **pdq** (x86_64 + aarch64) | **0** |
| Charged major (>1.5×) | **0** |
| Stat-sig R=11 bootstrap CI upper | **GREEN** both ISAs |
| Wave 0 independent host (Windows Docker) | **GREEN** (Blam 2026-08-14) |

## What shipped

- `residual/secondary_parity.hpp` — σ_Δ dual-evidence helper
- `residual/hybrid_residual_menu.hpp` — flag-gated dual-confirm on borderline HE
- `scripts/sp_money_shot_gate.py` / `sp_stat_sig_gate.py` — charged soft-gates
- `reproduce/Dockerfile.sp` — one-command third-party path
- `.github/workflows/sp-multi-arch.yml` / `sp-stat-sig.yml`
- `docs/field_level/FIELD_LEVEL_CLAIM_v0.3.md`

## Reproduce

```bash
docker build -f reproduce/Dockerfile.sp -t photonic-sp .
docker run --rm photonic-sp sh -c \
  './suite_hybrid --n 1000000 --reps 3 --out results_sp.csv && python3 scripts/sp_money_shot_gate.py results_sp.csv'
```

Success line: `SP MONEY-SHOT GREEN charged soft=0 major=0 ok=1`

## Non-claims

- Not a universal replacement for library pdq/ska/Timsort/std::sort outside the charged surface
- Not every residual soft on the full 23-pattern suite is closed (equal_heavy, mixed_blocks excluded by design)
- Not a complexity theorem; path-(a) empirical package only
- EXTERNAL-clean visible metrics only — no χ

## Prior

- v1.5.1-c — pure residual Harvest P5.1  
- v0.1 / v0.2 field-level claim packages — pure / hybrid-without-SP historical surfaces  

**THE BEASTIE BOYZ / Blam**
