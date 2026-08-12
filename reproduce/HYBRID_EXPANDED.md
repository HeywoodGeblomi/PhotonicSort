# Independent Reproduction — Hybrid Residual Expanded Field Suite

Path-(a) hybrid residual configuration.

**Compare ratios, not absolute milliseconds.**  
Reference: `docs/field_level/FIELD_LEVEL_CLAIM_v0.2.md`

## Native

From **repository root**:

```bash
mkdir -p baselines
curl -fsSL -o baselines/pdqsort.h https://raw.githubusercontent.com/orlp/pdqsort/master/pdqsort.h
curl -fsSL -o baselines/ska_sort.hpp https://raw.githubusercontent.com/skarupke/ska_sort/master/ska_sort.hpp

g++ -O3 -std=c++17 -DNDEBUG -I. -Iresidual -Ibaselines \
  -o expanded_hybrid scripts/expanded_field_bench_hybrid.cpp

./expanded_hybrid --n 1000000 --reps 7 --out hybrid.csv
python3 scripts/field_metrics.py hybrid.csv
```

Soft threshold path-(a) secondary bar: **1.20**. Primary geo ≤ 0.90.

## Non-claims

- Hybrid residual (library ska/pdq on HE and residual-quality zones)
- Not pure residual excellence
- EXTERNAL-clean visible metrics only

**THE BEASTIE BOYZ**
