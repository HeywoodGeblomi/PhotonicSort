# PhotonicSort C 1.3.2-c

**Opt-in Aggressive / ForceHole modes** — NORMAL remains default.

## What changed

- `photonic_sort_mode_t`: `NORMAL` | `AGGRESSIVE` | `FORCE_HOLE`
- `photonic_sort_i64_ex(a, n, mode)` / `photonic_probe_i64_ex(..., mode)`
- `photonic_sort_set_mode` / `get_mode` / `mode_name`
- Mode-aware residual thresholds (LOW_DISORDER, LOW_CARD, PATTERNED, RANDOM abort)
- ForceHole ladder: structure (verified) → low-card → relaxed low-disorder → residual
- **STRUCTURE O(n) verify remains mandatory on every mode**

## Measured mode panel (n=100k, 24 trials/cell)

Generator: sorted + exact *k* pairwise swaps (sparse disorder).  
Correctness: **24/0 every cell**.

### STRUCTURE / hole-in-one rate (path code 1)

| swap frac | NORMAL | AGGRESSIVE | FORCE_HOLE |
|-----------|-------:|-----------:|-----------:|
| 0.0000 (pure sorted) | **100%** | **100%** | **100%** |
| 0.0005 → 0.1200 | **0%** | **0%** | **0%** |
| pure random | **0%** | **0%** | **0%** |

**STRUCTURE hole-in-one rate is unchanged by mode.** Once any swaps exist, O(n) verify rejects STRUCTURE for all three modes.

### Residual wall time (median ms) — where modes differ

| swap frac | k | NORMAL | AGGRESSIVE | FORCE_HOLE |
|-----------|--:|-------:|-----------:|-----------:|
| 0.0005 | 50 | 0.276 | **0.246** | 0.766 |
| 0.0010 | 100 | 0.742 | **0.254** | 0.773 |
| 0.0020 | 200 | 0.775 | 0.764 | 0.784 |
| 0.0400 | 4000 | **0.292** | 1.578 | 1.597 |
| 0.0800 | 8000 | **0.263** | 0.282 | 2.277 |
| 0.1200 | 12000 | 0.260 | 0.259 | 0.281 |
| random | — | 1.82 | 1.81 | 1.79 |

### Reading

- **AGGRESSIVE** wins the light almost-sorted band (≈0.05–0.1% swaps) via more frequent LOW_DISORDER residual.
- **FORCE_HOLE** can pay a ladder tax on mid-band disorder (4–8% swaps) before falling through.
- Pure random / heavy disorder: modes essentially equal.
- Modes change **residual selection and timing**, not STRUCTURE hit rate.

Harness: `c/tests/sensitivity_modes.c` (reproducible).

## Non-claims

- Default is still NORMAL (safety-first).
- ForceHole is opt-in only.
- Does **not** increase STRUCTURE hole-in-one rate on sparse-swap data.
- Does not claim hole-in-one on every input.
- Worst case remains O(n log n).

## Usage

```c
photonic_sort_i64(a, n);  /* NORMAL */
photonic_sort_i64_ex(a, n, PHOTONIC_MODE_AGGRESSIVE);
photonic_sort_i64_ex(a, n, PHOTONIC_MODE_FORCE_HOLE);
```

## Contributors

**Heywood Geblomi** · Grok (xAI) collaboration
