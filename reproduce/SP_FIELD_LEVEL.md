# Wave 0 — Secondary Parity Field-Level Independent Reproduction

**Status:** SHIPPED 2026-08-14  
**Squad:** THE BEASTIE BOYZ / Blam  
**Claim surface:** Hybrid residual + `-DSECONDARY_PARITY` dual-evidence, charged soft-gate vs **pdq**

## One-command (Docker)

From **repository root**:

```bash
docker build -f reproduce/Dockerfile.sp -t photonic-sp .
docker run --rm photonic-sp
```

Expected terminal line:

```text
SP MONEY-SHOT GREEN charged soft=0 major=0 ok=1
```

### Formal statistical packaging (optional, slower)

```bash
docker run --rm -e STAT_SIG=1 photonic-sp
```

Expected:

```text
SP STAT-SIG GREEN charged CI-upper soft=0 major=0
```

## Native

```bash
mkdir -p baselines residual
curl -fsSL -o baselines/pdqsort.h https://raw.githubusercontent.com/orlp/pdqsort/master/pdqsort.h
curl -fsSL -o baselines/ska_sort.hpp https://raw.githubusercontent.com/skarupke/ska_sort/master/ska_sort.hpp
cp baselines/ska_sort.hpp residual/ska_sort.hpp

g++ -O3 -std=c++17 -DNDEBUG -DSECONDARY_PARITY \
  -I. -Iresidual -Ibaselines \
  -o suite_hybrid scripts/expanded_field_bench_hybrid.cpp

# Money-shot (matches multi-arch CI)
./suite_hybrid --n 1000000 --reps 3 --out results_sp.csv
python3 scripts/sp_money_shot_gate.py results_sp.csv

# Or formal R=11
./suite_hybrid --n 1000000 --reps 11 --out results_sp.csv --raw-out raw_sp.csv
python3 scripts/sp_stat_sig_gate.py raw_sp.csv --min-reps 11
```

Makefile shortcuts (from `reproduce/`):

```bash
make sp-money-shot    # R=3 charged soft-gate
make sp-stat-sig      # R=11 CI gate
```

## Charged surface (hard-fail)

sorted, reverse, almost_sorted, few_k4_dense, few_k16_dense, organpipe, sawtooth, push_middle, runs_noise, random, gaussianish, adversarial, db_pk, timestamps

**Excluded (documented residual, not dual-evidence claims):** equal_heavy, mixed_blocks

## How to compare

- **Compare ratios (`menu/pdq`), not absolute milliseconds.**
- Host noise is expected; soft-gate is the auditable surface.
- Reference locks: `docs/field_level/MULTI_ARCH_MONEY_SHOT_LOCKED.md`, `STAT_SIG_PROTOCOL.md`

## Non-claims

- Not a universal replacement for library pdq/ska/Timsort/std::sort outside the measured suite.
- Does not claim every residual soft on the full 23-pattern suite is closed.
- EXTERNAL-clean visible metrics only. No χ. No internal irreversible state.
- Absolute wall times are host-dependent.

**THE BEASTIE BOYZ**
