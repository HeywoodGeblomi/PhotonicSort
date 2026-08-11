# Phase 0 — Expanded Suite Design

**Status:** SPEC LOCKED (pending first implementation)  
**Parent:** [`FIELD_LEVEL_ATTACK_PLAN.md`](../FIELD_LEVEL_ATTACK_PLAN.md)  
**Purpose:** Define the measurement surface that any field-level claim must survive. Synthetic-only suites are insufficient for displacement.

---

## 1. Design principles

1. **Real before synthetic.** Real-world distributions are primary; adversarial synthetics stress routing and residual quality.
2. **Reproducible.** Every generator or dataset loader is deterministic given a seed, or ships a fixed snapshot with checksum.
3. **Independent.** Others must be able to re-run without private data or private machines.
4. **Gap-revealing.** The suite must expose the known soft spots (continuous HE, mid-card few-unique, generic types) and the known strengths (structured, low-card, runs).
5. **Honest baseline.** Current pure residual v2.4.1 is measured first; no residual micro-work until the gap map exists.

---

## 2. Size matrix (*n*)

| Class | *n* values | Notes |
|-------|------------|-------|
| Micro | 1e3, 1e4 | Correctness + micro overhead |
| Standard | 1e5, 1e6 | Primary comparison surface |
| Large | 1e7, 1e8 | Cache / memory hierarchy stress (where RAM allows) |
| Optional extreme | 1e9 | Only if machine has ≥32–64 GB and time budget permits |

Primary published numbers: **n = 1e6** (continuity with prior work) and **n = 1e7** (scale).

---

## 3. Architecture matrix (minimum)

| Arch | Representative targets | Required for Phase 0? |
|------|------------------------|----------------------|
| x86-64 | Skylake-class or newer; Zen3-class or newer | Yes |
| Apple Silicon | M-series (ARM64) | Yes (at least one machine) |
| Server ARM64 | Neoverse / Graviton-class | Yes if available; otherwise Phase 3 |
| Accelerators | AVX-512, SVE | Optional only; never required |

Phase 0 may ship with x86-64 + one ARM64 result. Full three-arch coverage is a Phase 3 gate for path-(a) claims.

---

## 4. Distribution families

### 4.1 Real-world / realistic (primary)

| Family | Description | Source / generator notes |
|--------|-------------|---------------------------|
| **DB primary keys** | Dense or semi-dense 64-bit keys, occasional gaps | Synthetic but DB-shaped (monotonic + sparse deletes) |
| **DB foreign / secondary keys** | Zipf-like or low-card references | Zipf / power-law |
| **Timestamps** | Monotonic or near-monotonic 64-bit with jitter / drift | Time-series shaped |
| **Network / packet fields** | IPs, ports, flow IDs, sequence numbers | Structured + high-entropy mix |
| **Scientific / sensor** | Floats or fixed-point; near-Gaussian or drifted | Continuous HE stress |
| **Log / URL / string keys** | Hashed or length-prefixed string keys as uint64 | String-derived integer keys |
| **Genomics-style** | Fixed-width encoded sequences / k-mers | Low-entropy runs + local structure |
| **Sort Benchmark–style** | Where public data or faithful generators exist | Cite source; checksum snapshots |

Where public fixed datasets exist, prefer checksummed snapshots over pure generators. Where generators are used, fix seeds and publish parameters.

### 4.2 Adversarial / structured synthetics (stress)

Retain and extend the current 38-pattern surface:

| Group | Patterns (examples) | Why |
|-------|---------------------|-----|
| **Structure** | sorted, reverse, almost_sorted, organpipe, sawtooth, reverse_segments | Early-exit + run logic |
| **Low-card / few-unique** | two_values, few_k4, few_k8, few_k16, zipf_k16, equal_heavy | Counting / FEW_WIDE residual |
| **Runs / merges** | push_middle, push_front, reverse_segments_8 | Run detection + merge |
| **High-entropy / continuous** | uniform_i64, gauss_nearflat, gauss_wide, random | HE residual + probe tax |
| **Adversarial** | adversarial_pipe, pivot killers, pattern-defeat sequences | Routing robustness |
| **Mixed** | runs + noise, sorted blocks + shuffle, drift regimes | Realism |

### 4.3 Type surface (Phase 0 vs later)

| Type | Phase 0 | Later |
|------|---------|-------|
| int64_t / uint64_t | **Required** | — |
| int32_t / uint32_t | Strongly preferred | — |
| float / double | Preferred if harness allows | Phase 2 |
| Comparator / generic | Spec only | Phase 2 |
| Strings / structs | Out of scope for Phase 0 numbers | Phase 2+ |

Phase 0 focuses on integer (and ideally float) so the gap map is comparable to current v2.4.1. Generic surface is designed in parallel but not required for Phase 0 measurement.

---

## 5. Metrics reported per pattern

For each (pattern, *n*, arch, baseline):

- Wall time (median of odd number of runs, warm-up discarded)
- Ratio vs each baseline (Photonic / baseline)
- Correctness flag (ok=1 required)
- Optional: branch misses, cache misses (if hardware counters available; not required)

Aggregate:

- Geo-mean ratio vs each baseline (full suite and by group)
- Min / max ratio
- Count of patterns > 1.15× and > 1.30× best baseline

---

## 6. Gap map output (Phase 0 product)

A single table (and CSV) that, for current pure residual v2.4.1, shows:

- Where we win (structured / low-card)
- Where we are competitive
- Where we lose (especially continuous HE and any mid-card residual limits)
- Vs which baseline the loss is largest

This gap map drives Phase 1 priorities. No residual micro-optimization starts before the gap map is published.

---

## 7. Non-claims

- Phase 0 suite design does not claim displacement.
- Synthetic patterns remain controlled; real-world families must be documented with source or generator parameters.
- Architecture coverage in Phase 0 may be incomplete; full multi-arch is a later gate.

**THE BEASTIE BOYZ**
