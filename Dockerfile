# PhotonicSort — C11 core + Python reference
# Build:  docker build -t photonicsort .
# Test:   docker build --target test -t photonicsort:test .
# Demo:   docker run --rm photonicsort

ARG PYTHON_IMAGE=python:3.12-slim-bookworm

# ---------------------------------------------------------------------------
# Stage: C toolchain
# ---------------------------------------------------------------------------
FROM debian:bookworm-slim AS c-builder

RUN apt-get update && apt-get install -y --no-install-recommends \
        gcc make libc6-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /src
COPY c/ /src/c/

WORKDIR /src/c
# Portable release profile: -O3 -DNDEBUG -flto, no -march=native
RUN make release

# ---------------------------------------------------------------------------
# Stage: run C tests only (CI-friendly)
# ---------------------------------------------------------------------------
FROM c-builder AS test
RUN make test && ./demo
CMD ["./test_photonic_sort"]

# ---------------------------------------------------------------------------
# Stage: Python reference (stdlib only)
# ---------------------------------------------------------------------------
FROM ${PYTHON_IMAGE} AS python-ref
WORKDIR /app
COPY photonic_sort.py pyproject.toml ./
COPY tests/ ./tests/
RUN python -m unittest discover -s tests -v
CMD ["python", "photonic_sort.py"]

# ---------------------------------------------------------------------------
# Stage: default runtime image (C demo + static lib artifacts)
# ---------------------------------------------------------------------------
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
