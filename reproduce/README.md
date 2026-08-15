# Independent Reproduction — PhotonicSort

**Wave 0** pure residual + **Field-Level** hybrid residual money-shot (`Dockerfile.sp`).

## Field-Level money-shot (hybrid residual, DEFAULT dual residual)

From the **repository root**:

```bash
docker build -f reproduce/Dockerfile.sp -t photonic-sp .
docker run --rm photonic-sp
```

Expected: `SP MONEY-SHOT GREEN charged soft=0 major=0 ok=1`

Formal R=11:

```bash
docker run --rm -e STAT_SIG=1 photonic-sp
```

### Windows path length

Git on Windows may fail clone under deep paths (`MAX_PATH`). Use a short target or enable long paths:

```powershell
git -c core.longpaths=true clone https://github.com/HeywoodGeblomi/PhotonicSort.git C:\ps
cd C:\ps
docker build -f reproduce/Dockerfile.sp -t photonic-sp .
docker run --rm photonic-sp
```

Optional persistent fix: `git config --global core.longpaths true`

## Quick start — Wave 0 (int64 pure residual)

```bash
docker build -f reproduce/Dockerfile -t photonic-reproduce .
docker run --rm photonic-reproduce
```

Native:

```bash
cd reproduce
mkdir -p baselines
curl -fsSL -o baselines/pdqsort.h https://raw.githubusercontent.com/orlp/pdqsort/master/pdqsort.h
curl -fsSL -o baselines/ska_sort.hpp https://raw.githubusercontent.com/skarupke/ska_sort/master/ska_sort.hpp
make
./harness --n 1000000 --reps 5 --out results --arch-tag local
```

## Quick start — Wave 2 multi-type (int32 / uint32)

```bash
cd reproduce
make harness_multitype
./harness_multitype --type i32 --n 1000000 --reps 5 --out results_i32
```

Or `make run-i32` / `make run-u32`.

## What is measured

| Harness | Types | Algorithm | Suite |
|---------|-------|-----------|-------|
| `Dockerfile.sp` / `suite_hybrid` | multi-type charged surface | hybrid residual (DEFAULT dual residual) | Field-Level money-shot |
| `harness` (Wave 0) | int64 | pure residual menu | Phase 0 field |
| `harness_multitype` (Wave 2) | int32 / uint32 | pure residual | core pattern family |

Protocol: warm-up + median of odd reps; full correctness verify every run. **Compare ratios, not absolute milliseconds.**

## Non-claims

- Hybrid money-shot is the Field-Level charged surface only (see `docs/field_level/FIELD_LEVEL_CLAIM_v0.5.md`).
- Pure residual alone does not claim unscoped soft=0 on residual-quality / HE territory.
- Not a universal replacement for library pdq/ska/Timsort/std::sort outside the measured suite.
- Host-dependent absolute times; ratios are the auditable surface.

**THE BEASTIE BOYZ**
