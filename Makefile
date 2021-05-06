CC=gcc
INCLUDES=./include
SOURCES=./source/*.c
BINARY=./main
PROG=./main.c
LIBS=-lm

all:
    $(CC) -g -o $(BINARY) $(PROG) $(SOURCES) -I$(INCLUDES) $(LIBS)

preproc:
    $(CC) -E $(PROG) $(SOURCES) -I$(INCLUDES) $(LIBS)

run:
    $(BINARY)