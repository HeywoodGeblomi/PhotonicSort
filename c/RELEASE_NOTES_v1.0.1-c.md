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

## 2. Full SHA-256 checksum catalog

**Algorithm:** SHA-256 (FIPS 180-4)  
**Implementation commit:** `841882d906dc3fbf3bbc5c8dfa97f2773c0c1818`  
**Tag:** `v1.0.1-c`

| Machine-readable file | Scope |
|-----------------------|--------|
| [`SHA256SUMS_v1.0.1-c.txt`](../SHA256SUMS_v1.0.1-c.txt) | Full repository tree @ `841882d` |
| [`c/SHA256SUMS`](./SHA256SUMS) | C code artifacts (live-check on `main`) |

### 2.1 Complete repository tree @ `841882d` (all 14 files)

| SHA-256 | Bytes | Path |
|---------|------:|------|
| `c5a4e48ca579d8be5584b1f26b01cfbe753b59c1bba219be3d8f8be30921c61f` | 225 | `.gitignore` |
| `27bb8f20a000f0cae81eefa18d9c9c9e312199f4b26a6b428004fd475018911a` | 649 | `CITATION.cff` |
| `b7de01ba944dd7ef1fa8f9feef55769cdc9bfdfd9681c314e95d87bac9632521` | 1123 | `LICENSE` |
| `da849e28e0ce605bacf9b26201f06f9ee6387d7bf3e1d33f587cef17dd11182a` | 5803 | `README.md` |
| `7ec498f28fb3345e359fd6f7c406934b4a63ca115115b82af6fcfe70ce8bf6a1` | 4295 | `RESEARCH.md` |
| `ff71d4adb893a87c72dc0c0fa33ef89b3fe5267bd044e930b9613db25c494715` | 921 | `c/Makefile` |
| `4dc00cb880af4bdd28149f1e9628683ed6783b6eb1c8cfd3941779309c28b8e6` | 1830 | `c/README.md` |
| `d453c678f19b2d4f6378eed640ac328bf914a71d9d0e331e698bc64c5ca1339a` | 4980 | `c/examples/demo.c` |
| `e4d4446e136b8b03c95dbb5a53ea4884db0351491a235e7c4e63eb4ad68ffb54` | 16905 | `c/photonic_sort.c` |
| `16b795d5150d59e4d0bcf5a16182be041f351a3e8d0ecdc1fb667814b348e622` | 3497 | `c/photonic_sort.h` |
| `1637f73a1adfb88ce638a0102e1a921cae746ca4a4aaa861721f045e7b11fd59` | 4392 | `c/tests/test_photonic_sort.c` |
| `2bbdd552e0782d07c5d5f5d72983331f21d41b376c6ad0f530352be97589e89c` | 19688 | `photonic_sort.py` |
| `2e250763a95d26346a8eaa888aabb691556b08465f7c370de06b00d7af9c375d` | 1271 | `pyproject.toml` |
| `03197caa57e33f92e6aecae8a82dc4e5aa4e8286c637121652de494c49238798` | 3833 | `tests/test_photonic_sort.py` |

**GNU `sha256sum` block (copy into a file and run `sha256sum -c` against a tree extracted at `841882d`):**

```
c5a4e48ca579d8be5584b1f26b01cfbe753b59c1bba219be3d8f8be30921c61f  .gitignore
27bb8f20a000f0cae81eefa18d9c9c9e312199f4b26a6b428004fd475018911a  CITATION.cff
b7de01ba944dd7ef1fa8f9feef55769cdc9bfdfd9681c314e95d87bac9632521  LICENSE
da849e28e0ce605bacf9b26201f06f9ee6387d7bf3e1d33f587cef17dd11182a  README.md
7ec498f28fb3345e359fd6f7c406934b4a63ca115115b82af6fcfe70ce8bf6a1  RESEARCH.md
ff71d4adb893a87c72dc0c0fa33ef89b3fe5267bd044e930b9613db25c494715  c/Makefile
4dc00cb880af4bdd28149f1e9628683ed6783b6eb1c8cfd3941779309c28b8e6  c/README.md
d453c678f19b2d4f6378eed640ac328bf914a71d9d0e331e698bc64c5ca1339a  c/examples/demo.c
e4d4446e136b8b03c95dbb5a53ea4884db0351491a235e7c4e63eb4ad68ffb54  c/photonic_sort.c
16b795d5150d59e4d0bcf5a16182be041f351a3e8d0ecdc1fb667814b348e622  c/photonic_sort.h
1637f73a1adfb88ce638a0102e1a921cae746ca4a4aaa861721f045e7b11fd59  c/tests/test_photonic_sort.c
2bbdd552e0782d07c5d5f5d72983331f21d41b376c6ad0f530352be97589e89c  photonic_sort.py
2e250763a95d26346a8eaa888aabb691556b08465f7c370de06b00d7af9c375d  pyproject.toml
03197caa57e33f92e6aecae8a82dc4e5aa4e8286c637121652de494c49238798  tests/test_photonic_sort.py
```

### 2.2 Primary C implementation digests

```
e4d4446e136b8b03c95dbb5a53ea4884db0351491a235e7c4e63eb4ad68ffb54  c/photonic_sort.c
16b795d5150d59e4d0bcf5a16182be041f351a3e8d0ecdc1fb667814b348e622  c/photonic_sort.h
ff71d4adb893a87c72dc0c0fa33ef89b3fe5267bd044e930b9613db25c494715  c/Makefile
d453c678f19b2d4f6378eed640ac328bf914a71d9d0e331e698bc64c5ca1339a  c/examples/demo.c
1637f73a1adfb88ce638a0102e1a921cae746ca4a4aaa861721f045e7b11fd59  c/tests/test_photonic_sort.c
```

### 2.3 Verify commands

```bash
# Live check of code artifacts on main (docs may differ; code must match)
sha256sum -c c/SHA256SUMS

# Per-file from implementation commit
git show 841882d:c/photonic_sort.c | sha256sum
# expect e4d4446e136b8b03c95dbb5a53ea4884db0351491a235e7c4e63eb4ad68ffb54

git show 841882d:c/photonic_sort.h | sha256sum
# expect 16b795d5150d59e4d0bcf5a16182be041f351a3e8d0ecdc1fb667814b348e622

# Full tree @ 841882d
git archive --format=tar 841882d | tar -x -C /tmp/ps-v
cd /tmp/ps-v && sha256sum -c /path/to/SHA256SUMS_v1.0.1-c.txt
```


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
