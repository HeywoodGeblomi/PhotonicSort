# Phase 0 Gap Map (u32)

**n=1000000** · reps=5 (median) · patterns=18

Ratios = Photonic / baseline (lower is better for Photonic).

| Pattern | vs std | vs pdq | vs ska | ok |
|---------|-------:|-------:|-------:|:--:|
| sorted | 0.075× | 0.989× | 0.064× | 1 |
| reverse | 0.114× | 0.542× | 0.070× | 1 |
| organpipe | 0.031× | 0.102× | 0.167× | 1 |
| sawtooth | 0.085× | 0.526× | 0.209× | 1 |
| almost_sorted | 0.109× | 0.283× | 0.078× | 1 |
| uniform | 0.247× | 0.667× | 0.919× | 1 |
| two_values | 0.096× | 0.621× | 0.078× | 1 |
| few_k16_dense | 0.331× | 1.625× | 0.669× | 1 |
| few_k16_wide | 0.286× | 1.188× | 0.758× | 1 |
| zipf_k16 | 0.080× | 0.425× | 0.180× | 1 |
| push_middle | 0.170× | 0.678× | 0.199× | 1 |
| db_pk | 0.795× | 1.829× | 0.508× | 1 |
| timestamps | 0.135× | 1.595× | 0.093× | 1 |
| equal_heavy | 0.184× | 1.612× | 0.149× | 1 |
| adversarial_pipe | 0.039× | 0.090× | 0.144× | 1 |
| reverse_segments_8 | 0.027× | 0.152× | 0.119× | 1 |
| nearly_sorted_blocks | 0.042× | 0.074× | 0.104× | 1 |
| gauss_mix | 0.207× | 0.849× | 1.111× | 1 |

**Geo-mean Photonic/std = 0.116× · Photonic/pdq = 0.519× · Photonic/ska = 0.198×**

Phase 0 path-(a) evidence. EXTERNAL-clean. **Not field-level.**

**THE BEASTIE BOYZ**
