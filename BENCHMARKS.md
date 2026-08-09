# PhotonicSort benchmarks

**Hard numbers beat vibes.** Judge the algorithm on measurements — not the account name.

> Classical adaptive hybrid. **Not** photonic hardware. **Not** “sort at the speed of light.”  
> Worst case remains **O(n log n)**. No P = NP claim.

## Vector-2 / Domination Suite v0.1 (hybrid residual configuration)

**Status: Criteria MET** (2026-08-09)

| Criterion | Threshold | Measured | Result |
|-----------|-----------|----------|--------|
| Geometric mean vs `std::sort` | ≥ 1.5× | **7.08×** | PASS |
| Geometric mean vs best specialized (pdqsort / ska_sort) | ≥ 1.5× | **1.94×** | PASS |
| Minimum speedup vs `std::sort` | ≥ 1.0× | **1.32×** | PASS |
| Major regressions vs specialized | 0 | **0** | PASS |

**What was under test:** Photonic probe + residual selection. Structured / patterned / low-card paths used the pure-C Photonic residual menu. High-entropy paths (random, adversarial) dispatched to library pdqsort / ska_sort after selection from visible probe metrics.

**Scope (mandatory):** On Domination Suite v0.1 the locked Vector-2 criteria are met. Structured / patterned / low-card wins remain pure-C Photonic. High-entropy residual uses library-strength pdqsort / ska_sort after correct selection. Result is scoped to this suite; broader evaluation may differ. No asymptotic claim. Full details: [docs/vector2/VECTOR2_CLAIM_v0.1.md](./docs/vector2/VECTOR2_CLAIM_v0.1.md).

The pure-C self-contained residual numbers (Plan A / v1.3.x) continue below.

---

**Headline wall-time vs `std::sort`:** C11 **v1.3.1-c** (Plan A residual menu).  
**Mode panel (NORMAL / AGGRESSIVE / FORCE_HOLE):** C11 **v1.3.2-c** — see below.

Structure early-exit is **verified O(n)** before returning (no silent false positives).

---

## Headline: PhotonicSort C vs `std::sort` (n = 1,000,000, `int64_t`)

| Pattern | PhotonicSort C (ms) | `std::sort` (ms) | Ratio (`std` / photonic) |
|---------|--------------------:|-----------------:|-------------------------:|
| **Sorted** | **0.37** | 10.62 | **~29×** |
| **Reverse** | **0.78** | 7.15 | **~9×** |
| **Organpipe** | **2.47** | 81.3 | **~33×** |
| **Sawtooth** | **1.59** | 23.2 | **~15×** |
| **Few unique** | **1.52** | 19.7 | **~13×** |
| **Almost sorted** | **2.97** | 10.84 | **~3.6×** |
| **Random** | **46.7** | 77.7 | **~1.7×** |

**Metric:** best of 5 wall times · correctness checked vs `std::sort` each trial.  
**Build:** `cc -O3 -std=c11` (PhotonicSort) · `g++ -O3 -std=c++17` (`std::sort`).  
**Host:** [`benchmarks/environment.txt`](./benchmarks/environment.txt)

### Same comparison @ n = 200,000

| Pattern | PhotonicSort C | `std::sort` | Ratio |
|---------|----------------:|------------:|------:|
| Sorted | **0.068** | 1.97 | **~29×** |
| Reverse | **0.144** | 1.34 | **~9×** |
| Organpipe | **0.37** | 14.8 | **~40×** |
| Sawtooth | **0.27** | 4.50 | **~17×** |
| Few unique | **0.26** | 3.78 | **~14×** |
| Almost sorted | **1.60** | 1.90 | **~1.2×** |
| Random | **3.48** | 13.8 | **~4.0×** |

**How to read this**

- **Sorted / reverse:** probe + O(n) verify → structure path (still far ahead of `std::sort`).
- **Sawtooth / few unique:** `LOW_CARD` counting residual (Plan A).
- **Almost sorted:** `LOW_DISORDER` insertion/pdq residual.
- **Organpipe:** patterned / run-merge residual.
- **Random:** GyroRank pilot → LSD radix residual; Plan A wins on this host at n=1e6 (~1.7×).

Raw data: [`benchmarks/results_c_n1m.csv`](./benchmarks/results_c_n1m.csv) · [`benchmarks/results_c_n200k.csv`](./benchmarks/results_c_n200k.csv)

---

## v1.3.2-c mode panel — NORMAL vs AGGRESSIVE vs FORCE_HOLE

n = 100 000 · 24 trials/cell · sorted + exact *k* pairwise swaps · correctness 24/0 every cell.  
Harness: [`c/tests/sensitivity_modes.c`](./c/tests/sensitivity_modes.c)

### STRUCTURE hole-in-one rate (path code 1)

| swap frac | NORMAL | AGGRESSIVE | FORCE_HOLE |
|-----------|-------:|-----------:|-----------:|
| 0.0000 (pure sorted) | **100%** | **100%** | **100%** |
| 0.0005 → 0.1200 | **0%** | **0%** | **0%** |
| pure random | **0%** | **0%** | **0%** |

Modes do **not** increase STRUCTURE hit rate. Mandatory O(n) verify rejects STRUCTURE as soon as any swaps exist.

### Residual median wall time (ms)

| swap frac | k | NORMAL | AGGRESSIVE | FORCE_HOLE |
|-----------|--:|-------:|-----------:|-----------:|
| 0.0005 | 50 | 0.276 | **0.246** | 0.766 |
| 0.0010 | 100 | 0.742 | **0.254** | 0.773 |
| 0.0020 | 200 | 0.775 | 0.764 | 0.784 |
| 0.0400 | 4000 | **0.292** | 1.578 | 1.597 |
| 0.0800 | 8000 | **0.263** | 0.282 | 2.277 |
| 0.1200 | 12000 | 0.260 | 0.259 | 0.281 |
| random | — | 1.82 | 1.81 | 1.79 |

**Reading:** AGGRESSIVE wins the light almost-sorted band (≈0.05–0.1% swaps). FORCE_HOLE can pay a ladder tax on mid-band disorder. Modes change residual selection/timing, not STRUCTURE rate. Default remains NORMAL.

---

## Route → residual (Plan A)

| Route | When | Residual |
|-------|------|----------|
| STRUCTURE | Fully sorted / reverse (verified) | O(n) exit or reverse-in-place |
| LOW_CARD | Few distinct keys / small dense range | Counting sort |
| LOW_DISORDER | Low inversion ratio | Cache-local insertion (n≤4096) / pdq |
| PATTERNED | Long runs | Capacity-checked run merge (≤32 runs) |
| RANDOM | Unstructured | LSD radix → introsort fallback |

Brand contract: **probe → structure early-exit → residual talent menu**.

---

## PhotonicSort Python vs CPython Timsort (honesty panel)

n = 1,000,000 · best of 5 · `list.sort` is **native C Timsort** inside CPython.

| Pattern | PhotonicSort Python (ms) | `list.sort` Timsort (ms) |
|---------|-------------------------:|-------------------------:|
| Sorted | 8.4 | **6.2** |
| Reverse | 9.6 | **8.6** |
| Organpipe | 16.2 | **8.6** |
| Sawtooth | 466 | **24.6** |
| Random | 1525 | **269** |

A pure-Python probe cannot beat CPython’s C Timsort on bulk paths — expected.  
Python = reference / portable. **Performance claims live in the C11 port** (`c/`).

CSV: [`benchmarks/results_python.csv`](./benchmarks/results_python.csv)

---

## What is being compared

| Label | Implementation |
|-------|----------------|
| PhotonicSort C (headline) | `photonic_sort_i64` — **v1.3.1-c** |
| PhotonicSort C (mode panel) | `photonic_sort_i64_ex` — **v1.3.2-c** |
| `std::sort` | libstdc++ introsort |
| Timsort | CPython 3 `list.sort` |
| PhotonicSort Python | [`photonic_sort.py`](./photonic_sort.py) |

| Pattern | Generator |
|---------|-----------|
| Sorted | `0 .. n-1` |
| Reverse | `n-1 .. 0` |
| Organpipe | ascending then descending |
| Sawtooth | `i % 64` |
| Few unique | `i % 16` |
| Almost sorted | sorted + ~0.1% random swaps |
| Sparse-swap panel | sorted + exact *k* pairwise swaps |
| Random | uniform in `[0, 4n)` |

---

## Non-claims

1. No optical hardware.
2. Does not “sort at light speed.”
3. Does not prove P = NP.
4. On some hosts / sizes, `std::sort` can still win pure random — always re-measure.
5. Gains come from **structure detection + residual routing**. “Photonic” is metaphor — [`RESEARCH.md`](./RESEARCH.md).
6. AGGRESSIVE / FORCE_HOLE do **not** raise STRUCTURE hole-in-one rate on sparse-swap data.
7. The pure-C residual menu does not claim to beat library pdqsort / ska_sort on pure high-entropy; the Vector-2 result uses library residual after selection on those patterns.

*Re-run on your hardware before publishing comparative claims.*
