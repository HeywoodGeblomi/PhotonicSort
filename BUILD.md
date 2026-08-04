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
make            # builds lib + demo + test_photonic_sort
make test       # run unit tests
./demo          # small timing / path-routing demo
```

Default flags (safe performance — no `-Ofast` / no fast-math):

```text
-O3 -std=c11 -Wall -Wextra -Wpedantic
-fno-strict-aliasing -fomit-frame-pointer -pipe
-ffunction-sections -fdata-sections
-Wl,--gc-sections          # at link time
```

### Release build (portable, recommended)

```bash
make release    # clean + -DNDEBUG -O3 -flto + lib + demo + test
```

Produces `libphotonic_sort.a`, `demo`, and `test_photonic_sort`.

### Alternate compiler / toggles

```bash
make clean all test CC=clang
make clean all test LTO=1          # link-time optimization
make native                        # -march=native -mtune=native -flto (local only)
```

### Machine-local peak

Portable releases and Docker images **omit** `-march=native`. For local microbenchmarks:

```bash
make native
```

### Link into your program

```bash
cd c
make lib
$(CC) -O3 -std=c11 your_app.c -I. -L. -lphotonic_sort -o your_app
```

Header: `#include "photonic_sort.h"` with `-I path/to/PhotonicSort/c`.

### Install (optional)

```bash
sudo make install PREFIX=/usr/local
```

### Makefile targets

| Target | Action |
|--------|--------|
| `all` | Static lib + demo + tests binary |
| `lib` | `libphotonic_sort.a` |
| `test` | Build and run unit tests |
| `release` | Portable `-DNDEBUG -flto` rebuild + test |
| `native` | Local CPU peak |
| `install` | Header + static lib to `PREFIX` |
| `clean` / `help` | Clean / list options |

### Windows

MinGW / MSYS2 / WSL recommended (`make`). MSVC: add `photonic_sort.c` to a C project and include the header.

---

## Python reference

```bash
cd PhotonicSort
python3 photonic_sort.py
python3 -m unittest discover -s tests -v
python3 -m pip install -e .    # optional
```

---

## Docker

Multi-stage `Dockerfile` at repo root builds the **C11 release profile** (`make release`: `-O3 -DNDEBUG -flto`), runs tests, and optionally the Python suite.

### Build

```bash
docker build -t photonicsort .
docker build --target test -t photonicsort:test .
docker build --target python-ref -t photonicsort:python .
```

### Run

```bash
docker run --rm photonicsort           # C tests + demo
docker run --rm photonicsort:test      # C tests only
docker run --rm photonicsort:python    # Python demo
```

### Artifacts in the default image

| Path | Content |
|------|---------|
| `/opt/photonicsort/demo` | C demo |
| `/opt/photonicsort/test_photonic_sort` | C unit tests |
| `/opt/photonicsort/lib/libphotonic_sort.a` | Static library |
| `/opt/photonicsort/include/photonic_sort.h` | Public header |
| `/opt/photonicsort/src/photonic_sort.c` | Source |

```bash
docker create --name ps photonicsort
docker cp ps:/opt/photonicsort/lib/libphotonic_sort.a .
docker cp ps:/opt/photonicsort/include/photonic_sort.h .
docker rm ps
```

Docker uses the **portable** flag set (no `-march=native`). Bases: `debian:bookworm-slim`, `python:3.12-slim-bookworm`.

---

## Verify

```bash
cd c && make test && ./demo
python3 -m unittest discover -s tests -v
docker build -t photonicsort . && docker run --rm photonicsort
```

Performance methodology: [BENCHMARKS.md](./BENCHMARKS.md).
