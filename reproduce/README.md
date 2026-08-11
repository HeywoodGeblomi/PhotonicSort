# Independent Reproduction — PhotonicSort Pure Residual

**Wave 0** of the Barrier Break Plan.  
**Goal:** third party runs one command and gets a gap map comparable to the published reference.

## Quick start (Docker)

```bash
cd reproduce
docker build -t photonic-reproduce .
docker run --rm photonic-reproduce
```

Output: `results/gap_map.md` and `results/gap_map.csv` printed / written inside the container.

To keep results on the host:

```bash
mkdir -p out
docker run --rm -v "$PWD/out:/out" photonic-reproduce \
  sh -c "./harness --n 1000000 --reps 5 --out /out --arch-tag myhost"
```

## Quick start (native)

```bash
cd reproduce
make
./harness --n 1000000 --reps 5 --out results --arch-tag local
```

Requires: `g++` with C++17, Linux/macOS.

## What is measured

| Item | Detail |
|------|--------|
| Algorithm | Pure residual menu (`residual/pure_residual_menu.hpp`) post Phase 1 |
| Baselines | `std::sort`, pdqsort (vendored), ska_sort (vendored) |
| Suite | 18-pattern Phase 0 field standard suite @ n=1e6 |
| Protocol | Warm-up + median of odd reps; full correctness verify every run |

## How to compare

- **Compare ratios, not absolute milliseconds.** Absolute times vary by host, compiler, and load.
- Reference ratios: `docs/phase0_field/PHASE1_LOW_DISORDER.md` and gap maps under `docs/phase0_field/`.
- Geo-mean Photonic/pdq near **0.80×** (post Phase 1) is the expected band; soft spots and strengths should align pattern-for-pattern within noise.

## Non-claims

- Not field-level.
- Not a universal replacement for library pdq/ska/Timsort/std::sort.
- Sequential int64 only in this harness.
- Host-dependent absolute times; ratios are the auditable surface.

**THE BEASTIE BOYZ**
