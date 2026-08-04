# PhotonicSort C 1.1.0-c — Geblomi residual menu

Brand contract unchanged: **probe → structure early-exit → residual talent menu**.

## Infusions from GeblomiSort

1. **Joint merge gate** — `max_run ∧ low direction-change rate` (SHAP conjunction).
2. **Capacity-checked run merge** — natural runs, ≤8 tracked; half-buffer merge; overflow → other residual.
3. **pdqsort-class introsort** — median-of-three / ninther, depth-limited, heapsort fallback.
4. **LSD radix (int64)** — order-preserving sign-bit transform; preferred on random scalars.
5. **Equal-heavy bias** — high equal ratio lowers effective inversion and biases routing.
6. **Confidence routing** — soft preference between radix vs introsort; low confidence → introsort.

`photonic_sort_i64_force_collapse` remains **stable mergesort**.

## API

Compatible with 1.0.1-c. Version string: `1.1.0-c`.

## Verify

```bash
cd c && make test
```
