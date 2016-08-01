CC=g++
CFLAGS=-Wall -pedantic -std=c++14
LFLAGS=-lOpenCL
PROG=openCLcpp_lister

all: $(PROG)

$(PROG): main.o
	$(CC) -o $@ $^ $(CFLAGS) $(LFLAGS)

%.o: %.cpp
	$(CC) -o $@ -c $^ $(CFLAGS)

.PHONY: clean javel

clean:
	rm *.o

javel: clean
	rm $(PROG)
