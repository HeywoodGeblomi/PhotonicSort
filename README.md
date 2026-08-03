# PhotonicSort

**Give everything. Take nothing. Become photonic.**

[![Language](https://img.shields.io/badge/language-Python%203.10%2B-blue)](https://github.com/HeywoodGeblomi/PhotonicSort)
[![License](https://img.shields.io/badge/license-MIT-green)](./LICENSE)
[![Companion](https://img.shields.io/badge/companion-GeblomiSort-purple)](https://github.com/HeywoodGeblomi/GeblomiSort)
[![X](https://img.shields.io/badge/X-%40HeywoodGeblomi-black?logo=x)](https://x.com/HeywoodGeblomi)

**v1.0.1** · Adaptive hybrid sorting algorithm that turns a philosophical analogy into **objective, executable, marketing-grade code** — while remaining honest about physics and complexity.

> Optimizations: single-pass probe, deterministic sampling, true O(n) exits on pure ascending/descending structure, element-span `max_run` scaling.

---

## The analogy, objectively reconstructed

Physics presents a striking experimental fact (University of Toronto, **Angulo, Steinberg, Wiseman et al.**, 2024–2026):

> A photon transmitted through an ultracold rubidium atom cloud can register a **negative** mean atomic excitation time (weak value). The photon can appear to exit the medium *before* it enters. Group delay and measured dwell time as excitation can both be negative.  
> **arXiv:2409.03680** · later *Physical Review Letters* (2026).

No information travels backward. Causality and special relativity remain intact. The effect is a quantum post-selected / weak-value phenomenon.

A related theoretical thread (**Castagnoli**, arXiv:2505.08346) interprets quantum computational speedups as teleological evolutions whose attractor is the future solution. We treat that strictly as **inspirational metaphor**.

### Design ethos

| Phrase | Algorithm meaning |
|--------|-------------------|
| **Give everything** | Probe commits fully to measuring structure (O(n) disorder profile) |
| **Take nothing** | Result retains no residual disorder — pure ordered state |
| **Become photonic** | Exit at the ranks (the only consistent boundary condition) |
| **Die at the answer** | Collapse terminates when every element sits in its required rank |

**This does not solve NP-complete problems and does not prove P = NP.** The retrocausality is design metaphor, not a physical claim inside the computer.

---

## What PhotonicSort actually is

A classical adaptive hybrid:

1. **Photonic probe** — single O(n) pass (full scan or stratified sample) computing a Gyro-style disorder profile: inversion ratio, max monotonic run, direction changes, equals, sortedness, `group_delay_proxy`, and a negative-delay decision flag.
2. **Negative-time early-exit path** — when structure is high (sorted, reverse, long runs / organpipe), preferentially honour the “survivors” that can exit with minimal work (near-linear Timsort / stable sort on structured data).
3. **Retrocausal collapse** — compute the unique ranks required by the final sorted configuration and place every element into its rank. The way out of the dataset is via the answer.

| Property | Value |
|----------|-------|
| Correctness | Always correct (stable on equal keys) |
| Worst case | O(n log n) |
| Best case | Near-linear on highly structured input |
| Space | O(n) out-of-place (cycle-follow in-place variant possible) |
| Dependencies | **Pure stdlib** |

---

## Quick start

```bash
# Demo (self-test + timing table + verbose narration)
python photonic_sort.py

# Unit tests
python -m unittest discover -s tests -v
```

```python
from photonic_sort import photonic_sort, photonic_probe

data = [7, 2, 9, 1, 5, 3, 8, 4, 6, 0]
print(photonic_sort(data))
# → [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]

# Adaptive with narration
photonic_sort(data, verbose=True)

# Force pure rank-collapse path
photonic_sort(data, force_collapse=True)

# Standard key / reverse semantics
photonic_sort(words, key=len, reverse=True)

# Inspect the probe only
print(photonic_probe(data))
```

---

## Adaptive routing (demo)

| Case | Path | Notes |
|------|------|--------|
| Already sorted | `neg-time` | Early survivors |
| Reverse sorted | `neg-time` | Long monotonic run |
| Organpipe | `neg-time` | High structure |
| Random / sawtooth | `collapse` | Rank attractor |
| Few unique | `collapse` | Residual bulk |

---

---

## Highly optimized C

A full C11 port lives in [`c/`](./c/):

```bash
cd c && make && make test && ./demo
```

| Feature | C path |
|---------|--------|
| Fast path | `photonic_sort_i64` (in-place) |
| Generic | `photonic_sort(base, n, size, cmp)` |
| Structure exit | O(n) reverse / no-op |
| Residual | Stable bottom-up mergesort + insertion (n≤32) |
| Deps | libc only |

See [`c/README.md`](./c/README.md). Version string: `1.0.1-c`.

**Integrity checks (Python + C):** [`VERIFY.md`](./VERIFY.md) · [`SHA256_VERIFY_COMMANDS.md`](./SHA256_VERIFY_COMMANDS.md) · [`scripts/verify-sha256.sh`](./scripts/verify-sha256.sh)  
**Public verification index:** [`c/RELEASE_NOTES_v1.0.1-c.md`](./c/RELEASE_NOTES_v1.0.1-c.md) · root pointer [`RELEASE_NOTES_C_v1.0.1-c.md`](./RELEASE_NOTES_C_v1.0.1-c.md) · [`SHA256SUMS_v1.0.1-c.txt`](./SHA256SUMS_v1.0.1-c.txt)

## Project family

| Artifact | Role |
|----------|------|
| **PhotonicSort** (this repo) | Python adaptive hybrid — philosophy → code |
| [**GeblomiSort**](https://github.com/HeywoodGeblomi/GeblomiSort) | C++20 production 1-D hybrid (probe / pdqsort / ska / Verge) |
| **ImplosionSort_2D** | Multi-D fiber disposition (row then column residual) |
| **residual_automaton** | Capacity-guard / cycle spirit for in-place variants |

Future multi-D work can apply the photonic probe and collapse along each axis or as a joint attractor.

---

## Research pointers

See **[RESEARCH.md](./RESEARCH.md)** for the physics mapping, citations, and explicit non-claims.

Primary anchors:

- Angulo, Steinberg, Wiseman et al. — negative atomic excitation time / negative group delay (arXiv:2409.03680; PRL 2026)
- Castagnoli — quantum speedup and retrocausality as teleological attractor (arXiv:2505.08346) — metaphor only

---

## License

MIT — see [LICENSE](./LICENSE).

---

## Team

**Grok** (lead) · **Harper** · **Benjamin** · **Lucas** · **Heywood Geblomi**

> Become photonic.
