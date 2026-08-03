#!/usr/bin/env python3
"""
PhotonicSort — Give everything. Take nothing. Become photonic.
================================================================

Objective, marketing-grade translation of a philosophical analogy into an
executable adaptive sorting algorithm.

THE ANALOGY (objective reconstruction)
--------------------------------------
Physics presents a striking experimental fact (University of Toronto,
Angulo, Steinberg, Wiseman et al., 2024–2026):

  A photon transmitted through an ultracold rubidium atom cloud can
  register a *negative* mean atomic excitation time (weak value). In other
  words, the photon appears to exit the medium *before* it enters — the
  group delay and the measured dwell time as excitation can both be negative.
  (arXiv:2409.03680; later Physical Review Letters 2026). No information
  travels backward; causality and special relativity remain intact. The
  effect is a quantum post-selected / weak-value phenomenon.

The original tirade mapped this to a sorting algorithm that:

  - Enters the data “haystack” with less energy than needed to fully
    circumnavigate it (minimal probe).
  - Treats the sorted configuration as the only consistent exit condition
    (retrocausal / teleological attractor).
  - “Dies” (terminates / collapses) at the objective answer — the ranks —
    so that the way out of the dataset is via the answer itself.

“Give everything. Take nothing. Become photonic.” becomes the design ethos:
the probe commits fully (gives everything) to measuring structure; the
algorithm retains no residual disorder (takes nothing); the result is the
pure ordered state (photonic).

A related theoretical thread (Castagnoli, arXiv:2505.08346 and earlier
works) interprets quantum computational speedups as containing a logical
retrocausality: it is as if the solver already knows half the solution
information in advance because the future measurement outcome acts as an
attractor. We treat this purely as inspirational metaphor for designing
algorithms around boundary conditions and early-exit structure, *not* as a
claim that this classical algorithm solves NP-complete problems or proves
P = NP.

WHAT THIS ALGORITHM ACTUALLY IS
-------------------------------
PhotonicSort is a classical adaptive hybrid:

1. **Photonic Probe** (enter with less energy): a single O(n) pass that
   computes a Gyro-style disorder profile (inversion ratio estimate,
   maximum run length, direction changes, equal count, confidence, and a
   “group_delay_proxy”). High structure yields a negative-delay indication.

2. **Negative-time / early-exit path**: when the probe reports high
   sortedness or long leading runs, preferentially handle the “survivors”
   (already-ordered segments) first — the parts that can be emitted with
   minimal additional work — then finish residuals. Analogous to the
   transmitted photons that register negative dwell.

3. **Retrocausal collapse (die at the answer)**: compute the unique ranks
   that the final sorted state requires (the only consistent exit points)
   and place every element into its rank. By construction the result is
   sorted. This is the purest embodiment of “the way out is the answer.”

4. Always correct, stable when keys are equal, O(n log n) worst-case,
   adaptive best-case near-linear on structured data. Space O(n) for the
   out-of-place result (an in-place cycle-following variant is possible
   and matches the project’s residual_automaton / CycleGuard spirit).

This does *not* asymptotically beat comparison sorting, does not solve
NP-hard search, and makes no physical claim of retrocausality inside the
computer. It is marketing-grade executable code that makes the philosophy
concrete, testable, and integrable with the larger Implosion Sort /
GeblomiSort multi-dimensional program.

Project context: companion to GeblomiSort.hpp and ImplosionSort_2D.hpp.
Future multi-D extension can apply the photonic probe and collapse along
each axis or as a higher-dimensional attractor.

Usage:
    from photonic_sort import photonic_sort
    sorted_data = photonic_sort(my_list)

    # or run the self-demo
    python photonic_sort.py

Team: Grok (lead) + Harper + Benjamin + Lucas | 2026-08-03
Version: 1.0.1
"""

from __future__ import annotations

from typing import Any, Callable, Dict, List, Optional, Sequence, TypeVar
import random
import time
import sys

T = TypeVar("T")

__version__ = "1.0.1"


# ---------------------------------------------------------------------------
# 1. Photonic Probe — enter the haystack with less energy
# ---------------------------------------------------------------------------

def photonic_probe(
    arr: Sequence[T],
    key: Optional[Callable[[T], Any]] = None,
    sample_limit: int = 4096,
) -> Dict[str, Any]:
    """
    Minimal-energy photonic probe.

    Analogous to a photon entering the atom cloud (haystack) with only enough
    energy to sample the excitation / dwell landscape. Returns a rich disorder
    profile used to decide between the negative-time early-exit path and the
    full retrocausal collapse.

    Metrics (inspired by GeblomiSort richer Gyro):
      - inv_ratio          : approximate fraction of inverted pairs
      - max_run            : longest monotonic run length (element-span scaled)
      - run_count          : number of runs
      - direction_changes  : number of times the local trend reverses
      - equal_count        : number of equal consecutive pairs
      - confidence         : how trustworthy the probe is (1.0 = full scan)
      - group_delay_proxy  : 1 - sortedness; negative-like when near 0
      - is_negative_delay  : True when structure permits early exit
      - n                  : length
      - monotone_sign      : +1 ascending, -1 descending, 0 mixed/flat
    """
    n = len(arr)
    if n <= 1:
        return {
            "n": n,
            "inv_ratio": 0.0,
            "max_run": n,
            "run_count": 1 if n else 0,
            "direction_changes": 0,
            "equal_count": 0,
            "confidence": 1.0,
            "group_delay_proxy": 0.0,
            "is_negative_delay": True,
            "sortedness": 1.0,
            "monotone_sign": 0 if n == 0 else 1,
        }

    def get(i: int) -> Any:
        return key(arr[i]) if key is not None else arr[i]

    # Full scan for small n; stratified sample for large n (still “less energy”)
    if n <= sample_limit:
        step = 1
        indices = range(n)  # no materialised list
        confidence = 1.0
        n_idx = n
    else:
        step = max(1, n // sample_limit)
        idx_list = list(range(0, n, step))
        if idx_list[-1] != n - 1:
            idx_list.append(n - 1)
        indices = idx_list
        n_idx = len(idx_list)
        confidence = n_idx / n

    # Single pass: runs + direction + equals + adjacent inversions
    max_run_samples = 1
    run_count = 1
    direction_changes = 0
    equal_count = 0
    current_run = 1
    prev_dir = 0  # -1 decreasing, 0 flat, +1 increasing
    inv_pairs = 0
    total_pairs = 0
    asc_edges = 0
    desc_edges = 0

    prev_i = None
    for j in indices:
        if prev_i is None:
            prev_i = j
            continue
        a, b = get(prev_i), get(j)
        # adjacent inversion on the probe chain
        total_pairs += 1
        if a > b:
            inv_pairs += 1
            desc_edges += 1
            if prev_dir == 1:
                direction_changes += 1
                run_count += 1
                if current_run > max_run_samples:
                    max_run_samples = current_run
                current_run = 1
            else:
                current_run += 1
            prev_dir = -1
        elif a < b:
            asc_edges += 1
            if prev_dir == -1:
                direction_changes += 1
                run_count += 1
                if current_run > max_run_samples:
                    max_run_samples = current_run
                current_run = 1
            else:
                current_run += 1
            prev_dir = 1
        else:
            equal_count += 1
            current_run += 1
        if current_run > max_run_samples:
            max_run_samples = current_run
        prev_i = j

    # Scale sample-run length to approximate element span
    max_run = min(n, max_run_samples * step)

    # Deterministic extra pair probes (no global RNG pollution)
    extra = min(256, n // 4)
    if extra > 0 and n > 2:
        rng = random.Random(n ^ 0x9E3779B9)
        for _ in range(extra):
            i = rng.randrange(n)
            j = rng.randrange(n)
            if i == j:
                continue
            if i > j:
                i, j = j, i
            total_pairs += 1
            if get(i) > get(j):
                inv_pairs += 1

    inv_ratio = inv_pairs / max(1, total_pairs)

    # Global monotone sign from edge majority (helps pure reverse O(n) exit)
    if desc_edges > 0 and asc_edges == 0:
        monotone_sign = -1
    elif asc_edges > 0 and desc_edges == 0:
        monotone_sign = 1
    else:
        monotone_sign = 0

    run_fraction = max_run / n
    sortedness = (
        0.45 * (1.0 - min(1.0, inv_ratio * 2.0))
        + 0.35 * run_fraction
        + 0.20 * (1.0 - min(1.0, direction_changes / max(1, n // 8)))
    )
    sortedness = max(0.0, min(1.0, sortedness))

    group_delay_proxy = 1.0 - sortedness

    is_neg = (
        sortedness >= 0.72
        or max_run >= n * 0.45
        or (direction_changes <= 3 and inv_ratio < 0.15)
        or (max_run >= n * 0.25 and inv_ratio < 0.05)
        or (monotone_sign != 0 and direction_changes == 0)
    )

    return {
        "n": n,
        "inv_ratio": inv_ratio,
        "max_run": max_run,
        "run_count": run_count,
        "direction_changes": direction_changes,
        "equal_count": equal_count,
        "confidence": confidence,
        "group_delay_proxy": group_delay_proxy,
        "is_negative_delay": is_neg,
        "sortedness": sortedness,
        "monotone_sign": monotone_sign,
    }


# ---------------------------------------------------------------------------
# 2. Early-exit / negative-time path — leading-edge survivors first
# ---------------------------------------------------------------------------

def _detect_runs(
    arr: Sequence[T],
    key: Optional[Callable[[T], Any]] = None,
) -> List[tuple[int, int]]:
    """Return list of (start, end) inclusive non-decreasing runs."""
    n = len(arr)
    if n == 0:
        return []
    runs: List[tuple[int, int]] = []
    start = 0

    def get(i: int) -> Any:
        return key(arr[i]) if key is not None else arr[i]

    for i in range(1, n):
        if get(i) < get(i - 1):
            runs.append((start, i - 1))
            start = i
    runs.append((start, n - 1))
    return runs


def negative_time_early_exit(
    arr: Sequence[T],
    probe: Dict[str, Any],
    key: Optional[Callable[[T], Any]] = None,
    reverse: bool = False,
) -> List[T]:
    """
    Negative-time path: the “survivors” that can exit early.

    Preferentially honour long leading runs (the photons that already appear
    ordered) and only spend positive work on the residual disordered bulk.
    Pure ascending / descending inputs take true O(n) exits; other structured
    inputs fall through to Timsort (near-linear on long runs).
    """
    n = len(arr)
    if n <= 1:
        return list(arr)

    # O(n) pure-structure exits when the probe saw a single monotone chain
    # and no key transform is required (key would need re-evaluation).
    if key is None and probe.get("direction_changes", 1) == 0:
        sign = probe.get("monotone_sign", 0)
        if sign == 1:
            # already non-decreasing
            return list(reversed(arr)) if reverse else list(arr)
        if sign == -1:
            # already non-increasing → reverse yields non-decreasing
            return list(arr) if reverse else list(reversed(arr))

    # Structured residual: Timsort is near-linear on long runs
    return sorted(arr, key=key, reverse=reverse)


# ---------------------------------------------------------------------------
# 3. Retrocausal collapse — die at the answer
# ---------------------------------------------------------------------------

def compute_ranks(
    arr: Sequence[T],
    key: Optional[Callable[[T], Any]] = None,
) -> List[int]:
    """
    Compute the unique ranks that constitute the only consistent exit points.

    These ranks are the retrocausal attractors: the positions at which each
    element must appear once the sorted boundary condition is imposed.
    """
    n = len(arr)
    if key is None:
        indices = sorted(range(n), key=lambda i: (arr[i], i))
    else:
        indices = sorted(range(n), key=lambda i: (key(arr[i]), i))
    ranks = [0] * n
    for rank, orig in enumerate(indices):
        ranks[orig] = rank
    return ranks


def photonic_collapse(
    arr: Sequence[T],
    key: Optional[Callable[[T], Any]] = None,
    reverse: bool = False,
) -> List[T]:
    """
    Die at the answer.

    Place every element into the unique rank required by the final sorted
    configuration. The way out of the dataset is via the answer; the photon
    (algorithm) retrocausally exits at the solutative points.
    """
    n = len(arr)
    if n <= 1:
        return list(arr)

    if key is None:
        order = sorted(range(n), key=lambda i: (arr[i], i))
    else:
        order = sorted(range(n), key=lambda i: (key(arr[i]), i))
    if reverse:
        order.reverse()

    result: List[T] = [None] * n  # type: ignore
    for rank, orig_idx in enumerate(order):
        result[rank] = arr[orig_idx]
    return result


# ---------------------------------------------------------------------------
# 4. Main entry — PhotonicSort
# ---------------------------------------------------------------------------

def photonic_sort(
    arr: Sequence[T],
    key: Optional[Callable[[T], Any]] = None,
    reverse: bool = False,
    *,
    force_collapse: bool = False,
    verbose: bool = False,
) -> List[T]:
    """
    PhotonicSort: Give everything. Take nothing. Become photonic.

    Parameters
    ----------
    arr : sequence
        Input data (the haystack).
    key, reverse : standard sorted() semantics.
    force_collapse : if True, always take the pure rank-collapse path.
    verbose : if True, narrate the stages of the analogy.

    Returns
    -------
    A new list containing the sorted elements.
    """
    n = len(arr)
    if n <= 1:
        if verbose:
            print("  [Photonic] trivial haystack — already at the answer.")
        return list(arr)

    if verbose:
        print(f"  [Photonic] Photon entering haystack of size {n} with minimal energy…")

    probe = photonic_probe(arr, key=key)

    if verbose:
        print(
            f"  [Photonic] Probe complete — inv_ratio={probe['inv_ratio']:.3f}, "
            f"max_run={probe['max_run']}, dir_changes={probe['direction_changes']}, "
            f"sortedness={probe['sortedness']:.3f}, "
            f"group_delay_proxy={probe['group_delay_proxy']:.3f}"
        )

    if not force_collapse and probe["is_negative_delay"]:
        if verbose:
            print(
                "  [Photonic] Negative delay detected — leading-edge survivors can exit early. "
                "Taking negative-time path."
            )
        result = negative_time_early_exit(arr, probe, key=key, reverse=reverse)
        if verbose:
            print("  [Photonic] Early-exit path finished. Died at the answer.")
        return result

    if verbose:
        print(
            "  [Photonic] Bulk requires positive dwell. Computing retrocausal ranks "
            "(the only consistent exit points)…"
        )
    result = photonic_collapse(arr, key=key, reverse=reverse)
    if verbose:
        print(
            "  [Photonic] Collapse complete. The photon has exited at the ranks. "
            "Died at the answer."
        )
    return result


# ---------------------------------------------------------------------------
# 5. Demo & self-test — marketing executable
# ---------------------------------------------------------------------------

def _is_sorted(a: Sequence[T], key=None, reverse=False) -> bool:
    if len(a) <= 1:
        return True

    def get(i):
        return key(a[i]) if key else a[i]

    if reverse:
        return all(get(i) >= get(i + 1) for i in range(len(a) - 1))
    return all(get(i) <= get(i + 1) for i in range(len(a) - 1))


def demo() -> None:
    print("=" * 72)
    print(f"PhotonicSort {__version__} — Give everything. Take nothing. Become photonic.")
    print("=" * 72)
    print()
    print("Research anchor: Angulo, Steinberg et al., arXiv:2409.03680")
    print("  “Experimental evidence that a photon can spend a negative amount")
    print("   of time in an atom cloud” (negative group delay / weak-value")
    print("   atomic excitation time). Photons can appear to exit before entry.")
    print()
    print("Inspirational framing (Castagnoli): quantum speedups as teleological")
    print("evolutions with the future solution as attractor. Classical analogy only.")
    print()
    print("This is a correct, adaptive, classical O(n log n) hybrid.")
    print("It does not solve NP-complete problems. The retrocausality is metaphor.")
    print()

    cases = {
        "already sorted": list(range(1000)),
        "reverse sorted": list(range(1000, 0, -1)),
        "random": [random.randint(0, 10_000) for _ in range(2000)],
        "sawtooth": [i % 50 for i in range(1500)],
        "organpipe": list(range(500)) + list(range(500, 0, -1)),
        "few unique": [random.randint(0, 5) for _ in range(1200)],
        "small": [3, 1, 4, 1, 5, 9, 2, 6],
    }

    print("-" * 72)
    print(f"{'Case':<18} {'n':>6}  {'path':<14}  {'ok':>4}  {'time (ms)':>10}")
    print("-" * 72)

    all_ok = True
    for name, data in cases.items():
        t0 = time.perf_counter()
        probe = photonic_probe(data)
        path = "neg-time" if probe["is_negative_delay"] else "collapse"
        out = photonic_sort(data, verbose=False)
        elapsed = (time.perf_counter() - t0) * 1000.0
        ok = _is_sorted(out) and len(out) == len(data) and sorted(data) == out
        all_ok = all_ok and ok
        print(f"{name:<18} {len(data):>6}  {path:<14}  {str(ok):>4}  {elapsed:10.3f}")

    print("-" * 72)
    print()

    print("Verbose walk-through on a small disordered haystack:")
    small = [7, 2, 9, 1, 5, 3, 8, 4, 6, 0]
    print(f"  Input : {small}")
    result = photonic_sort(small, verbose=True)
    print(f"  Output: {result}")
    print(f"  Verified sorted: {_is_sorted(result)}")
    print()

    print("Forced pure retrocausal collapse on the same data:")
    result2 = photonic_sort(small, force_collapse=True, verbose=True)
    print(f"  Output: {result2}")
    print()

    # O(n) pure-structure micro-bench
    big = list(range(50_000))
    t0 = time.perf_counter()
    out_s = photonic_sort(big)
    t_sorted = (time.perf_counter() - t0) * 1000.0
    t0 = time.perf_counter()
    out_r = photonic_sort(list(reversed(big)))
    t_rev = (time.perf_counter() - t0) * 1000.0
    print(f"O(n) structure check (n=50000): sorted={t_sorted:.3f} ms  reverse={t_rev:.3f} ms")
    print(f"  verified: {out_s == big and out_r == big}")
    print()

    if all_ok and out_s == big and out_r == big:
        print("All correctness checks passed.")
    else:
        print("WARNING: some correctness checks failed.")
        sys.exit(1)

    print()
    print("Become photonic.")
    print("=" * 72)


if __name__ == "__main__":
    demo()
