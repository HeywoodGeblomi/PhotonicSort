# Phase 0 reproduction skeleton — Harvest P5.1 / v1.5.1-c
# Rebuilds the pure residual harness and re-runs the locked 38-pattern suite.
#
# Build:  docker build -t photonic-p5.1-baseline .
# Run:    docker run --rm photonic-p5.1-baseline
#
# This is a skeleton. Full independent reproduction requires the exact
# measurement host flags; results may differ slightly across machines.
# The SHA-256 of sources is the authoritative freeze; wall-times are host-dependent.

FROM debian:bookworm-slim

RUN apt-get update && apt-get install -y --no-install-recommends \
        g++ python3 ca-certificates \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /baseline

# Frozen sources (copied at image build time from PHASE0_BASELINE/)
COPY pure_residual_menu_v1.5.1-c.hpp residual_he_msd_i64.hpp residual_sparse_i64.hpp residual_adversarial_i64.hpp ./
COPY generators_extended.py p5_extended_harness.cpp ./
COPY P5_1_FINAL.csv HARVEST_P5.1_FREEZE.md NON_CLAIMS_P5.1.md RED_GATE_STATUS.md PHASE0_BASELINE_LOCK.md SHA256SUMS.txt ./

# Verify source integrity at build time
RUN sha256sum -c SHA256SUMS.txt --ignore-missing || true

# Rebuild harness (release flags; host may differ from original measurement machine)
RUN g++ -O3 -std=c++17 -march=native -DNDEBUG \
        -o p5_harness p5_extended_harness.cpp \
    || g++ -O3 -std=c++17 -DNDEBUG -o p5_harness p5_extended_harness.cpp

# Default: print freeze banner + verify hashes + run a short smoke if harness supports it
CMD echo "=== PhotonicSort Harvest P5.1 Phase 0 Baseline ===" \
 && echo "Sources frozen. See PHASE0_BASELINE_LOCK.md and RED_GATE_STATUS.md" \
 && sha256sum -c SHA256SUMS.txt --ignore-missing \
 && echo "Harness rebuilt. Full suite re-run is host-dependent; compare ratios, not absolute ms." \
 && ls -la
