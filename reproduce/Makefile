CXX ?= g++
CXXFLAGS ?= -O3 -std=c++17 -DNDEBUG
INCLUDES = -I. -I../residual -Ibaselines

.PHONY: all clean run run-i32 harness harness_multitype expanded run-expanded run-expanded-i32 run-expanded-u32 run-expanded-i64 sp-money-shot sp-stat-sig

all: harness

# Wave 0 — int64 independent reproduction (locked residual floors)
harness: harness.cpp ../residual/*.hpp baselines/*
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o harness harness.cpp

run: harness
	./harness --n 1000000 --reps 5 --out results --arch-tag local

# Wave 2 — multi-type (int32 / uint32)
harness_multitype: harness_multitype.cpp ../residual/*.hpp baselines/*
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o harness_multitype harness_multitype.cpp

run-i32: harness_multitype
	./harness_multitype --type i32 --n 1000000 --reps 5 --out results_i32 --arch-tag local

run-u32: harness_multitype
	./harness_multitype --type u32 --n 1000000 --reps 5 --out results_u32 --arch-tag local

# Expanded Field Suite (P01–P23) — path-(a) field-level evidence
expanded:
	$(CXX) $(CXXFLAGS) -I.. -I../residual -Ibaselines \
	  -o expanded_field_bench ../scripts/expanded_field_bench.cpp

run-expanded: expanded
	./expanded_field_bench --n 1000000 --reps 5 --out expanded.csv
	python3 ../scripts/field_metrics.py expanded.csv

run-expanded-i32: expanded
	./expanded_field_bench --n 1000000 --reps 5 --only i32 --out expanded_i32.csv

run-expanded-u32: expanded
	./expanded_field_bench --n 1000000 --reps 5 --only u32 --out expanded_u32.csv

run-expanded-i64: expanded
	./expanded_field_bench --n 1000000 --reps 5 --only i64 --out expanded_i64.csv

# Secondary Parity Field-Level (Wave 0 SP)
sp-money-shot:
	$(CXX) $(CXXFLAGS) -DSECONDARY_PARITY -I.. -I../residual -Ibaselines \
	  -o suite_hybrid ../scripts/expanded_field_bench_hybrid.cpp
	./suite_hybrid --n 1000000 --reps 3 --out results_sp.csv
	python3 ../scripts/sp_money_shot_gate.py results_sp.csv

sp-stat-sig:
	$(CXX) $(CXXFLAGS) -DSECONDARY_PARITY -I.. -I../residual -Ibaselines \
	  -o suite_hybrid ../scripts/expanded_field_bench_hybrid.cpp
	./suite_hybrid --n 1000000 --reps 11 --out results_sp.csv --raw-out raw_sp.csv
	python3 ../scripts/sp_stat_sig_gate.py raw_sp.csv --min-reps 11

clean:
	rm -f harness harness_multitype expanded_field_bench suite_hybrid
	rm -rf results results_i32 results_u32 results_multitype expanded.csv expanded_*.csv results_sp.csv raw_sp.csv
