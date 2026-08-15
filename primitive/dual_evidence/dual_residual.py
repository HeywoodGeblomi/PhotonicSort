"""dual_residual.py — High-level SP-aware residual API (upward integration).
Prefers dual_evidence_cpp → de_cpp → pure Python. EXTERNAL-clean. THE BEASTIE BOYZ / Blam
"""
from __future__ import annotations
from typing import Any, Dict, List, Optional, Sequence, Union
import math

_BACKEND = "python"
_de = None
_ACTION = {0: "STAY", 1: "LEFT", 2: "RIGHT"}
_TALENT = {0: "CHEAP", 1: "FULL_GENERIC", 2: "FULL_STRUCTURE", 3: "FULL_HE"}

try:
    import dual_evidence_cpp as _de  # type: ignore
    _BACKEND = "pybind"
except ImportError:
    try:
        import de_cpp as _de  # type: ignore
        _BACKEND = "ctypes"
    except ImportError:
        _de = None
        _BACKEND = "python"

def backend() -> str:
    return _BACKEND

def _py_dual_choose(L_rate, L_sigma, R_rate, R_sigma, floor=0.05) -> str:
    lo = abs(L_sigma) >= floor
    ro = abs(R_sigma) >= floor
    if ro and not lo: return "RIGHT"
    if lo and not ro: return "LEFT"
    if lo and ro: return "RIGHT" if abs(R_sigma) >= abs(L_sigma) else "LEFT"
    return "STAY"

def _py_sigma_amps(amps: Sequence[float]) -> float:
    n = len(amps)
    if n < 8: return 0.0
    odd = list(range(0, n, 2))
    if len(odd) < 4: return 0.0
    Ee, Eo = odd[0::2], odd[1::2]
    return sum(amps[i] for i in Ee) / len(Ee) - sum(amps[i] for i in Eo) / len(Eo)

def _py_residual_decide(stream, classical_threshold=0.08, sigma_floor=0.02, expected_polarity=0.0):
    T = len(stream)
    if T <= 0:
        return {"owned": False, "talent": "CHEAP", "sigma_delta": 0.0, "classical_score": 0.0}
    classical_score = sum(abs(x) for x in stream) / T
    sigma_delta = _py_sigma_amps(stream)
    classical = classical_score > classical_threshold
    owned = classical and abs(sigma_delta) >= sigma_floor
    if expected_polarity != 0.0 and owned:
        owned = (sigma_delta * expected_polarity) > 0
    if not owned: talent = "CHEAP"
    elif abs(sigma_delta) > 0.1: talent = "FULL_STRUCTURE"
    elif classical_score > 0.2: talent = "FULL_HE"
    else: talent = "FULL_GENERIC"
    return {"owned": owned, "talent": talent, "sigma_delta": float(sigma_delta), "classical_score": float(classical_score)}

def dual_choose(L_rate, L_sigma, R_rate, R_sigma, floor=0.05) -> str:
    if _BACKEND == "pybind":
        return _ACTION[int(_de.dual_choose(float(L_rate), float(L_sigma), float(R_rate), float(R_sigma), float(floor)))]
    if _BACKEND == "ctypes":
        return _de.dual_choose(float(L_rate), float(L_sigma), float(R_rate), float(R_sigma), float(floor))
    return _py_dual_choose(L_rate, L_sigma, R_rate, R_sigma, floor)

def residual_decide(stream, classical_threshold=0.08, sigma_floor=0.02, expected_polarity=0.0):
    try:
        import numpy as np
        if hasattr(stream, "astype"):
            stream = stream.astype(np.float32, copy=False)
    except ImportError:
        pass
    if _BACKEND == "pybind":
        try:
            import numpy as np
            arr = np.asarray(stream, dtype=np.float32)
            d = _de.residual_decide(arr, classical_threshold, sigma_floor, expected_polarity)
            return {"owned": bool(d["owned"]), "talent": _TALENT.get(int(d["talent"]), "CHEAP"),
                    "sigma_delta": float(d["sigma_delta"]), "classical_score": float(d["classical_score"])}
        except Exception:
            pass
    return _py_residual_decide(list(stream), classical_threshold, sigma_floor, expected_polarity)

def residual_decide_batch(streams, **kwargs):
    return [residual_decide(s, **kwargs) for s in streams]

def probe_to_stream(probe: Dict[str, float]):
    inv = float(probe.get("inv_ratio", probe.get("inversion_ratio", 0.5)))
    runs = float(probe.get("max_run", 0.0))
    dchg = float(probe.get("direction_changes", 0.0))
    sortedness = float(probe.get("sortedness", 1.0 - inv))
    n = 64
    stream = []
    for i in range(n):
        base = inv * (0.5 + 0.5 * math.sin(i * 0.3))
        if runs > 0 and (i % max(1, int(n / max(runs, 1)))) < 2:
            base *= 0.3
        if dchg > 0 and i % 7 == 0:
            base += 0.05 * dchg
        stream.append(base * (1.0 - 0.5 * sortedness))
    return stream

def select_residual_from_probe(probe: Dict[str, float], **kwargs):
    return residual_decide(probe_to_stream(probe), **kwargs)
