# Expanded Field Suite — FROZEN (A0)

**Date:** 2026-08-12  
**Status:** **FROZEN for path-(a) measurement**  
**Parent:** [`FIELD_LEVEL_ENTRY_ATTACK.md`](./FIELD_LEVEL_ENTRY_ATTACK.md)  
**Rule:** No residual code changes until A1 gap map vs **best specialized** exists on this suite.

---

## 1. Purpose

This is the claim surface any path-(a) field-level entry must survive. Measuring only vs pdq on a soft-spot subset is insufficient. Primary metric is vs **best specialized** (see `BASELINES_LOCKED.md`).

---

## 2. Size matrix (*n*)

| Class | *n* | Role |
|-------|-----|------|
| Sanity | 1e5 | Fast regression |
| **Primary** | **1e6** | Continuity with Phase 0 / soft-spot kills |
| Scale | 1e7 | Cache hierarchy; required for A3 |
| Optional | 1e8 | Only if RAM ≥ 16 GB and time budget allows |

Published field-level numbers use **n = 1e6** (primary) and **n = 1e7** (scale). All generators are deterministic given fixed seeds below.

---

## 3. Type surface (A0 freeze)

| Type | A0 required | Notes |
|------|-------------|-------|
| `int64_t` | **Yes** | Primary excellence surface |
| `int32_t` | **Yes** | Multi-type gate |
| `uint32_t` | **Yes** | Multi-type gate |
| `uint64_t` | Preferred | Same residual menu as i64 where unsigned-safe |
| `float` / `double` | **A3** | Not required for A0/A1 freeze |
| Generic comparator | **A3** | Not required for A0/A1 freeze |

---

## 4. Architecture matrix

| Arch | Required |
|------|----------|
| x86_64 (ubuntu-24.04 class) | **Yes** |
| aarch64 (ubuntu-24.04-arm class) | **Yes** |

Already MET on PO2 subset; must **hold** on full Expanded Field Suite for A3 exit.

---

## 5. Pattern catalog (FROZEN)

### 5.1 Core controlled patterns (19) — already measured

All generators live in the Phase 0 harness family (`gen_*`). Seeds fixed: **seed = 42** unless noted.

| ID | Pattern | Group | Generator contract (summary) |
|----|---------|-------|------------------------------|
| P01 | `sorted` | Structure | Identity / ascending unique |
| P02 | `reverse` | Structure | Strict descending |
| P03 | `almost_sorted` | Structure | Sorted + n/1000 random swaps |
| P04 | `organpipe` | Structure | Up then down |
| P05 | `sawtooth` | Structure | Period-1024 ascending teeth |
| P06 | `reverse_segments` | Structure | 8 equal reversed sorted segments |
| P07 | `push_middle` | Runs | 3 sorted runs (high \| low \| high) |
| P08 | `runs_noise` | Runs | Long runs + local noise |
| P09 | `few_k4_dense` | Low-card | Values in `{0,1,2,3}` cycling |
| P10 | `few_k16_dense` | Low-card | k=16 compact domain |
| P11 | `few_k16_wide` | Low-card | k=16 wide numeric range |
| P12 | `equal_heavy` | Low-card | Large equal blocks |
| P13 | `zipf_k16` | Low-card | Zipf over 16 keys |
| P14 | `db_pk` | Real-shaped | Dense-ish primary keys with gaps |
| P15 | `timestamps` | Real-shaped | Near-monotonic with jitter |
| P16 | `pipe_sparse` | Adversarial | Sparse pipe / HE-ish structured |
| P17 | `adversarial` | Adversarial | Pattern-defeat / pivot stress |
| P18 | `random` | HE | Uniform independent |
| P19 | `gaussianish` | HE | Continuous near-uniform (discretized Gaussian) |

### 5.2 Extended real-shaped (A0 locked, measured in A1+)

| ID | Pattern | Group | Generator contract |
|----|---------|-------|--------------------|
| P20 | `db_fk_zipf` | Real-shaped | Foreign-key style: Zipf over ~√n distinct refs |
| P21 | `timestamp_drift` | Real-shaped | Monotonic base + regime drift every n/10 |
| P22 | `mixed_blocks` | Mixed | Alternating sorted blocks of size 256 + shuffle islands |
| P23 | `uniform_u32` | HE | Full-range uint32 uniform (ska relevance) |

P20–P23 generators must be pure, seed-42 deterministic, and documented in harness before A1 numbers are published. Until implemented, A1 may report core 19 only with explicit “extended pending” note — but A0 **locks the names and contracts**.

### 5.3 Out of claim scope (explicit)

| Pattern class | Why out |
|---------------|---------|
| String / struct keys | Not A0–A3 type surface |
| Parallel / GPU sorts | Sequential claim only |
| Adversarial crafted solely against one residual (non-reproducible) | Forbidden |
| Floating-point until A3 | Deferred |

---

## 6. Seed and reproducibility

- Default seed: **42**
- Every generator: pure function of `(n, seed)` or fixed checksummed snapshot
- Correctness: every timed run must end with `ok=1` (fully sorted, stable multiset of input)
- Docker one-command and multi-arch CI must re-run this suite without private data

---

## 7. Soft-spot / catastrophe rule (unchanged)

Under cold isolation, any **in-scope** pattern with ratio vs **best specialized** > **1.15×** is a soft spot. A2 must close or formally scope it out before A4 claim language.

---

## 8. Freeze statement

> The Expanded Field Suite pattern list, n-matrix, and type surface above are **frozen** as of 2026-08-12. Residual improvements may proceed only against gaps measured on this suite under the locked metric in `METRICS_LOCKED.md`. Changing the suite requires a new A0 revision and invalidates prior path-(a) gap maps.

**THE BEASTIE BOYZ**
