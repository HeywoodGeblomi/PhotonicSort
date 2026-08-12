# Multi-type Gap Map (i32)

**n=1000000** · reps=5 (median) · arch=local · post soft-spot kill

Ratios = Photonic / baseline (lower is better for Photonic).

| Pattern | vs std | vs pdq | vs ska | ok |
|---------|-------:|-------:|-------:|:--:|
| sorted | 0.069× | 0.832× | 0.041× | 1 |
| reverse | 0.121× | 0.560× | 0.064× | 1 |
| organpipe | 0.039× | 0.117× | 0.190× | 1 |
| sawtooth | 0.050× | 0.298× | 0.134× | 1 |
| almost_sorted | 0.100× | 0.199× | 0.059× | 1 |
| uniform | 0.322× | 0.702× | 1.123× | 1 |
| two_values | 0.019× | 0.178× | 0.060× | 1 |
| few_k16_wide | 0.197× | 0.890× | 0.578× | 1 |
| few_k16_dense | 0.157× | 0.819× | 0.363× | 1 |
| zipf_k16 | 0.057× | 0.313× | 0.154× | 1 |
| push_middle | 0.104× | 0.359× | 0.102× | 1 |
| db_pk | 1.073× | 1.590× | 0.481× | 1 |

**Geo-mean Photonic/std = 0.106× · Photonic/pdq = 0.440× · Photonic/ska = 0.165×**

Wave 2 multi-type. Not field-level. EXTERNAL-clean preference.

**THE BEASTIE BOYZ**
