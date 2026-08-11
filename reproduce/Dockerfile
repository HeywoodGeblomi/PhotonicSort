# PhotonicSort independent reproduction — pure residual vs std/pdq/ska
#
# Build:  docker build -t photonic-reproduce .
# Run:    docker run --rm photonic-reproduce
# Mount:  docker run --rm -v "$PWD/out:/out" photonic-reproduce \
#           sh -c "./harness --n 1000000 --reps 5 --out /out"
#
# Compare ratios (not absolute ms) to docs/phase0_field/PHASE1 reference.
# EXTERNAL-clean pure residual only. Not field-level.
# THE BEASTIE BOYZ — Wave 0

FROM debian:bookworm-slim

RUN apt-get update && apt-get install -y --no-install-recommends \
        g++ make ca-certificates \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /reproduce

COPY baselines/ baselines/
COPY residual/ residual/
COPY harness.cpp Makefile ./

RUN make harness

# Default: run standard suite @ n=1e6, 5 reps
CMD ./harness --n 1000000 --reps 5 --out results --arch-tag docker \
 && echo "" \
 && cat results/gap_map.md
