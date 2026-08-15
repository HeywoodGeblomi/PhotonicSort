"""ctypes bind to libdual_evidence.so — B2 surface.
THE BEASTIE BOYZ / Blam · EXTERNAL-clean
"""
from __future__ import annotations
import ctypes
from pathlib import Path

_LIB = None
_ACTION = {0: "STAY", 1: "LEFT", 2: "RIGHT"}

def _load():
    global _LIB
    if _LIB is not None:
        return _LIB
    here = Path(__file__).resolve().parent
    for name in ("libdual_evidence.so", "libdual_evidence.dylib", "dual_evidence.dll"):
        p = here / name
        if p.exists():
            _LIB = ctypes.CDLL(str(p))
            break
    if _LIB is None:
        raise FileNotFoundError(
            "libdual_evidence.so not found — build with: make -C primitive/dual_evidence"
        )
    _LIB.de_sigma_amps.argtypes = [ctypes.POINTER(ctypes.c_float), ctypes.c_int]
    _LIB.de_sigma_amps.restype = ctypes.c_float
    _LIB.de_classical_choose.argtypes = [ctypes.c_float]*4
    _LIB.de_classical_choose.restype = ctypes.c_int
    _LIB.de_dual_choose.argtypes = [ctypes.c_float]*5
    _LIB.de_dual_choose.restype = ctypes.c_int
    _LIB.de_dual_confirm_choose.argtypes = [ctypes.c_float]*8
    _LIB.de_dual_confirm_choose.restype = ctypes.c_int
    _LIB.de_second_solid.argtypes = [ctypes.c_float]*3
    _LIB.de_second_solid.restype = ctypes.c_int
    return _LIB

def sigma_amps(amps) -> float:
    lib = _load()
    n = len(amps)
    arr = (ctypes.c_float * n)(*[float(x) for x in amps])
    return float(lib.de_sigma_amps(arr, n))

def classical_choose(L_rate, L_cv, R_rate, R_cv) -> str:
    return _ACTION[_load().de_classical_choose(float(L_rate), float(L_cv), float(R_rate), float(R_cv))]

def dual_choose(L_rate, L_sigma, R_rate, R_sigma, floor=0.05) -> str:
    return _ACTION[_load().de_dual_choose(
        float(L_rate), float(L_sigma), float(R_rate), float(R_sigma), float(floor))]

def second_solid(sigma_delta, expected_polarity=0.0, floor=0.005) -> bool:
    return bool(_load().de_second_solid(float(sigma_delta), float(expected_polarity), float(floor)))
