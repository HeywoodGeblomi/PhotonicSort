CXX ?= g++
CXXFLAGS ?= -O3 -std=c++17 -DNDEBUG
INCLUDES = -I. -I../residual -Ibaselines

.PHONY: all clean run

all: harness

harness: harness.cpp ../residual/*.hpp baselines/*
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o harness harness.cpp

run: harness
	./harness --n 1000000 --reps 5 --out results --arch-tag local

clean:
	rm -f harness
	rm -rf results
