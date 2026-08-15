# Build instructions

PhotonicSort ships multiple layers. The **default production sorter** for Field-Level claims is the C++ hybrid residual (dual residual / residual talent).

| Layer | Path | Toolchain | Notes |
|-------|------|-----------|-------|
| **C++ hybrid residual (DEFAULT)** | [`residual/`](./residual/) | C++17 + CMake | Dual residual on by default |
| **C11 core** | [`c/`](./c/) | C11 + make | Solid portable core; not the dual residual path |
| **Python reference** | [`photonic_sort.py`](./photonic_sort.py) | Python ≥ 3.10 | Pure-Python; `pip install .` |
| **Rust** | [`rust/`](./rust/) | cargo | C11 path via photonic-sort-sys |

---

## C++ hybrid residual (stranger path)

```bash
git clone https://github.com/HeywoodGeblomi/PhotonicSort.git && cd PhotonicSort
mkdir -p baselines
curl -fsSL -o baselines/pdqsort.h \
  https://raw.githubusercontent.com/orlp/pdqsort/master/pdqsort.h
curl -fsSL -o baselines/ska_sort.hpp \
  https://raw.githubusercontent.com/skarupke/ska_sort/master/ska_sort.hpp

cmake -B build -DCMAKE_INSTALL_PREFIX=$HOME/.local
cmake --build build          # builds examples/hybrid_sort if baselines present
cmake --install build
```

```cpp
#include <PhotonicSort/hybrid_residual_menu.hpp>
hybrid_residual::sort_i64(a, n);  // DEFAULT dual residual
```

Options:
- `-DCLASSICAL_RESIDUAL=ON` — escape hatch (unconditional ska on mid-band HE)
- `-DPHOTONIC_BUILD_EXAMPLES=OFF` — skip example target

No third-party libraries beyond the two baseline headers above.

---

## C11 core

```bash
git clone https://github.com/HeywoodGeblomi/PhotonicSort.git
cd PhotonicSort/c
make && make test && ./demo
```

```bash
make release    # -DNDEBUG -O3 -flto (dynamic, portable)
make static     # fully static (-static) for scratch/distroless
make native     # -march=native -flto (local only)
make lib && make install PREFIX=/usr/local
```

Default flags: `-O3 -fomit-frame-pointer -ffunction-sections -Wl,--gc-sections` (no `-Ofast`).

---

## Python reference

```bash
python3 photonic_sort.py
python3 -m unittest discover -s tests -v
```

---

## Docker

Multi-stage `Dockerfile` (BuildKit). Stages are separate machines; `COPY --from` moves artifacts; `--target` selects which stage to tag; the final image is the last `FROM` unless `--target` overrides.

```text
c-builder ─────────────► test
       │                   runtime          (default tag)
       │
c-builder-static ──────► test-static
       ├────────────────► runtime-static    (scratch)
       ├────────────────► runtime-distroless
       └────────────────► lib-export        (.a + .h only)

python-ref   (parallel; no C toolchain)
```

```bash
export DOCKER_BUILDKIT=1
```

### Build targets

```bash
docker build -t photonicsort .                              # default runtime
docker build --target test -t photonicsort:test .
docker build --target test-static -t photonicsort:test-static .
docker build --target python-ref -t photonicsort:python .
docker build --target lib-export -t photonicsort:lib .      # .a + .h only
docker build --target runtime-static -t photonicsort:static .
docker build --target runtime-distroless -t photonicsort:distroless .
```

### Run

```bash
docker run --rm photonicsort
docker run --rm photonicsort:test
docker run --rm photonicsort:static
docker run --rm photonicsort:python
```

### Extract the static library (`lib-export`)

```bash
docker build --target lib-export -t photonicsort:lib .
docker create --name ps-lib photonicsort:lib
docker cp ps-lib:/libphotonic_sort.a .
docker cp ps-lib:/photonic_sort.h .
docker rm ps-lib
```

### BuildKit cache mounts

C builder stages use `RUN --mount=type=cache` for apt (Debian) and apk (Alpine). Package downloads stay on the **builder host** across rebuilds and are **not** baked into image layers.

### Static / scratch / distroless

| Stage | Base | Linkage | Use when |
|-------|------|---------|----------|
| `runtime` | `debian:bookworm-slim` | dynamic glibc | Default; shell + easy debug |
| `runtime-static` | `scratch` | fully static (musl) | Minimal attack surface |
| `runtime-distroless` | `gcr.io/distroless/static-debian12` | fully static | Distroless policy environments |
| `lib-export` | `scratch` | static `.a` | Depend on PhotonicSort from other builds |

Static path: Alpine `c-builder-static` runs `make static`.

### Provenance and SBOM

```bash
docker buildx create --name photonicsort-builder --use --driver docker-container 2>/dev/null \
  || docker buildx use photonicsort-builder

docker buildx build \
  --sbom=true \
  --provenance=mode=max \
  -t photonicsort:prov \
  --load \
  .

docker buildx build \
  --target runtime-static \
  --sbom=true \
  --provenance=mode=max \
  -t photonicsort:static-prov \
  --load \
  .

# Registry example:
# docker buildx build --sbom=true --provenance=mode=max \
#   -t ghcr.io/you/photonicsort:1.0.1 --push .
```

---

## Verify

```bash
cd c && make test && ./demo
cd c && make static && make test
python3 -m unittest discover -s tests -v
docker build -t photonicsort . && docker run --rm photonicsort
```

Performance methodology: [BENCHMARKS.md](./BENCHMARKS.md).
