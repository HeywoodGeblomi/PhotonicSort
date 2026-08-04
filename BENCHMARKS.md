# PhotonicSort benchmarks

**Hard numbers beat vibes.** This file exists so reviewers can judge the algorithm on measurements — not on the account name.

> Classical adaptive hybrid. **Not** photonic hardware. **Not** “sort at the speed of light.”  
> Worst case remains **O(n log n)**. No P = NP claim.

![PhotonicSort C vs std::sort, n=1e6](./benchmarks/photonic_vs_stdsort_n1m.svg)

---

## Headline (C, n = 1,000,000, `int64_t`)

Host: see [`benchmarks/environment.txt`](./benchmarks/environment.txt).  
Build: `cc -O3 -std=c11` for PhotonicSort; `g++ -O3 -std=c++17` for `std::sort`.  
Metric: **best of 5** wall times (ms). Correctness checked against `std::sort` every trial.

| Pattern | PhotonicSort C | `std::sort` | Ratio (`std` / photonic) |
|---------|----------------:|------------:|-------------------------:|
| Sorted | **0.019** | 15.03 | **~800×** |
| Reverse | **0.44** | 9.34 | **~21×** |
| Organpipe | **26.2** | 89.0 | **~3.4×** |
| Sawtooth | 27.1 | **23.9** | 0.88× |
| Random | 105.0 | **78.1** | 0.74× |

**Reading the table**

- On **already-ordered / reverse** data the probe takes the **negative-time early path** (near-linear structure exit). That is the design point of PhotonicSort — same family as Timsort’s run exploitation, different metaphor.
- On **random** data the residual path is a stable mergesort-style collapse. `std::sort` (introsort) wins, as expected for a highly tuned general-purpose baseline.
- Adaptive algorithms are **not** required to dominate every shape. They are required to be **correct** and to **exploit structure when it is present**.

Raw CSV: [`benchmarks/results_c_n1m.csv`](./benchmarks/results_c_n1m.csv) · also [`results_c_n200k.csv`](./benchmarks/results_c_n200k.csv)

---

## Python vs CPython Timsort (honesty panel)

| Pattern (n=1e6) | PhotonicSort Python | `list.sort` (Timsort) |
|-----------------|--------------------:|----------------------:|
| Sorted | 8.4 | **6.2** |
| Reverse | 9.6 | **8.6** |
| Organpipe | 16.2 | **8.6** |
| Sawtooth | 466 | **24.6** |
| Random | 1525 | **269** |

CPython’s `list.sort` is **native C Timsort**. A pure-Python probe + sort cannot beat it on bulk paths; that is expected, not a scandal. The Python module is the **reference / marketing / portable** implementation. The **performance claim for adaptive structure exits lives in the C11 port** (`c/`).

CSV: [`benchmarks/results_python.csv`](./benchmarks/results_python.csv)

PNG charts (same data): [`photonic_vs_stdsort_n1m.png`](./benchmarks/photonic_vs_stdsort_n1m.png) · [`photonic_vs_stdsort_by_n.png`](./benchmarks/photonic_vs_stdsort_by_n.png) · [`photonic_py_vs_timsort_n1m.png`](./benchmarks/photonic_py_vs_timsort_n1m.png)

---

## What is being compared

| Name in charts | Implementation |
|----------------|----------------|
| **PhotonicSort C** | `photonic_sort_i64` in [`c/photonic_sort.c`](./c/photonic_sort.c) |
| **`std::sort`** | libstdc++ introsort (`g++ -O3`) |
| **Timsort** | CPython 3 `list.sort` / `sorted()` |
| **PhotonicSort Python** | [`photonic_sort.py`](./photonic_sort.py) |

Patterns: Sorted `0..n-1` · Reverse `n-1..0` · Organpipe up/down · Sawtooth `i%64` · Random uniform in `[0,4n)`.

---

## Non-claims (read before quoting)

1. PhotonicSort does **not** use optical hardware.
2. It does **not** violate causality or “sort at light speed.”
3. It does **not** prove P = NP or solve NP-complete problems.
4. On unstructured random keys, tuned introsort/`std::sort` remains a strong default.
5. Gains shown above are from **structure detection + early exit** — standard adaptive-sort engineering; the “photonic” framing is metaphorical (see [`RESEARCH.md`](./RESEARCH.md)).

---

*Benchmarks generated for release documentation. Re-run on your hardware before publishing comparative claims.*
