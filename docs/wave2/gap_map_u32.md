# Multi-type Gap Map (u32)

**n=1000000** · reps=5 (median) · arch=local

Ratios = Photonic / baseline (lower is better for Photonic).

| Pattern | vs std | vs pdq | vs ska | ok |
|---------|-------:|-------:|-------:|:--:|
| sorted | 0.112× | 0.782× | 0.120× | 1 |
| reverse | 0.094× | 0.483× | 0.050× | 1 |
| organpipe | 0.041× | 0.117× | 0.209× | 1 |
| sawtooth | 0.055× | 0.334× | 0.131× | 1 |
| almost_sorted | 1.067× | 2.276× | 0.600× | 1 |
| uniform | 0.237× | 0.618× | 0.670× | 1 |
| two_values | 0.035× | 0.403× | 0.060× | 1 |
| few_k16_wide | 0.190× | 0.836× | 0.529× | 1 |
| few_k16_dense | 0.161× | 0.616× | 0.343× | 1 |
| zipf_k16 | 0.066× | 0.358× | 0.116× | 1 |
| push_middle | 0.551× | 2.398× | 0.571× | 1 |
| db_pk | 1.111× | 1.872× | 0.510× | 1 |

**Geo-mean Photonic/std = 0.159× · Photonic/pdq = 0.661× · Photonic/ska = 0.232×**

Wave 2 multi-type. Not field-level. EXTERNAL-clean preference.

**THE BEASTIE BOYZ**
