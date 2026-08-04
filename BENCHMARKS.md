# PhotonicSort benchmarks

**Hard numbers beat vibes.** Judge the algorithm on measurements — not the account name.

> Classical adaptive hybrid. **Not** photonic hardware. **Not** “sort at the speed of light.”  
> Worst case remains **O(n log n)**. No P = NP claim.

---

## Headline: PhotonicSort C vs `std::sort` (n = 1,000,000, `int64_t`)

| Pattern | PhotonicSort C (ms) | `std::sort` (ms) | Ratio (`std` / photonic) |
|---------|--------------------:|-----------------:|-------------------------:|
| **Sorted** | **0.019** | 15.03 | **~800×** |
| **Reverse** | **0.44** | 9.34 | **~21×** |
| **Organpipe** | **26.2** | 89.0 | **~3.4×** |
| Sawtooth | 27.1 | **23.9** | 0.88× |
| Random | 105.0 | **78.1** | 0.74× |

**Metric:** best of 5 wall times · correctness checked vs `std::sort` each trial.  
**Build:** `cc -O3 -std=c11` (PhotonicSort) · `g++ -O3 -std=c++17` (`std::sort`).  
**Host:** [`benchmarks/environment.txt`](./benchmarks/environment.txt)

### Same comparison @ n = 200,000

| Pattern | PhotonicSort C | `std::sort` | Ratio |
|---------|----------------:|------------:|------:|
| Sorted | **0.012** | 2.65 | **~220×** |
| Reverse | **0.091** | 1.67 | **~18×** |
| Organpipe | **4.43** | 15.6 | **~3.5×** |
| Sawtooth | 4.74 | **4.69** | ~1.0× |
| Random | 18.3 | **13.8** | 0.75× |

**How to read this**

- **Sorted / reverse:** probe takes the structure early-exit path (near-linear). Design point — same idea family as Timsort’s run exploitation.
- **Random:** residual mergesort-style path; tuned `std::sort` (introsort) wins, as expected.
- Adaptive sorts need not win every shape. They must be **correct** and **exploit structure when present**.

```
Time (ms, best) — C @ n=1e6

Sorted     photonic ·                                              0.019
           std::sort ████████████████████████████████████████████ 15.03

Reverse    photonic ██                                             0.44
           std::sort ████████████████████                          9.34

Organpipe  photonic ██████████                                    26.2
           std::sort ████████████████████████████████             89.0

Random     photonic ████████████████████████████████             105.0
           std::sort ████████████████████████                     78.1
```

Raw data: [`benchmarks/results_c_n1m.csv`](./benchmarks/results_c_n1m.csv) · [`benchmarks/results_c_n200k.csv`](./benchmarks/results_c_n200k.csv)

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
Python = reference / portable. **Structure-exit performance claims live in the C11 port** (`c/`).

CSV: [`benchmarks/results_python.csv`](./benchmarks/results_python.csv)

---

## What is being compared

| Label | Implementation |
|-------|----------------|
| PhotonicSort C | `photonic_sort_i64` — [`c/photonic_sort.c`](./c/photonic_sort.c) |
| `std::sort` | libstdc++ introsort |
| Timsort | CPython 3 `list.sort` |
| PhotonicSort Python | [`photonic_sort.py`](./photonic_sort.py) |

| Pattern | Generator |
|---------|-----------|
| Sorted | `0 .. n-1` |
| Reverse | `n-1 .. 0` |
| Organpipe | ascending then descending |
| Sawtooth | `i % 64` |
| Random | uniform in `[0, 4n)` |

---

## Non-claims

1. No optical hardware.
2. Does not “sort at light speed.”
3. Does not prove P = NP.
4. On unstructured random keys, `std::sort` remains a strong default.
5. Gains are from **structure detection + early exit**. “Photonic” is metaphor — [`RESEARCH.md`](./RESEARCH.md).

*Re-run on your hardware before publishing comparative claims.*
