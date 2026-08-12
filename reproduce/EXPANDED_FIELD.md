# Independent Reproduction — Expanded Field Suite

One-command path for the Expanded Field Suite (P01–P23) multi-baseline bench.

**Compare ratios, not absolute milliseconds.**  
Reference: `docs/field_level/MULTI_ARCH_RESULTS.md`, `docs/field_level/FIELD_LEVEL_CLAIM_v0.1.md`.

## Native (recommended)

From **repository root**:

```bash
mkdir -p baselines
curl -fsSL -o baselines/pdqsort.h https://raw.githubusercontent.com/orlp/pdqsort/master/pdqsort.h
curl -fsSL -o baselines/ska_sort.hpp https://raw.githubusercontent.com/skarupke/ska_sort/master/ska_sort.hpp

g++ -O3 -std=c++17 -DNDEBUG -I. -Iresidual -Ibaselines \
  -o expanded_field_bench scripts/expanded_field_bench.cpp

./expanded_field_bench --n 1000000 --reps 5 --out expanded.csv
python3 scripts/field_metrics.py expanded.csv
```

Optional filters: `--only i32|u32|i64`, `--n 10000000` for scale.

## Makefile (from reproduce/)

```bash
cd reproduce
make expanded          # build
make run-expanded      # n=1e6 all types
make run-expanded-i32  # i32 only
```

## Docker

```bash
# from repo root
docker build -f reproduce/Dockerfile.expanded -t photonic-expanded .
docker run --rm photonic-expanded
```

## What is measured

| Item | Value |
|------|-------|
| Suite | Expanded Field P01–P23 |
| Types | i32, u32, i64 |
| Baselines | pdqsort, ska_sort, std::sort |
| Metric | ratio vs min(pdq, ska, std) |
| Soft | ratio > 1.15 |
| Primary n | 1e6 |

## Formal scope

Patterns formally scoped as residual quality / HE path limits are listed in `docs/field_level/A1_GAP_MAP.md` §3 and `FIELD_LEVEL_CLAIM_v0.1.md`. In-scope soft_count excludes those patterns.

## Non-claims

- Not a SOTA displacement claim.
- Host-dependent absolute times; ratios are the auditable surface.
- EXTERNAL-clean pure residual only. No χ.

**THE BEASTIE BOYZ**
