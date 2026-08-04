# PhotonicSort benchmarks

**Hard numbers beat vibes.** Judge the algorithm on measurements — not the account name.

> Classical adaptive hybrid. **Not** photonic hardware. **Not** “sort at the speed of light.”  
> Worst case remains **O(n log n)**. No P = NP claim.

**Version under test:** C11 **v1.3.1-c** (Plan A residual menu + cache-local insertion).  
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

```
Time (ms, best) — C v1.3.1-c @ n=1e6

Sorted        photonic ·                                               0.37
              std::sort ████████████████████████████                   10.62

Reverse       photonic ██                                              0.78
              std::sort ████████████████                               7.15

Organpipe     photonic █                                               2.47
              std::sort ████████████████████████████████████████████  81.3

Sawtooth      photonic █                                               1.59
              std::sort ████████████████████████                       23.2

Few unique    photonic █                                               1.52
              std::sort ████████████████████                           19.7

Almost sorted photonic ███                                             2.97
              std::sort ████████████████                               10.84

Random        photonic ████████████████████████                        46.7
              std::sort ████████████████████████████████████           77.7
```

Raw data: [`benchmarks/results_c_n1m.csv`](./benchmarks/results_c_n1m.csv) · [`benchmarks/results_c_n200k.csv`](./benchmarks/results_c_n200k.csv)

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
| PhotonicSort C | `photonic_sort_i64` — [`c/photonic_sort.c`](./c/photonic_sort.c) **v1.3.1-c** |
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
| Random | uniform in `[0, 4n)` |

---

## Non-claims

1. No optical hardware.
2. Does not “sort at light speed.”
3. Does not prove P = NP.
4. On some hosts / sizes, `std::sort` can still win pure random — always re-measure.
5. Gains come from **structure detection + residual routing**. “Photonic” is metaphor — [`RESEARCH.md`](./RESEARCH.md).

*Re-run on your hardware before publishing comparative claims.*
