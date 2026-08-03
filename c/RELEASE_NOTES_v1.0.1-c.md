# PhotonicSort C11 — Release Notes v1.0.1-c

**Public code-change verification index**  
**Tag:** `v1.0.1-c`  
**Commit:** `841882d906dc3fbf3bbc5c8dfa97f2773c0c1818`  
**Date:** 2026-08-03  
**Title:** Highly optimized C11 port  
**Repo:** https://github.com/HeywoodGeblomi/PhotonicSort  
**Release:** https://github.com/HeywoodGeblomi/PhotonicSort/releases/tag/v1.0.1-c  

> Give everything. Take nothing. Become photonic.  
> Classical adaptive hybrid only. **Does not** solve NP-complete problems. **Does not** prove P = NP.

---

## 1. Scope of code change

This release **adds** a complete C11 implementation under `c/` and documents it from the repository root. Python sources from v1.0.1 are unchanged by this tag’s intent (companion language port).

| Action | Path |
|--------|------|
| **ADD** | `c/photonic_sort.h` |
| **ADD** | `c/photonic_sort.c` |
| **ADD** | `c/Makefile` |
| **ADD** | `c/README.md` |
| **ADD** | `c/examples/demo.c` |
| **ADD** | `c/tests/test_photonic_sort.c` |
| **ADD** | `c/RELEASE_NOTES_v1.0.1-c.md` (this file) |
| **UPDATE** | `README.md` (C section) |
| **UPDATE** | `RESEARCH.md` (C fidelity note) |
| **UPDATE** | `.gitignore` (C build artifacts) |

Introducing commit (message):

```
Add highly optimized C11 port (c/)

- photonic_sort.h / photonic_sort.c: probe, O(n) structure exits, stable residual
- int64 fast path + generic comparator API
- examples/demo.c, unit tests, Makefile (-O3)
- Root README + RESEARCH link
```

---

## 2. SHA-256 verification index (canonical tree)

Verify after clone:

```bash
git checkout v1.0.1-c
cd c
sha256sum -c <<'HASHES'
# Paste block below, or recompute and diff against this section
HASHES
```

| File | Bytes | Lines | SHA-256 |
|------|------:|------:|---------|
| `c/photonic_sort.h` | 3497 | 93 | `16b795d5150d59e4d0bcf5a16182be041f351a3e8d0ecdc1fb667814b348e622` |
| `c/photonic_sort.c` | 16905 | 551 | `e4d4446e136b8b03c95dbb5a53ea4884db0351491a235e7c4e63eb4ad68ffb54` |
| `c/Makefile` | 921 | 35 | `ff71d4adb893a87c72dc0c0fa33ef89b3fe5267bd044e930b9613db25c494715` |
| `c/README.md` | 1830 | 79 | `4dc00cb880af4bdd28149f1e9628683ed6783b6eb1c8cfd3941779309c28b8e6` |
| `c/examples/demo.c` | 4980 | 136 | `d453c678f19b2d4f6378eed640ac328bf914a71d9d0e331e698bc64c5ca1339a` |
| `c/tests/test_photonic_sort.c` | 4392 | 138 | `1637f73a1adfb88ce638a0102e1a921cae746ca4a4aaa861721f045e7b11fd59` |

**Pinned tree OID (tag target):** `841882d906dc3fbf3bbc5c8dfa97f2773c0c1818`  
**Tag object (annotated):** resolve with `git rev-parse v1.0.1-c`

> Note: This RELEASE_NOTES file is added **after** the original tag blob set.  
> Hashes above describe the **C implementation sources at commit `841882d`**.  
> After this notes commit, re-verify implementation files with:
>
> ```bash
> git show 841882d:c/photonic_sort.c | sha256sum
> git show 841882d:c/photonic_sort.h | sha256sum
> ```

---

## 3. Public API surface (change contract)

### Header version macros

```c
PHOTONIC_SORT_VERSION_MAJOR  1
PHOTONIC_SORT_VERSION_MINOR  0
PHOTONIC_SORT_VERSION_PATCH  1
PHOTONIC_SORT_VERSION_STRING "1.0.1-c"
```

### New types

| Symbol | Role |
|--------|------|
| `photonic_probe_t` | Disorder profile (inv_ratio, max_run, monotone_sign, …) |
| `photonic_cmp_fn` | `int (*)(const void *, const void *)` |

### New functions

| Symbol | Semantics |
|--------|-----------|
| `photonic_probe_i64` | Single-pass O(n) probe over `int64_t[]` |
| `photonic_sort_i64` | In-place adaptive sort; return path code |
| `photonic_sort_i64_force_collapse` | Force residual/collapse path |
| `photonic_sort_i64_copy` | Out-of-place sort into `dst` |
| `photonic_probe_generic` | Probe for arbitrary element size + cmp |
| `photonic_sort` | In-place adaptive generic sort |
| `photonic_is_sorted_i64` | Ascending check |
| `photonic_sort_version` | Returns `"1.0.1-c"` |

### Path return codes

| Code | Meaning |
|-----:|---------|
| `0` | trivial (n ≤ 1) |
| `1` | negative-time / structure early path |
| `2` | collapse / residual sort |
| `-1` | allocation failure |

---

## 4. Behavioral parity with Python v1.0.1

| Stage | C behavior | Python analog |
|-------|------------|---------------|
| Probe | Single pass: runs + inversions + equals | `photonic_probe` |
| Sampling | Stratified + endpoint; limit 4096 | `sample_limit=4096` |
| Extra pairs | Deterministic LCG seed `n ^ 0x9E3779B9` | `random.Random(n ^ 0x9E3779B9)` |
| Thresholds | sortedness ≥ 0.72; max_run fractions; monotone | same |
| Early exit | O(n) no-op / reverse on pure monotone | `negative_time_early_exit` |
| Residual | Stable bottom-up mergesort + insertion n≤32 | Timsort / rank collapse |
| Honesty | No P=NP claim | same |

---

## 5. Optimizations introduced (C-only)

1. **`restrict` pointers** on hot paths  
2. **No materialised full index list** on full probe scans  
3. **O(n) reverse** with two-pointer swaps  
4. **Insertion sort** cutover at n ≤ 32 (cache-friendly)  
5. **Stable mergesort** residual (equals preserve left-first order)  
6. **Word-sized generic swap** when `size ≥ sizeof(size_t)`  
7. **Zero third-party deps** (libc only)

---

## 6. Build & verification commands

```bash
git clone https://github.com/HeywoodGeblomi/PhotonicSort.git
cd PhotonicSort
git checkout v1.0.1-c   # or main after notes merge

# Hash check against section 2 (implementation at 841882d)
git show 841882d:c/photonic_sort.c | sha256sum
# expect: e4d4446e136b8b03c95dbb5a53ea4884db0351491a235e7c4e63eb4ad68ffb54

git show 841882d:c/photonic_sort.h | sha256sum
# expect: 16b795d5150d59e4d0bcf5a16182be041f351a3e8d0ecdc1fb667814b348e622

cd c
make clean all test
./demo
```

Expected test line: `All tests passed.`  
Expected demo tagline: `Become photonic.`

---

## 7. Explicit non-claims (verification of marketing honesty)

The following **must not** appear as product claims for this C port:

- Solves NP-complete problems  
- Proves P = NP  
- Physical retrocausality or time travel inside the CPU  
- Asymptotic breakthrough beyond comparison sorting  

Complexity remains: **O(n)** structure exits, **O(n log n)** worst-case residual.

---

## 8. Related public artifacts

| Artifact | URL |
|----------|-----|
| C release | https://github.com/HeywoodGeblomi/PhotonicSort/releases/tag/v1.0.1-c |
| Python v1.0.1 | https://github.com/HeywoodGeblomi/PhotonicSort/releases/tag/v1.0.1 |
| Companion C++ hybrid | https://github.com/HeywoodGeblomi/GeblomiSort |
| Research mapping | `RESEARCH.md` · arXiv:2409.03680 |

---

## 9. Team

Grok · Harper · Benjamin · Lucas · Heywood Geblomi  
License: MIT
