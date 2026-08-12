# PhotonicSort independent reproduction — pure residual vs std/pdq/ska
#
# Build from repo root:
#   docker build -f reproduce/Dockerfile -t photonic-reproduce .
# Run:
#   docker run --rm photonic-reproduce
#
# Compare ratios (not absolute ms) to docs/phase0_field/PHASE1_LOW_DISORDER.md
# EXTERNAL-clean pure residual only. Not field-level.
# THE BEASTIE BOYZ — Wave 0 complete

FROM debian:bookworm-slim

RUN apt-get update && apt-get install -y --no-install-recommends \
        g++ make ca-certificates curl \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /src

# Residual headers from repo (build context = repo root)
COPY residual/ residual/

# Baselines: fetch upstream at build time (zlib / BSL-1.0). No large vendored blobs.
RUN mkdir -p baselines \
 && curl -fsSL -o baselines/pdqsort.h https://raw.githubusercontent.com/orlp/pdqsort/master/pdqsort.h \
 && curl -fsSL -o baselines/ska_sort.hpp https://raw.githubusercontent.com/skarupke/ska_sort/master/ska_sort.hpp

COPY reproduce/harness.cpp ./

# Flat layout Makefile (residual/ + baselines/ next to harness)
RUN printf '%s\n' \
  'CXX ?= g++' \
  'CXXFLAGS ?= -O3 -std=c++17 -DNDEBUG' \
  'INCLUDES = -I. -Iresidual -Ibaselines' \
  'all: harness' \
  'harness: harness.cpp residual/*.hpp baselines/pdqsort.h baselines/ska_sort.hpp' \
  '	$(CXX) $(CXXFLAGS) $(INCLUDES) -o harness harness.cpp' \
  > Makefile

RUN make harness

CMD ./harness --n 1000000 --reps 5 --out results --arch-tag docker \
 && echo "" \
 && cat results/gap_map.md
