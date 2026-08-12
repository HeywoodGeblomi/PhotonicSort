# Claim Surface Reproduction

```bash
mkdir -p baselines
curl -fsSL -o baselines/pdqsort.h https://raw.githubusercontent.com/orlp/pdqsort/master/pdqsort.h
curl -fsSL -o baselines/ska_sort.hpp https://raw.githubusercontent.com/skarupke/ska_sort/master/ska_sort.hpp
g++ -O3 -std=c++17 -DNDEBUG -I. -Iresidual -Ibaselines -o claim_surface_bench scripts/claim_surface_bench.cpp
./claim_surface_bench --n 1000000 --reps 5 --out claim_surface.csv
python3 scripts/field_metrics.py claim_surface.csv --bootstrap 1000
```

Gates: geo ≤0.90×, max ≤1.15×, soft=0. Not field-level until x86_64 **and** aarch64 both PASS.

**THE BEASTIE BOYZ**
