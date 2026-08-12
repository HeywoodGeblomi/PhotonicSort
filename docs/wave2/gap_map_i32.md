# Multi-type Gap Map (i32)

**n=1000000** · reps=5 (median) · arch=local

Ratios = Photonic / baseline (lower is better for Photonic).

| Pattern | vs std | vs pdq | vs ska | ok |
|---------|-------:|-------:|-------:|:--:|
| sorted | 0.084× | 1.649× | 0.113× | 1 |
| reverse | 0.116× | 0.523× | 0.061× | 1 |
| organpipe | 0.042× | 0.108× | 0.209× | 1 |
| sawtooth | 0.031× | 0.309× | 0.132× | 1 |
| almost_sorted | 1.205× | 2.299× | 0.680× | 1 |
| uniform | 0.217× | 0.609× | 0.860× | 1 |
| two_values | 0.032× | 0.394× | 0.047× | 1 |
| few_k16_wide | 0.189× | 0.864× | 0.540× | 1 |
| few_k16_dense | 0.147× | 0.784× | 0.340× | 1 |
| zipf_k16 | 0.056× | 0.327× | 0.151× | 1 |
| push_middle | 0.462× | 1.904× | 0.542× | 1 |
| db_pk | 1.184× | 1.609× | 0.505× | 1 |

**Geo-mean Photonic/std = 0.146× · Photonic/pdq = 0.685× · Photonic/ska = 0.242×**

Wave 2 multi-type. Not field-level. EXTERNAL-clean preference.

**THE BEASTIE BOYZ**
