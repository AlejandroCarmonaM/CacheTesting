default: peak-mem-cache-gcc

PROGRAMS=peak-flops peak-mem peak-freq \
	peak-mem-cache \
	peak-mem-random-read-64-a peak-mem-random-read-64-b \
	peak-mem-random-write-64 peak-mem-seq-copy64 \
	peak-mem-seq-read-64 peak-mem-seq-write-64 \
	peak-mem-seq-copy-128

SOURCES_COMMON_CPP=util.cpp
SOURCES_COMMON_H=util.h
SOURCES_COMMON=$(SOURCES_COMMON_CPP) $(SOURCES_COMMON_H)

all: $(foreach prog,$(PROGRAMS),$(prog)-gcc $(prog)-clang $(prog)-icc)

%-gcc: %.cpp $(SOURCES_COMMON)
	g++ -g -std=c++20 -Wall -march=native -mtune=native -O3 -ffast-math -fopenmp $(SOURCES_COMMON_CPP) $*.cpp -o $@

%-clang: %.cpp $(SOURCES_COMMON)
	clang++ -g -std=c++20 -Wall -march=native -mtune=native -O3 -ffast-math -fopenmp $(SOURCES_COMMON_CPP) $*.cpp -o $@

%-icc: %.cpp $(SOURCES_COMMON)
	icpc -g -std=c++20 -Wall -xHost -O2 -qopenmp $(SOURCES_COMMON_CPP) $*.cpp -o $@

%-debug: %.cpp $(SOURCES_COMMON)
	g++ -g -std=c++20 -Wall -O0 -fopenmp $(SOURCES_COMMON_CPP) $*.cpp -o $@

.PHONY: clean
clean:
	rm -f $(foreach prog,$(PROGRAMS),$(prog)-gcc $(prog)-debug $(prog)-icc $(prog)-clang)

