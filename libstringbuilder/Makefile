CC     = gcc
FLAGS  = -g -Wall

TARGET = libstringbuilder.so
OBJDIR = objs

SOURCE = $(shell find src/ -name \*.c)

default:
	make $(TARGET)

$(TARGET):$(SOURCE)
	$(CC) -shared -fPIC -o $@ $^ $(FLAGS)


clear:
	rm $(TARGET)

all:
	make clear default
