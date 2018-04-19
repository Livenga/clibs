CC         = gcc
TARGET     = string_test
LINK_PATHS = ..
LINK       = -lstring

LINK_PATH = $(addprefix -L, $(LINK_PATHS))
SRC       = $(shell find . -name \*.c)


$(TARGET):$(SRC)
	$(CC) -o $@ $^ $(LINK_PATH) $(LINK) #-D_ERROR

run:$(TARGET)
	LD_LIBRARY_PATH=.. ./$^
