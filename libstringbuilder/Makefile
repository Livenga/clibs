CC     = gcc
FLAGS  = -g -Wall

TARGET = libstringbuilder.so
OBJDIR = objs

SOURCE = $(filter-out \
				 ./test/%.c, \
				 $(shell find . -name \*.c))

default:
	make $(TARGET)

$(TARGET):$(SOURCE)
	$(CC) -shared -fPIC -o $@ $^ $(FLAGS)


clear:
	rm $(TARGET)

all:
	make clear default
