# Dual-Evidence Residual Commitment

**Standalone EXTERNAL-clean primitive.**  
Odd-restricted contrast (Secondary Parity Signature) as a **second solid** before irreversible residual choice.

PhotonicSort’s `residual/secondary_parity.hpp` is the sorting-domain instance.  
This package is the domain-agnostic API.

THE BEASTIE BOYZ / Blam · 2026-08-14

---

## Contract

```
continuous observable stream
    → threshold / sign-change events
    → restrict to odd-indexed events
    → partition by ordinal parity
    → σ_Δ = mean(even-ordinal) − mean(odd-ordinal)

residual commit iff classical_evidence AND second_solid(σ_Δ)
```

| State | Meaning |
|-------|---------|
| classical only | PREPARED — abatable |
| classical + strong σ_Δ | OWNED — commit residual talent |
| weak / missing σ_Δ | no second solid — abate |

Visible metrics only. No χ. No internal irreversible state.

---

## Files

| File | Role |
|------|------|
| `dual_evidence.hpp` | C++17 header (stream → Sigma → dual_confirm) |
| `dual_evidence.py` | Python mirror + self-check |
| `README.md` | this file |

---

## Minimal use (C++)

```cpp
#include "dual_evidence.hpp"

float stream[T]; // continuous disorder / polarity / any scalar probe
// ... fill stream ...
auto s = dual_evidence::compute(stream, T, /*threshold*/ 0.05f);
bool classical = /* your primary evidence */;
if (dual_evidence::dual_confirm(classical, s.delta)) {
    // OWNED — commit irreversible residual choice
} else {
    // abate — safe residual / defer
}
```

## Minimal use (Python)

```python
from dual_evidence import compute, dual_confirm
s = compute(stream, c=0.05)
if dual_confirm(classical, s.delta):
    commit()
```

---

## Self-check

```bash
python3 dual_evidence.py
# expects: SELF-CHECK GREEN
```

---

## Non-claims

- Not a sorting algorithm.
- Not a claim that every domain benefits from odd-restricted contrast.
- Not χ. EXTERNAL-clean only.
- Orthogonality to classical summaries is stated for sorting probe streams (see `docs/field_level/ORTHOGONALITY_THEOREM.md`); other domains must re-verify separation.

---

## Relation to PhotonicSort

Field-Level Claim v0.5 uses this dual-evidence rule inside hybrid residual selection (`-DSECONDARY_PARITY`).  
This package exists so the **technique** can travel without the sorter.
