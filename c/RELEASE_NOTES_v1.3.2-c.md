# PhotonicSort C 1.3.2-c

**Opt-in Aggressive / ForceHole modes** — NORMAL remains default.

## What changed

- `photonic_sort_mode_t`: `NORMAL` | `AGGRESSIVE` | `FORCE_HOLE`
- `photonic_sort_i64_ex(a, n, mode)` / `photonic_probe_i64_ex(..., mode)`
- `photonic_sort_set_mode` / `get_mode` / `mode_name`
- Mode-aware residual thresholds (LOW_DISORDER, LOW_CARD, PATTERNED, RANDOM abort)
- ForceHole ladder: structure (verified) → low-card → relaxed low-disorder → residual
- **STRUCTURE O(n) verify remains mandatory on every mode**

## Measured (n=200k, 57 cases)

- Correctness: **0 fails**
- AGGRESSIVE raises LOW_DISORDER hit rate on almost-sorted 0.04–0.08 swap band
- Wall time on that band can still favor NORMAL radix; hole-in-one *rate* ≠ always faster
- Pure random / sorted / reverse / sawtooth / few_unique unchanged in route intent

## Non-claims

- Default is still NORMAL (safety-first).
- ForceHole is opt-in only.
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
