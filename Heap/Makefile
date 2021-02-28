G = g++
GFLAGS = -g -Wall

all: test

test: test.o heap.o
	$(G) $(GFLAGS) $^ -o $@

test.o: test.cpp
	$(G) $(GFLAGS) $^ -o $@ 

heap.o: heap.cpp heap.h
	$(G) $(GFLAGS) $^ -o $@

.PHONY: clean
clean: 
	rm -f *.o
	rm -f test
	echo All cleaned!