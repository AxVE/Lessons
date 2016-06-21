CC=g++
CFLAGS=-std=c++14 -W -Wall --pedantic -O0 -g 
LFAGS=
WORKPATH=$(abspath $(shell pwd))/
HEADERSPATH=-I $(WORKPATH)Includes/
WORKFLAGS=$(CFLAGS) $(HEADERSPATH)
SRC=Src/
OBJ=Obj/
BIN=Bin/
PROG=tree

all: prog

SOURCES=node.cpp main.cpp
OBJECTS=$(addsuffix .o, $(addprefix $(OBJ),$(basename $(notdir $(SOURCES)))))

prog: $(OBJECTS)
	$(CC) $(WORKFLAGS) -o $(BIN)$(PROG) $^ $(LFLAGS)

#$(OBJ)main.o:
#	$(CC) $(WORKFLAGS) -o $@ -c $^ main.cpp

$(OBJ)%.o: $(SRC)$(notdir %.cpp)
	$(CC) $(WORKFLAGS) -o $@ -c $^

.PHONY: clean javel

clean:
	rm $(OBJ)*.o

javel: clean
	rm $(BIN)*$(PROG)
