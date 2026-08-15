# FULL Bind B1+B2 — C API + ctypes

**Status:** **GREEN**  
**Date:** 2026-08-14  
**Squad:** THE BEASTIE BOYZ / Blam  
**EXTERNAL-clean.**

---

## Shipped

| Piece | Role |
|-------|------|
| `dual_evidence_c.h` / `.cpp` | C API export |
| `Makefile` | `make` → `libdual_evidence.so` |
| `de_cpp.py` | ctypes Python surface |

---

## Verification

| Check | Result |
|-------|--------|
| dual_choose bit-identical vs Python | **144/144** |
| classical_choose bit-identical | **GREEN** |
| C++ dual death-match via ctypes | **goals 40/40 · wrong 0** |
| classical death-match (control) | goals 0/40 · wrong 20 |

---

## Build + use

```bash
cd primitive/dual_evidence
make
python3 -c "import de_cpp; print(de_cpp.dual_choose(50, 0.0, 200, 0.2))"
# RIGHT
```

---

## Next (B3/B4)

- B3: optional PhotonicSort residual menu on FULL residual
- B4: pybind11 `dual_evidence_cpp` module

**One second solid. C++ in the loop.**
