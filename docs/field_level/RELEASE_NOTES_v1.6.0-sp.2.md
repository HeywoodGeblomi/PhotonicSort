# Release Notes — v1.6.0-sp.2

**Secondary Parity dual-evidence — expanded charged surface (v0.4 LOCKED)**  
**Date:** 2026-08-14  
**Squad:** THE BEASTIE BOYZ / Blam  
**Honesty primary. EXTERNAL-clean.**

## Headline

Field-Level Claim **v0.4 LOCKED** for hybrid residual + `-DSECONDARY_PARITY`:

| Gate | Result |
|------|--------|
| Charged soft@1.20 vs **pdq** (x86_64 + aarch64) | **0** (includes reverse_segments + equal_heavy) |
| Charged major (>1.5×) | **0** |
| Money-shot multi-arch | [run 31801191011](https://github.com/HeywoodGeblomi/PhotonicSort/actions/runs/31801191011) ALL 4 jobs GREEN |
| Stat-sig R=11 bootstrap CI upper | **GREEN** both ISAs (inherited) |
| Wave 0 independent host (Windows Docker) | **GREEN** |

## What shipped beyond v1.6.0-sp.1

- Soft-attack residual levers on main:
  - `residual/residual_reverse_segments.hpp` — O(n) fixed-block reverse (~0.08× vs pdq)
  - equal-heavy early path → library pdq (~1.0×)
- Charged surface expansion: **reverse_segments** + **equal_heavy** promoted to hard-fail
- CI alignment (`sp-multi-arch.yml` inline CHARGED) — #140
- `docs/field_level/FIELD_LEVEL_CLAIM_v0.4.md` **LOCKED** — #141

## Charged surface (hard-fail)

sorted · reverse · almost_sorted · few_k4_dense · few_k16_dense · organpipe · sawtooth · push_middle · runs_noise · random · gaussianish · adversarial · db_pk · timestamps · **reverse_segments** · **equal_heavy**

**Sole exclusion:** `mixed_blocks` (residual quality vs specialized + aarch64 history).

## Reproduce

```bash
docker build -f reproduce/Dockerfile.sp -t photonic-sp .
docker run --rm photonic-sp sh -c \
  './suite_hybrid --n 1000000 --reps 3 --out results_sp.csv && python3 scripts/sp_money_shot_gate.py results_sp.csv'
```

Success line: `SP MONEY-SHOT GREEN charged soft=0 major=0 ok=1`

## Non-claims

- Not a universal replacement for library pdq/ska/Timsort/std::sort outside the charged surface
- Not every residual soft on the full 23-pattern suite is closed (`mixed_blocks` remains a residual-quality limit)
- Not a complexity theorem; path-(a) empirical package only
- EXTERNAL-clean visible metrics only — no χ

## Prior

- v1.6.0-sp / v1.6.0-sp.1 — Secondary Parity Field-Level Claim v0.3  
- v1.5.1-c — pure residual Harvest P5.1  

**THE BEASTIE BOYZ / Blam**
