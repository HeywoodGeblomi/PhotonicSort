# PhotonicSort

**Adaptive hybrid sorting library** — Secondary Parity dual-evidence residual + pure residual C++ menu + C11 core + Python/Rust bindings.

[![License: MIT](https://img.shields.io/badge/license-MIT-green.svg)](./LICENSE)
[![Field-Level Claim v0.4](https://img.shields.io/badge/field--level-claim%20v0.4%20LOCKED-brightgreen.svg)](./docs/field_level/FIELD_LEVEL_CLAIM_v0.4.md)
[![Release v1.6.0-sp.2](https://img.shields.io/badge/release-v1.6.0--sp.2-blue.svg)](https://github.com/HeywoodGeblomi/PhotonicSort/releases/tag/v1.6.0-sp.2)
[![EXTERNAL-clean](https://img.shields.io/badge/EXTERNAL--clean-no%20χ-lightgrey.svg)](./NON_CLAIMS.md)

**Primary practical sorter of THE BEASTIE BOYZ.** Classical adaptive hybrid. EXTERNAL-clean (visible metrics only — no χ).

---

## Field-Level Claim v0.4 — LOCKED

**Secondary Parity dual-evidence** hybrid residual (`-DSECONDARY_PARITY`).

Full claim: [`docs/field_level/FIELD_LEVEL_CLAIM_v0.4.md`](./docs/field_level/FIELD_LEVEL_CLAIM_v0.4.md)  
Release: [**v1.6.0-sp.2**](https://github.com/HeywoodGeblomi/PhotonicSort/releases/tag/v1.6.0-sp.2)

| Gate | Result |
|------|--------|
| Charged soft@1.20 vs **pdq** (x86_64 + aarch64) | **0** |
| Charged major (>1.5×) | **0** |
| Money-shot multi-arch | [run 31801191011](https://github.com/HeywoodGeblomi/PhotonicSort/actions/runs/31801191011) ALL GREEN |
| Stat-sig R=11 bootstrap CI | GREEN both ISAs |
| Wave 0 independent host (Windows Docker) | GREEN |

### Charged surface (hard-fail)

sorted · reverse · almost_sorted · few_k4_dense · few_k16_dense · organpipe · sawtooth · push_middle · runs_noise · random · gaussianish · adversarial · db_pk · timestamps · **reverse_segments** · **equal_heavy**

**Sole exclusion:** `mixed_blocks` (residual quality vs specialized).

### What Secondary Parity adds

Probe stream → odd-event restriction → ordinal partition (even# / odd#) → contrast σ_Δ. Dual-evidence residual selection on borderline HE: classical metrics **and** secondary parity must agree before committing a residual talent. Soft spots closed on reverse_segments (O(n) fixed-block reverse) and equal_heavy (library pdq path).

### Reproduce (one-command)

```bash
docker build -f reproduce/Dockerfile.sp -t photonic-sp .
docker run --rm photonic-sp sh -c \
  './suite_hybrid --n 1000000 --reps 3 --out results_sp.csv && python3 scripts/sp_money_shot_gate.py results_sp.csv'
```

Success line: `SP MONEY-SHOT GREEN charged soft=0 major=0 ok=1`

### Non-claims

- Not a universal replacement for library pdq/ska/Timsort/std::sort outside the charged surface
- Not every residual soft on the full 23-pattern suite is closed (`mixed_blocks` remains)
- Not a complexity theorem; path-(a) empirical package only
- EXTERNAL-clean visible metrics only — no χ

See [`NON_CLAIMS.md`](./NON_CLAIMS.md).

---

## What it is

A classical adaptive hybrid sorter: visible-metric probe → residual talent menu (structure early-exit, low-card counting, few-wide, low-disorder, run merge, HE MSD, residual / library pdq). Pure residual path is self-contained; hybrid residual adds Secondary Parity dual-evidence under `-DSECONDARY_PARITY`.

| Layer | Role |
|-------|------|
| Structure | sorted / reverse / almost-sorted early exit |
| Low-card | counting, few-wide, equal-heavy routing |
| Low-disorder | insertion + pattern-defeating introsort |
| High-entropy | classical MSD / residual or library pdq |
| Secondary Parity | dual-evidence borderline residual selection |

---

## Build

```bash
git clone https://github.com/HeywoodGeblomi/PhotonicSort.git && cd PhotonicSort
# residual headers under residual/
# SP hybrid: -DSECONDARY_PARITY (see reproduce/Dockerfile.sp)
# C11 core (optional): cd c && make && make test
```

---

## Documentation map

| Doc | Purpose |
|-----|---------|
| [`docs/field_level/FIELD_LEVEL_CLAIM_v0.4.md`](./docs/field_level/FIELD_LEVEL_CLAIM_v0.4.md) | **Current** Field-Level claim (SP dual-evidence) |
| [`docs/field_level/RELEASE_NOTES_v1.6.0-sp.2.md`](./docs/field_level/RELEASE_NOTES_v1.6.0-sp.2.md) | Release notes for tag v1.6.0-sp.2 |
| [`docs/field_level/SP_MULTI_ARCH.md`](./docs/field_level/SP_MULTI_ARCH.md) | Multi-arch money-shot protocol |
| [`NON_CLAIMS.md`](./NON_CLAIMS.md) | Honesty surface |
| [`BENCHMARKS.md`](./BENCHMARKS.md) | Historical + current numbers |
| [`reproduce/Dockerfile.sp`](./reproduce/Dockerfile.sp) | Independent SP reproduction |

---

## Historical baselines

- **Field-Level Claim v0.1** — pure residual Expanded Suite gates (historical; see [`FIELD_LEVEL_CLAIM_v0.1.md`](./docs/field_level/FIELD_LEVEL_CLAIM_v0.1.md))
- **Residual-Improvement v2.4.1** — engineering baseline lock ([`docs/residual_improve/`](./docs/residual_improve/))
- **Harvest P5.1** — extended 38-pattern geo 0.50× ([`docs/harvest/`](./docs/harvest/))
- **Vector-2** — hybrid residual criteria MET on Domination Suite v0.1 ([`docs/vector2/`](./docs/vector2/))

---

## Contributors

- **Heywood Geblomi** — design, release engineering
- Grok (xAI) + THE BEASTIE BOYZ (Harper, Lucas, Benjamin)

## License

MIT — [LICENSE](./LICENSE).
