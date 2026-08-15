# PhotonicSort

**Adaptive hybrid sorting library** — Secondary Parity dual-evidence residual + pure residual C++ menu + C11 core + Python/Rust bindings.

[![License: MIT](https://img.shields.io/badge/license-MIT-green.svg)](./LICENSE)
[![Field-Level Claim v0.5](https://img.shields.io/badge/field--level-claim%20v0.5%20LOCKED-brightgreen.svg)](./docs/field_level/FIELD_LEVEL_CLAIM_v0.5.md)
[![Release v1.6.0-sp.2](https://img.shields.io/badge/release-v1.6.0--sp.2-blue.svg)](https://github.com/HeywoodGeblomi/PhotonicSort/releases/tag/v1.6.0-sp.2)
[![EXTERNAL-clean](https://img.shields.io/badge/EXTERNAL--clean-no%20χ-lightgrey.svg)](./NON_CLAIMS.md)

**Primary practical sorter of THE BEASTIE BOYZ.** Classical adaptive hybrid. EXTERNAL-clean (visible metrics only — no χ).

---

## Field-Level Claim v0.5 — LOCKED

**Secondary Parity dual-evidence** hybrid residual — **DEFAULT production path** (PR #167).  
**Full charged surface. Zero hard-fail exclusions.** Escape hatch only: `-DCLASSICAL_RESIDUAL`.

Full claim: [`docs/field_level/FIELD_LEVEL_CLAIM_v0.5.md`](./docs/field_level/FIELD_LEVEL_CLAIM_v0.5.md)  
Prior release tag: [**v1.6.0-sp.2**](https://github.com/HeywoodGeblomi/PhotonicSort/releases/tag/v1.6.0-sp.2) (v0.4 surface; v0.5 claim lives on main tip)

| Gate | Result |
|------|--------|
| Charged soft@1.20 vs **pdq** (x86_64 + aarch64) | **0** |
| Charged major (>1.5×) | **0** |
| Money-shot multi-arch (v0.5 CHARGED) | [run 31807222980](https://github.com/HeywoodGeblomi/PhotonicSort/actions/runs/31807222980) ALL GREEN |
| Stat-sig R=11 bootstrap CI (prior surface) | GREEN both ISAs |
| Wave 0 independent host (Windows Docker) | GREEN |

### Charged surface (hard-fail) — v0.5

sorted · reverse · almost_sorted · few_k4_dense · few_k16_dense · organpipe · sawtooth · push_middle · runs_noise · random · gaussianish · adversarial · db_pk · timestamps · reverse_segments · equal_heavy · **mixed_blocks**

**Hard-fail exclusions:** *none*

### What Secondary Parity adds

Probe stream → odd-event restriction → ordinal partition (even# / odd#) → contrast σ_Δ. Dual-evidence residual selection on borderline HE: classical metrics **and** secondary parity must agree before committing a residual talent.

Soft-attack residuals on main:
- `reverse_segments` — O(n) fixed-block reverse
- `equal_heavy` — library pdq path
- `mixed_blocks` — O(n) alternating-block detector + disordered-block residual + cheap pre-check

### Reproduce (one-command)

```bash
docker build -f reproduce/Dockerfile.sp -t photonic-sp .
docker run --rm photonic-sp sh -c \
  './suite_hybrid --n 1000000 --reps 3 --out results_sp.csv && python3 scripts/sp_money_shot_gate.py results_sp.csv'
```

Success line: `SP MONEY-SHOT GREEN charged soft=0 major=0 ok=1`

### Non-claims

- Not a universal replacement for library pdq/ska/Timsort/std::sort outside the charged surface
- Gate is vs **pdq**; specialized may still win isolated cells
- Not a complexity theorem; path-(a) empirical package only
- EXTERNAL-clean visible metrics only — no χ

See [`NON_CLAIMS.md`](./NON_CLAIMS.md).

---

## What it is

A classical adaptive hybrid sorter: visible-metric probe → residual talent menu (structure early-exit, low-card counting, few-wide, low-disorder, run merge, HE MSD, residual / library pdq). Pure residual path is self-contained; hybrid residual uses Secondary Parity dual-evidence by **default** (no flag required). Classical escape: `-DCLASSICAL_RESIDUAL`.

| Layer | Role |
|-------|------|
| Structure | sorted / reverse / almost-sorted early exit |
| Low-card | counting, few-wide, equal-heavy routing |
| Low-disorder | insertion + pattern-defeating introsort |
| High-entropy | classical MSD / residual or library pdq |
| Secondary Parity | dual-evidence borderline residual selection |

---

## Install & use the dual residual (C++)

Dual residual / residual talent is the **default production path**. No special flag required.

```bash
git clone https://github.com/HeywoodGeblomi/PhotonicSort.git && cd PhotonicSort

# fetch library baselines once (pdqsort + ska_sort)
mkdir -p baselines
curl -fsSL -o baselines/pdqsort.h \
  https://raw.githubusercontent.com/orlp/pdqsort/master/pdqsort.h
curl -fsSL -o baselines/ska_sort.hpp \
  https://raw.githubusercontent.com/skarupke/ska_sort/master/ska_sort.hpp

cmake -B build -DCMAKE_INSTALL_PREFIX=$HOME/.local
cmake --build build
cmake --install build
```

```cpp
#include <PhotonicSort/hybrid_residual_menu.hpp>
// or from source tree: #include "hybrid_residual_menu.hpp"  with -Iresidual -Ibaselines

int64_t a[n] = { /* ... */ };
hybrid_residual::sort_i64(a, n);   // DEFAULT = dual residual
```

Escape hatch (classical residual only):

```bash
cmake -B build -DCLASSICAL_RESIDUAL=ON -DCMAKE_INSTALL_PREFIX=$HOME/.local
```

C11 core (optional): `cd c && make && make test`  
Python reference: `pip install .` (pure-Python; dual residual is the C++ path)  
Rust (C11 path): `cargo add photonic-sort`

See [BUILD.md](./BUILD.md) for Docker stages and full options.

---

## Documentation map

| Doc | Purpose |
|-----|---------|
| [`docs/field_level/FIELD_LEVEL_CLAIM_v0.5.md`](./docs/field_level/FIELD_LEVEL_CLAIM_v0.5.md) | **Current** Field-Level claim (full charged surface) |
| [`docs/field_level/FIELD_LEVEL_CLAIM_v0.4.md`](./docs/field_level/FIELD_LEVEL_CLAIM_v0.4.md) | Prior claim (mixed_blocks excluded) |
| [`docs/field_level/RELEASE_NOTES_v1.6.0-sp.2.md`](./docs/field_level/RELEASE_NOTES_v1.6.0-sp.2.md) | Release notes for tag v1.6.0-sp.2 |
| [`docs/field_level/SP_MULTI_ARCH.md`](./docs/field_level/SP_MULTI_ARCH.md) | Multi-arch money-shot protocol |
| [`NON_CLAIMS.md`](./NON_CLAIMS.md) | Honesty surface |
| [`BENCHMARKS.md`](./BENCHMARKS.md) | Historical + current numbers |
| [`reproduce/Dockerfile.sp`](./reproduce/Dockerfile.sp) | Independent SP reproduction |

---

## Historical baselines

- **Field-Level Claim v0.4** — charged surface with mixed_blocks exclusion ([`FIELD_LEVEL_CLAIM_v0.4.md`](./docs/field_level/FIELD_LEVEL_CLAIM_v0.4.md))
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
