#!/bin/bash
# Wave 0 SP Field-Level independent reproduction entrypoint
# STAT_SIG=1 → R=11 + bootstrap CI gate; default money-shot R=3
set -euo pipefail
N="${N:-1000000}"
if [ "${STAT_SIG:-0}" = "1" ]; then
  REPS="${REPS:-11}"
  echo "# mode=stat-sig n=$N reps=$REPS"
  ./suite_hybrid --n "$N" --reps "$REPS" \
    --out results_sp.csv --raw-out raw_sp.csv | tee results.txt
  python3 scripts/sp_stat_sig_gate.py raw_sp.csv \
    --threshold 1.20 --bootstrap 2000 --min-reps "$REPS" | tee gate.txt
else
  REPS="${REPS:-3}"
  echo "# mode=money-shot n=$N reps=$REPS"
  ./suite_hybrid --n "$N" --reps "$REPS" \
    --out results_sp.csv | tee results.txt
  python3 scripts/sp_money_shot_gate.py results_sp.csv \
    --threshold 1.20 | tee gate.txt
fi
echo "# done — compare ratios, not absolute ms"
