# PhotonicSort — multi-stage Docker build
#
# Stages (targets):
#   c-builder       — Debian + gcc, dynamic release build (cache-mounted apt)
#   c-builder-static— Alpine + musl, fully static release
#   test            — C unit tests (from c-builder)
#   test-static     — C unit tests (static binaries)
#   python-ref      — Python reference unit tests
#   lib-export      — scratch: libphotonic_sort.a + photonic_sort.h only
#   runtime         — debian slim + dynamic artifacts (pragmatic default)
#   runtime-static  — scratch + fully static demo/test (zero libc in image)
#   runtime-distroless — distroless/static + static binaries
#
# Examples:
#   docker build -t photonicsort .
#   docker build --target lib-export -t photonicsort:lib .
#   docker build --target runtime-static -t photonicsort:static .
#   docker buildx build --sbom=true --provenance=true -t photonicsort:prov --load .

# syntax=docker/dockerfile:1.7

ARG PYTHON_IMAGE=python:3.12-slim-bookworm
ARG DISTROLESS_IMAGE=gcr.io/distroless/static-debian12

# =============================================================================
# Stage: c-builder — dynamic C11 release (glibc)
# =============================================================================
FROM debian:bookworm-slim AS c-builder

# BuildKit cache mounts: apt lists/archives persist across builds on the same builder
RUN --mount=type=cache,target=/var/cache/apt,sharing=locked \
    --mount=type=cache,target=/var/lib/apt,sharing=locked \
    rm -f /etc/apt/apt.conf.d/docker-clean \
    && apt-get update \
    && DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends \
        gcc make libc6-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /src
COPY c/ /src/c/
WORKDIR /src/c

# Portable dynamic release: -O3 -DNDEBUG -flto (no -march=native)
RUN make release

# =============================================================================
# Stage: c-builder-static — fully static C11 release (musl)
# =============================================================================
FROM alpine:3.20 AS c-builder-static

RUN --mount=type=cache,target=/var/cache/apk,sharing=locked \
    apk add --no-cache gcc make musl-dev

WORKDIR /src
COPY c/ /src/c/
WORKDIR /src/c

# Fully static: works on scratch / distroless-static
RUN make static

# =============================================================================
# Stage: test — dynamic C tests (CI)
# =============================================================================
FROM c-builder AS test
RUN make test && ./demo
CMD ["./test_photonic_sort"]

# =============================================================================
# Stage: test-static — static C tests (CI)
# =============================================================================
FROM c-builder-static AS test-static
RUN make test && ./demo
CMD ["./test_photonic_sort"]

# =============================================================================
# Stage: python-ref — Python reference tests
# =============================================================================
FROM ${PYTHON_IMAGE} AS python-ref
WORKDIR /app
COPY photonic_sort.py pyproject.toml ./
COPY tests/ ./tests/
RUN python -m unittest discover -s tests -v
CMD ["python", "photonic_sort.py"]

# =============================================================================
# Stage: lib-export — ship only static library + header (for other projects)
# =============================================================================
# Use: docker build --target lib-export -t photonicsort:lib .
#      docker create --name ps-lib photonicsort:lib
#      docker cp ps-lib:/libphotonic_sort.a . && docker cp ps-lib:/photonic_sort.h .
FROM scratch AS lib-export
COPY --from=c-builder-static /src/c/libphotonic_sort.a /libphotonic_sort.a
COPY --from=c-builder-static /src/c/photonic_sort.h /photonic_sort.h
# scratch has no shell; consumers copy files out via `docker cp` or use as build context

# =============================================================================
# Stage: runtime — pragmatic default (glibc dynamic, slim)
# =============================================================================
FROM debian:bookworm-slim AS runtime
WORKDIR /opt/photonicsort
COPY --from=c-builder /src/c/demo /opt/photonicsort/demo
COPY --from=c-builder /src/c/test_photonic_sort /opt/photonicsort/test_photonic_sort
COPY --from=c-builder /src/c/libphotonic_sort.a /opt/photonicsort/lib/libphotonic_sort.a
COPY --from=c-builder /src/c/photonic_sort.h /opt/photonicsort/include/photonic_sort.h
COPY --from=c-builder /src/c/photonic_sort.c /opt/photonicsort/src/photonic_sort.c

RUN printf '%s\n' \
  '#!/bin/sh' \
  'set -e' \
  'echo "== PhotonicSort C tests =="' \
  '/opt/photonicsort/test_photonic_sort' \
  'echo' \
  'echo "== PhotonicSort C demo =="' \
  '/opt/photonicsort/demo' \
  > /usr/local/bin/photonicsort-check \
  && chmod +x /usr/local/bin/photonicsort-check

CMD ["photonicsort-check"]

# =============================================================================
# Stage: runtime-static — scratch (fully static binaries, no OS packages)
# =============================================================================
FROM scratch AS runtime-static
COPY --from=c-builder-static /src/c/demo /demo
COPY --from=c-builder-static /src/c/test_photonic_sort /test_photonic_sort
COPY --from=c-builder-static /src/c/libphotonic_sort.a /lib/libphotonic_sort.a
COPY --from=c-builder-static /src/c/photonic_sort.h /include/photonic_sort.h
# Default: run unit tests (no shell in scratch — exec binary directly)
CMD ["/test_photonic_sort"]

# =============================================================================
# Stage: runtime-distroless — Google distroless/static + static binaries
# =============================================================================
FROM ${DISTROLESS_IMAGE} AS runtime-distroless
COPY --from=c-builder-static /src/c/demo /demo
COPY --from=c-builder-static /src/c/test_photonic_sort /test_photonic_sort
COPY --from=c-builder-static /src/c/libphotonic_sort.a /lib/libphotonic_sort.a
COPY --from=c-builder-static /src/c/photonic_sort.h /include/photonic_sort.h
CMD ["/test_photonic_sort"]
