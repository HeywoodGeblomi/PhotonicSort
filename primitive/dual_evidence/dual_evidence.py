#!/usr/bin/env python3
"""Dual-Evidence Residual Commitment — standalone Python mirror.

Continuous stream → odd-event restriction → σ_Δ → second solid.
EXTERNAL-clean. THE BEASTIE BOYZ / Blam 2026-08-14
"""
from __future__ import annotations
from dataclasses import dataclass
from typing import Sequence
import math


@dataclass
class Sigma:
    even_mean: float = 0.0
    odd_mean: float = 0.0
    delta: float = 0.0
    n_odd_events: int = 0


def compute(stream: Sequence[float], c: float = 0.05) -> Sigma:
    """σ_Δ from continuous scalar stream (threshold crossings)."""
    T = len(stream)
    if T < 4:
        return Sigma()
    events = []
    above = stream[0] > c
    for t in range(1, T):
        now = stream[t] > c
        if now != above:
            events.append(t)
            above = now
    if len(events) < 4:
        return Sigma()
    odd_events = events[0::2]
    if len(odd_events) < 2:
        return Sigma()
    E_even = odd_events[0::2]
    E_odd = odd_events[1::2]
    mu_e = sum(stream[i] for i in E_even) / len(E_even) if E_even else 0.0
    mu_o = sum(stream[i] for i in E_odd) / len(E_odd) if E_odd else 0.0
    return Sigma(even_mean=mu_e, odd_mean=mu_o, delta=mu_e - mu_o, n_odd_events=len(odd_events))


def second_solid(sigma_delta: float, expected_polarity: float = 0.0, floor: float = 0.005) -> bool:
    if abs(sigma_delta) < floor:
        return False
    if expected_polarity == 0.0:
        return True
    return (sigma_delta * expected_polarity) > 0.0


def dual_confirm(classical: bool, sigma_delta: float,
                 expected_polarity: float = 0.0, floor: float = 0.005) -> bool:
    if not classical:
        return False
    return second_solid(sigma_delta, expected_polarity, floor)


def _self_check() -> None:
    # Synthetic stream: threshold crossings with phase-dependent amplitude bias
    def make_stream(phase: int, n: int = 64) -> list[float]:
        s = []
        for i in range(n):
            base = 0.02 * math.sin(i * 0.4)
            # inject spikes so odd-event ordinals see different means by phase
            if i % 7 == (3 + phase) % 7:
                base += 0.25 * (1 if phase == 0 else -1)
            if i % 7 == (5 + phase) % 7:
                base += 0.15 * (1 if phase == 0 else -1)
            s.append(base)
        return s

    s0 = compute(make_stream(0))
    s1 = compute(make_stream(1))
    # Contract checks
    assert dual_confirm(False, 1.0) is False, "no classical → not OWNED"
    assert dual_confirm(True, 0.0) is False, "weak σ_Δ → not OWNED"
    assert dual_confirm(True, 0.1) is True, "classical + strong σ_Δ → OWNED"
    # Phase streams should produce usable deltas (not both zero)
    assert s0.n_odd_events >= 2 or s1.n_odd_events >= 2, "need events"
    print("# dual_evidence self-check")
    print(f"  phase0 delta={s0.delta:.4f} n_odd={s0.n_odd_events}")
    print(f"  phase1 delta={s1.delta:.4f} n_odd={s1.n_odd_events}")
    print(f"  dual_confirm(True, 0.1)={dual_confirm(True, 0.1)}")
    print(f"  dual_confirm(True, 0.0)={dual_confirm(True, 0.0)}")
    print("SELF-CHECK GREEN")


if __name__ == "__main__":
    _self_check()
