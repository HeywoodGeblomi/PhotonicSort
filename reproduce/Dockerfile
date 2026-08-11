# PhotonicSort independent reproduction — pure residual vs std/pdq/ska
#
# Build from repo root:
#   docker build -f reproduce/Dockerfile -t photonic-reproduce .
# Run:
#   docker run --rm photonic-reproduce
#
# Compare ratios (not absolute ms) to docs/phase0_field/PHASE1 reference.
# EXTERNAL-clean pure residual only. Not field-level.
# THE BEASTIE BOYZ — Wave 0

FROM debian:bookworm-slim

RUN apt-get update && apt-get install -y --no-install-recommends \
        g++ make ca-certificates \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /src

# Residual headers from repo (build context = repo root)
COPY residual/ residual/
COPY reproduce/baselines/ baselines/
COPY reproduce/harness.cpp reproduce/Makefile ./

# Makefile expects ../residual when run from reproduce/; adjust for flat layout
RUN printf '%s\n' \
  'CXX ?= g++' \
  'CXXFLAGS ?= -O3 -std=c++17 -DNDEBUG' \
  'INCLUDES = -I. -Iresidual -Ibaselines' \
  'all: harness' \
  'harness: harness.cpp residual/*.hpp baselines/*' \
  '\t$$(CXX) $$(CXXFLAGS) $$(INCLUDES) -o harness harness.cpp' \
  > Makefile

RUN make harness

CMD ./harness --n 1000000 --reps 5 --out results --arch-tag docker \
 && echo "" \
 && cat results/gap_map.md
