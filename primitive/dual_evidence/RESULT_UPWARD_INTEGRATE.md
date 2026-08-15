# Upward Dual-Evidence Integration

**Status:** **GREEN**  
**Date:** 2026-08-14  
**Squad:** THE BEASTIE BOYZ / Blam  
**EXTERNAL-clean.**

---

## Track 1 — Main Python API

`dual_residual.py` — users call this, not the primitive by hand.

```python
from dual_residual import residual_decide, dual_choose, backend
print(backend())  # pybind | ctypes | python
d = residual_decide(stream)
# {'owned', 'talent', 'sigma_delta', 'classical_score'}
```

Fallback chain: **pybind → ctypes → pure Python**.

`photonic_sort.photonic_residual_talent(arr)` — soft-wire from probe metrics.

## Track 2 — Real usage demos

```bash
python3 demo_talent_patterns.py
# structured     FULL_STRUCTURE ~17/40
# high_entropy   FULL_HE        ~32/40
# cheap          CHEAP          40/40

python3 harness_bind_perf.py
```

## Non-claims

Does not change Field-Level charged soft-gate numbers. Sort path still classical Timsort/collapse. Talent is advisory/annotation until residual menu is fully driven by it in production C++ residual.

**Foundation:** `make all && make test` then `import dual_residual`.
