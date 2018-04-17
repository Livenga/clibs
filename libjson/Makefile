CC = gcc
FLAGS = -O2 -Wall# -g

TARGET = libjson.so

SRC = $(filter-out \
			./test/%.c, \
			$(shell find . -name \*.c))

$(TARGET):$(SRC)
	$(CC) -fPIC -shared -o $@ $^


clean:
	[ ! -d $(TARGET) ] || rm $(TARGET)

all:
	make clean
	make

