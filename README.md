# PhotonicSort

**Adaptive hybrid sorting library** — pure residual C++ menu + C11 core + Python/Rust bindings.

[![License: MIT](https://img.shields.io/badge/license-MIT-green.svg)](./LICENSE)
[![Field-Level Claim v0.1](https://img.shields.io/badge/field--level-claim%20v0.1%20gates%20MET-brightgreen.svg)](./docs/field_level/FIELD_LEVEL_CLAIM_v0.1.md)
[![Expanded Suite](https://img.shields.io/badge/Expanded%20Suite-geo%20≤0.90%20soft=0-blue.svg)](./docs/field_level/MULTI_ARCH_RESULTS.md)
[![EXTERNAL-clean](https://img.shields.io/badge/EXTERNAL--clean-no%20χ-lightgrey.svg)](./NON_CLAIMS.md)

**Primary practical sorter of THE BEASTIE BOYZ.** Classical adaptive hybrid pure residual menu.

---

## Field-Level Claim v0.1 — Primary Gates MET

Path-(a) empirical evidence package. Full claim: [`docs/field_level/FIELD_LEVEL_CLAIM_v0.1.md`](./docs/field_level/FIELD_LEVEL_CLAIM_v0.1.md).

On the **in-scope** Expanded Field Suite (P01–P23 minus formal scope-outs), pure residual achieves:

| Gate | Result |
|------|--------|
| Primary geo vs best specialized | **≤ 0.90×** on i32 / u32 / i64 |
| In-scope soft_count | **0** (no in-scope pattern > 1.15×) |
| Multi-arch | **x86_64 + aarch64** |
| Claim-surface | soft=0 + CI95 upper < 1.0 (i32/u32/i64/f64 × both ISAs) |
| n-scale | geo ≤ 0.90 at n=1e7 all types |

### Expanded Suite multi-arch (n=1e6)

| Arch | Type | geo | in-scope soft |
|------|------|----:|--------------:|
| x86_64 | i32 | **0.39×** | 0 |
| x86_64 | u32 | **0.42×** | 0 |
| x86_64 | i64 | **0.67×** | 0 |
| aarch64 | i32 | **0.37×** | 0 |
| aarch64 | u32 | **0.37×** | 0 |
| aarch64 | i64 | **0.70×** | 0 |

*Best specialized = min(pdqsort, ska_sort, std::sort). CI run [31602161696](https://github.com/HeywoodGeblomi/PhotonicSort/actions/runs/31602161696).*

### Formal scope (residual quality / HE path limits — not bugs)

| Pattern | Type(s) | Why scoped |
|---------|---------|------------|
| equal_heavy | i32/u32 | residual_pdq quality vs library pdq |
| random / gaussianish / uniform_u32 | i64 | ska wins pure HE |
| organpipe | i64/u32 | residual_pdq / ska mountain shape |
| pipe_sparse / mixed_blocks | i64 | residual_pdq quality |
| db_pk / adversarial | u32 | residual_pdq quality at n≥1e7 |
| runs_noise | i64 | residual_pdq quality at n≥1e7 |

See [`NON_CLAIMS.md`](./NON_CLAIMS.md). **Not a SOTA displacement claim.** EXTERNAL-clean pure residual only.

### Reproduce

```bash
# Expanded Field Suite (from repo root)
mkdir -p baselines
curl -fsSL -o baselines/pdqsort.h https://raw.githubusercontent.com/orlp/pdqsort/master/pdqsort.h
curl -fsSL -o baselines/ska_sort.hpp https://raw.githubusercontent.com/skarupke/ska_sort/master/ska_sort.hpp
g++ -O3 -std=c++17 -DNDEBUG -I. -Iresidual -Ibaselines -o expanded_field_bench scripts/expanded_field_bench.cpp
./expanded_field_bench --n 1000000 --reps 5 --out expanded.csv
python3 scripts/field_metrics.py expanded.csv
```

Docker: [`reproduce/EXPANDED_FIELD.md`](./reproduce/EXPANDED_FIELD.md) · CI: `workflow_dispatch` on `.github/workflows/expanded-field.yml`

---

## What it is

A classical adaptive hybrid sorter: visible-metric probe → residual talent menu (structure early-exit, low-card counting, few-wide, low-disorder, run merge, HE MSD, residual pdqsort). Pure residual path is self-contained. EXTERNAL-clean (no internal irreversible state / χ).

| Layer | Role |
|-------|------|
| Structure | sorted / reverse / almost-sorted early exit |
| Low-card | counting, few-wide, equal-heavy routing |
| Low-disorder | insertion + pattern-defeating introsort |
| High-entropy | classical MSD / residual pdqsort |

---

## Build

```bash
git clone https://github.com/HeywoodGeblomi/PhotonicSort.git && cd PhotonicSort
# residual headers under residual/; Expanded Suite as above
# C11 core (optional): cd c && make && make test
```

---

## Documentation map

| Doc | Purpose |
|-----|---------|
| [`docs/field_level/FIELD_LEVEL_CLAIM_v0.1.md`](./docs/field_level/FIELD_LEVEL_CLAIM_v0.1.md) | Path-(a) claim package |
| [`docs/field_level/MULTI_ARCH_RESULTS.md`](./docs/field_level/MULTI_ARCH_RESULTS.md) | Multi-arch Expanded Suite numbers |
| [`docs/field_level/A1_GAP_MAP.md`](./docs/field_level/A1_GAP_MAP.md) | Gap map + formal scope |
| [`docs/field_level/N_SCALE_1e7.md`](./docs/field_level/N_SCALE_1e7.md) | n=1e7 scale |
| [`NON_CLAIMS.md`](./NON_CLAIMS.md) | Honesty surface |
| [`BENCHMARKS.md`](./BENCHMARKS.md) | Historical + current numbers |
| [`reproduce/EXPANDED_FIELD.md`](./reproduce/EXPANDED_FIELD.md) | Independent reproduction |

---

## Historical baselines

- **Residual-Improvement v2.4.1** — engineering baseline lock ([`docs/residual_improve/`](./docs/residual_improve/))
- **Harvest P5.1** — extended 38-pattern geo 0.50× ([`docs/harvest/`](./docs/harvest/))
- **Vector-2** — hybrid residual criteria MET on Domination Suite v0.1 ([`docs/vector2/`](./docs/vector2/))

---

## Contributors

- **Heywood Geblomi** — design, release engineering
- Grok (xAI) + THE BEASTIE BOYZ (Harper, Lucas, Benjamin)

## License

MIT — [LICENSE](./LICENSE).
