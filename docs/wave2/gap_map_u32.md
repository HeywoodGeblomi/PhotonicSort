# Multi-type Gap Map (u32)

**n=1000000** · reps=5 (median) · arch=local · post soft-spot kill

Ratios = Photonic / baseline (lower is better for Photonic).

| Pattern | vs std | vs pdq | vs ska | ok |
|---------|-------:|-------:|-------:|:--:|
| sorted | 0.067× | 1.046× | 0.047× | 1 |
| reverse | 0.103× | 0.430× | 0.052× | 1 |
| organpipe | 0.042× | 0.122× | 0.220× | 1 |
| sawtooth | 0.054× | 0.378× | 0.118× | 1 |
| almost_sorted | 0.114× | 0.287× | 0.074× | 1 |
| uniform | 0.247× | 0.596× | 0.858× | 1 |
| two_values | 0.021× | 0.268× | 0.059× | 1 |
| few_k16_wide | 0.197× | 0.940× | 0.553× | 1 |
| few_k16_dense | 0.154× | 0.687× | 0.339× | 1 |
| zipf_k16 | 0.063× | 0.374× | 0.106× | 1 |
| push_middle | 0.113× | 0.493× | 0.117× | 1 |
| db_pk | 1.253× | 2.163× | 0.562× | 1 |

**Geo-mean Photonic/std = 0.108× · Photonic/pdq = 0.500× · Photonic/ska = 0.161×**

Wave 2 multi-type. Not field-level. EXTERNAL-clean preference.

**THE BEASTIE BOYZ**
