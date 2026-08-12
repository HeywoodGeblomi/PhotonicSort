# Independent Reproduction — PhotonicSort Pure Residual

**Wave 0** of the Barrier Break Plan (int64 residual floors).  
**Wave 2** multi-type harness is additive (int32 / uint32).

## Quick start — Wave 0 (int64, independent reproduction)

From the **repository root**:

```bash
docker build -f reproduce/Dockerfile -t photonic-reproduce .
docker run --rm photonic-reproduce
```

Native:

```bash
cd reproduce
# fetch baselines once if missing
mkdir -p baselines
curl -fsSL -o baselines/pdqsort.h https://raw.githubusercontent.com/orlp/pdqsort/master/pdqsort.h
curl -fsSL -o baselines/ska_sort.hpp https://raw.githubusercontent.com/skarupke/ska_sort/master/ska_sort.hpp
make
./harness --n 1000000 --reps 5 --out results --arch-tag local
```

Reference ratios: `docs/phase0_field/PHASE1_LOW_DISORDER.md` and `docs/WAVE1_RESIDUAL_FLOORS.md`.

## Quick start — Wave 2 multi-type (int32 / uint32)

```bash
cd reproduce
make harness_multitype
./harness_multitype --type i32 --n 1000000 --reps 5 --out results_i32
# later: --type u32
```

Or `make run-i32` / `make run-u32`.

Uses `pure_residual::sort_i32` / `sort_u32`. Wave 0 int64 harness is completely untouched.

## What is measured

| Harness | Types | Algorithm | Suite |
|---------|-------|-----------|-------|
| `harness` (Wave 0) | int64 | pure residual menu | 18-pattern Phase 0 field |
| `harness_multitype` (Wave 2) | int32 / uint32 | pure residual (expanding) | core pattern family |

Protocol: warm-up + median of odd reps; full correctness verify every run.

## How to compare

- **Compare ratios, not absolute milliseconds.**
- Soft spots and strengths should align pattern-for-pattern within noise.

## Non-claims

- Not field-level.
- Not a universal replacement for library pdq/ska/Timsort/std::sort.
- Multi-type surface under construction; until measured, primary claims remain sequential-int64 specialized.
- Host-dependent absolute times; ratios are the auditable surface.
- Protecting i64 residual excellence is non-negotiable.

**THE BEASTIE BOYZ**
