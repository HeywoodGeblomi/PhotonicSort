CXX ?= g++
CXXFLAGS ?= -O3 -std=c++17 -DNDEBUG
INCLUDES = -I. -I../residual -Ibaselines

.PHONY: all clean run run-i32 harness harness_multitype

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

clean:
	rm -f harness harness_multitype
	rm -rf results results_i32 results_u32 results_multitype
