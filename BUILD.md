# Build instructions

PhotonicSort ships two layers:

| Layer | Path | Toolchain |
|-------|------|-----------|
| **C11 core** (recommended for performance) | [`c/`](./c/) | C11 compiler (`gcc` / `clang`) + `make` |
| **Python reference** | [`photonic_sort.py`](./photonic_sort.py) | Python ≥ 3.10 (stdlib only) |

No third-party libraries are required for either layer.

---

## Prerequisites

### C11 core

| Requirement | Notes |
|-------------|--------|
| C11 compiler | `gcc` 7+ or `clang` 6+ (`CC=clang make` works) |
| `make` | GNU Make or compatible |
| libc | ISO C library with `stdlib.h`, `string.h`, `stdint.h` |

Optional: `g++` if you rebuild the benchmark harness that compares against `std::sort` (not required for the library).

### Python reference

| Requirement | Notes |
|-------------|--------|
| Python | 3.10 or newer |
| pip / build (optional) | Only if installing as a package |

---

## C11 core

```bash
git clone https://github.com/HeywoodGeblomi/PhotonicSort.git
cd PhotonicSort/c
```

### Build library objects, demo, and tests

```bash
make            # builds demo + test_photonic_sort
make test       # run unit tests
./demo          # small timing / path-routing demo
```

Default flags:

```text
-O3 -std=c11 -Wall -Wextra -Wpedantic -fno-strict-aliasing
```

### Release build

```bash
make release    # clean + -DNDEBUG -O3 + rebuild + test
```

### Alternate compiler

```bash
make clean all test CC=clang
# or
make clean all test CC=gcc-13
```

### Machine-local peak (optional)

Portable releases should **omit** these. For local microbenchmarks only:

```bash
make clean all CFLAGS="-O3 -std=c11 -march=native -flto -Wall -Wextra -fno-strict-aliasing -I."
```

### Link into your program

```bash
# Compile the single translation unit once
$(CC) -O3 -std=c11 -c photonic_sort.c -o photonic_sort.o

# Link with your code
$(CC) -O3 -std=c11 your_app.c photonic_sort.o -o your_app
```

Header:

```c
#include "photonic_sort.h"   /* add -I path/to/PhotonicSort/c when compiling */
```

There is no install target yet; copy `photonic_sort.h` + `photonic_sort.c` (or the `.o`) into your tree, or point `-I` / link flags at `c/`.

### Makefile targets

| Target | Action |
|--------|--------|
| `all` (default) | Build `demo` and `test_photonic_sort` |
| `test` | Build tests if needed and run them |
| `demo` | Build `./demo` |
| `demo-run` | Build and run `./demo` |
| `release` | `clean` + `-DNDEBUG` rebuild + `test` |
| `clean` | Remove objects and binaries |

### Windows (MSVC)

The primary path is Makefile + GCC/Clang (MinGW, MSYS2, or WSL recommended):

```bash
# MSYS2 / MinGW example
pacman -S mingw-w64-x86_64-gcc make
cd c
make
```

MSVC project files are not shipped. You can add `photonic_sort.c` to a Visual Studio project as a C file, define nothing special beyond a C11-capable toolset, and include `photonic_sort.h`.

### Troubleshooting (C)

| Symptom | Check |
|---------|--------|
| `restrict` errors under C++ | Compile as C, or `#define restrict` before including the header from C++ (see header `extern "C"` block) |
| Link errors for `photonic_sort_*` | Ensure `photonic_sort.o` is on the link line |
| Tests fail | Run `make clean all test`; confirm `sizeof(int64_t) == 8` |

---

## Python reference

### Run without installing

```bash
git clone https://github.com/HeywoodGeblomi/PhotonicSort.git
cd PhotonicSort

python3 photonic_sort.py                    # demo / self-check
python3 -m unittest discover -s tests -v    # unit tests
```

### Import from the repo root

```bash
cd PhotonicSort
python3 -c "from photonic_sort import photonic_sort; print(photonic_sort([3,1,2]))"
```

### Install editable (optional)

```bash
python3 -m pip install -e .
photonic-sort-demo    # console script from pyproject.toml
```

Dependencies: **none** beyond the standard library.

### Troubleshooting (Python)

| Symptom | Check |
|---------|--------|
| `ModuleNotFoundError: photonic_sort` | Run from repo root, or `pip install -e .` |
| Import works but tests missing | Ensure `tests/` is present; use `python3 -m unittest discover -s tests -v` |

---

## Verify a build

After building:

```bash
# C
cd c && make test && ./demo

# Python
python3 -m unittest discover -s tests -v

# Optional digest checks (release artifacts)
# see VERIFY.md and scripts/verify-sha256.sh
```

Performance numbers and methodology: [BENCHMARKS.md](./BENCHMARKS.md).
