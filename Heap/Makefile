G = g++
GFLAGS = -g -Wall
OBJECTS = test.o heap.o
EFILE = test.exe

all: test

test: $(OBJECTS)
	$(G) $(GFLAGS) $^ -o $@

test.o: test.cpp 
	$(G) $(GFLAGS) $^ -o $@ -c

heap.o: heap.cpp heap.h
	$(G) $(GFLAGS) $< -o $@ -c

.PHONY: clean
clean: 
	rm -rf $(OBJECTS) $(EFILE)
	echo "All cleaned!"