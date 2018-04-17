CC = gcc
FLAGS = -g -Wall -O2
OPTS  = -L../ -I../
LINKS = -ljson

SRC = $(shell find . -name \*.c)

all:
	$(CC) -o main $(SRC) $(OPTS) $(FLAGS) $(LINKS)

run:
	@LD_LIBRARY_PATH=../ ./main
