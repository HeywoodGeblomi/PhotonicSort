# PhotonicSort PO2 — one-command independent reproduction
#
# From repo root:
#   docker build -f reproduce/Dockerfile -t photonic-po2 .
#   docker run --rm photonic-po2
#
# Compare ratios (not absolute ms) to docs/phase0_field/REFERENCE_RATIOS_PO2.md
# EXTERNAL-clean pure residual. Not field-level.
# THE BEASTIE BOYZ — PO2 2026-08-12

FROM debian:bookworm-slim

RUN apt-get update && apt-get install -y --no-install-recommends \
        g++ make ca-certificates curl \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /src

COPY residual/ residual/

RUN mkdir -p baselines \
 && curl -fsSL -o baselines/pdqsort.h https://raw.githubusercontent.com/orlp/pdqsort/master/pdqsort.h

COPY reproduce/harness_po2.cpp ./harness.cpp

RUN printf '%s\n' \
  'CXX ?= g++' \
  'CXXFLAGS ?= -O3 -std=c++17 -DNDEBUG' \
  'INCLUDES = -I. -Iresidual -Ibaselines' \
  'all: harness' \
  'harness: harness.cpp residual/*.hpp residual/*.h baselines/pdqsort.h' \
  '\t$$(CXX) $$(CXXFLAGS) $$(INCLUDES) -o harness harness.cpp' \
  > Makefile

RUN make harness

CMD ["./harness", "--n", "1000000", "--reps", "7"]
