# Wave 0 — Independent Reproduction

**Status:** Infrastructure live on `reproduce/`  
**Parent:** [Barrier Break Plan](../BARRIER_BREAK_PLAN.md)

One-command Docker harness measures the pure residual menu (post Phase 1 low_disorder) against `std::sort`, vendored pdqsort, and vendored ska_sort on the 18-pattern Phase 0 field suite.

```bash
# from repository root
docker build -f reproduce/Dockerfile -t photonic-reproduce .
docker run --rm photonic-reproduce
```

Native: `cd reproduce && make && ./harness --n 1000000 --reps 5 --out results`.

Reference ratios: [PHASE1_LOW_DISORDER.md](./PHASE1_LOW_DISORDER.md) (db_pk ≈1.12×, timestamps ≈0.43×, geo ≈0.80× vs pdq).

Compare **ratios**, not absolute milliseconds. EXTERNAL-clean. Not field-level.

**THE BEASTIE BOYZ**
