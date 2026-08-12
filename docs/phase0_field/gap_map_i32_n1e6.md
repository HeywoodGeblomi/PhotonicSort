# Phase 0 Gap Map (i32)

**n=1000000** · reps=5 (median) · patterns=18

Ratios = Photonic / baseline (lower is better for Photonic).

| Pattern | vs std | vs pdq | vs ska | ok |
|---------|-------:|-------:|-------:|:--:|
| sorted | 0.102× | 1.119× | 0.067× | 1 |
| reverse | 0.153× | 0.656× | 0.080× | 1 |
| organpipe | 0.037× | 0.102× | 0.187× | 1 |
| sawtooth | 0.062× | 0.319× | 0.156× | 1 |
| almost_sorted | 0.125× | 0.255× | 0.074× | 1 |
| uniform | 0.242× | 0.679× | 0.805× | 1 |
| two_values | 0.082× | 0.523× | 0.086× | 1 |
| few_k16_dense | 0.113× | 0.756× | 0.345× | 1 |
| few_k16_wide | 0.269× | 1.055× | 0.763× | 1 |
| zipf_k16 | 0.074× | 0.352× | 0.176× | 1 |
| push_middle | 0.122× | 0.465× | 0.125× | 1 |
| db_pk | 0.832× | 1.729× | 0.528× | 1 |
| timestamps | 0.142× | 1.562× | 0.092× | 1 |
| equal_heavy | 0.163× | 1.493× | 0.136× | 1 |
| adversarial_pipe | 0.041× | 0.089× | 0.118× | 1 |
| reverse_segments_8 | 0.028× | 0.136× | 0.111× | 1 |
| nearly_sorted_blocks | 0.043× | 0.064× | 0.097× | 1 |
| gauss_mix | 0.281× | 1.056× | 1.535× | 1 |

**Geo-mean Photonic/std = 0.111× · Photonic/pdq = 0.462× · Photonic/ska = 0.184×**

Phase 0 path-(a) evidence. EXTERNAL-clean. **Not field-level.**

**THE BEASTIE BOYZ**
