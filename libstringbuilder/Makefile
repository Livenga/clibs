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

static:$(SOURCE)
	$(CC) -c $(SOURCE) -static
	ar rcs -o libstringbuilder.a $(patsubst %.c, %.o, $^)

clear:
	rm $(TARGET)

all:
	make clear default
