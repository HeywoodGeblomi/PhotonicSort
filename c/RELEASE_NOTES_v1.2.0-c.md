# PhotonicSort C 1.2.0-c — GyroRank pilot gate

Offsets the **random-data probe overhead** weakness by integrating the
GyroRank observe → gate pattern into the C probe.

## Problem

On pure random input, a full structure probe is wasted work: the residual
path was already going to radix/introsort. Headline gap vs `std::sort`
(~105 ms vs ~78 ms @ 1e6) was partly probe cost, not residual quality.

## Fix (GyroRank integration)

1. **Pilot sample** — first min(256, n/8) consecutive pairs.
2. **Gate RANDOM early** when `inv_ratio ≥ 0.42`, direction-change rate ≥ 0.28,
   and equal ratio < 0.20 → set `pilot_aborted=1`, skip full probe.
3. **Residual** — pilot-aborted path goes straight to **LSD radix** (fallback pdq).
4. Structure / patterned paths unchanged (sorted, reverse, run-merge).

## Probe additions

| Field | Meaning |
|-------|---------|
| `route` | `STRUCTURE` / `PATTERNED` / `RANDOM` |
| `pilot_aborted` | 1 if full probe short-circuited |
| `pilot_samples` | pairs observed before gate |

## Verify

```bash
cd c && make test
```

Companion ranking kernel (multi-objective): [GyroRank](https://github.com/HeywoodGeblomi/GyroRank).
