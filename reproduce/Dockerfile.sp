# PhotonicSort Field-Level dual residual (DEFAULT talent path) — Wave 0 independent reproduction
#
# From repository root:
#   docker build -f reproduce/Dockerfile.sp -t photonic-sp .
#   docker run --rm photonic-sp                    # money-shot R=3
#   docker run --rm -e STAT_SIG=1 photonic-sp      # formal R=11 CI gate
#
# Reference: docs/field_level/MULTI_ARCH_MONEY_SHOT_LOCKED.md
#            docs/field_level/STAT_SIG_PROTOCOL.md
# EXTERNAL-clean. THE BEASTIE BOYZ 2026-08-15

FROM debian:bookworm-slim

RUN apt-get update && apt-get install -y --no-install-recommends \
        g++ make ca-certificates curl python3 \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /src

COPY residual/ residual/
COPY scripts/expanded_field_bench_hybrid.cpp scripts/expanded_field_bench_hybrid.cpp
COPY scripts/sp_money_shot_gate.py scripts/sp_money_shot_gate.py
COPY scripts/sp_stat_sig_gate.py scripts/sp_stat_sig_gate.py
COPY reproduce/run_sp.sh reproduce/run_sp.sh

RUN mkdir -p baselines \
 && curl -fsSL -o baselines/pdqsort.h \
      https://raw.githubusercontent.com/orlp/pdqsort/master/pdqsort.h \
 && curl -fsSL -o baselines/ska_sort.hpp \
      https://raw.githubusercontent.com/skarupke/ska_sort/master/ska_sort.hpp \
 && cp baselines/ska_sort.hpp residual/ska_sort.hpp

RUN g++ -O3 -std=c++17 -DNDEBUG \
      -I. -Iresidual -Ibaselines \
      -o suite_hybrid scripts/expanded_field_bench_hybrid.cpp \
 && chmod +x reproduce/run_sp.sh

ENV STAT_SIG=0
ENV N=1000000

CMD ["bash", "reproduce/run_sp.sh"]
