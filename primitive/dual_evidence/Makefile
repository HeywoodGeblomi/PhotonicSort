# Build libdual_evidence.so for ctypes bind (B1/B2)
CXX ?= g++
CXXFLAGS ?= -O2 -std=c++17 -fPIC

libdual_evidence.so: dual_evidence_c.cpp dual_evidence_c.h spike_agent_gate.hpp dual_evidence.hpp
	$(CXX) $(CXXFLAGS) -shared -o $@ dual_evidence_c.cpp

.PHONY: clean test
clean:
	rm -f libdual_evidence.so

test: libdual_evidence.so
	python3 -c "import de_cpp; print(de_cpp.dual_choose(50,0.0,200,0.0)); print('OK')"
