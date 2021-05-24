# Pedro Lucas de Moliner de Castro - 11795784
# Matheus Henrique de Cerqueira Pinto - 11911104

CC = gcc
INCLUDES = ./includes
SOURCES = ./source/*.c
BINARY = ./main
PROG = ./main.c
ZIP = ./G19.zip

all:
	$(CC) -g -o $(BINARY) $(PROG) $(SOURCES) -I$(INCLUDES) -Wall -Werror

run:
	$(BINARY)

clean:
	rm $(BINARY) $(ZIP) *.bin *.csv *.my

zip:
	zip $(ZIP) $(PROG) $(INCLUDES)/* $(SOURCES) Makefile
