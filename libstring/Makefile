CC    = gcc
FLAGS = -g -Wall

TARGET = libstring.so

SRC = $(filter-out \
			./test/%c, \
			$(shell find . -name \*.c))

$(TARGET):$(SRC)
	$(CC) -shared -fPIC -o $@ $^ $(FLAGS)
