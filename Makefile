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
