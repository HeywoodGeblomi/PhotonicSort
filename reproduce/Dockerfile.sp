# PhotonicSort Field-Level dual residual (DEFAULT talent path) — Wave 0 independent reproduction
#
# ZERO-NETWORK money-shot. All baselines are vendored under residual/baselines/.
# Do not curl at build time.
#
# From repository root:
#   docker build -f reproduce/Dockerfile.sp -t photonic-sp .
#   docker run --rm photonic-sp                    # money-shot R=3
#   docker run --rm -e STAT_SIG=1 photonic-sp      # formal R=11 CI gate
#
# Reference: docs/field_level/MULTI_ARCH_MONEY_SHOT_LOCKED.md
#            docs/field_level/STAT_SIG_PROTOCOL.md
# EXTERNAL-clean. THE BEASTIE BOYZ 2026-08-23

FROM debian:bookworm-slim

RUN apt-get update && apt-get install -y --no-install-recommends \
        g++ make python3 \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /src

COPY residual/ residual/
COPY scripts/expanded_field_bench_hybrid.cpp scripts/expanded_field_bench_hybrid.cpp
COPY scripts/sp_money_shot_gate.py scripts/sp_money_shot_gate.py
COPY scripts/sp_stat_sig_gate.py scripts/sp_stat_sig_gate.py
COPY reproduce/run_sp.sh reproduce/run_sp.sh

# Vendored baselines only — zero network. Place on residual include path.
RUN test -f residual/baselines/pdqsort.h \
 && test -f residual/baselines/ska_sort.hpp \
 && cp residual/baselines/pdqsort.h residual/pdqsort.h \
 && cp residual/baselines/ska_sort.hpp residual/ska_sort.hpp

RUN g++ -O3 -std=c++17 -DNDEBUG \
      -I. -Iresidual -Iresidual/baselines \
      -o suite_hybrid scripts/expanded_field_bench_hybrid.cpp \
 && chmod +x reproduce/run_sp.sh

ENV STAT_SIG=0
ENV N=1000000

CMD ["bash", "reproduce/run_sp.sh"]
