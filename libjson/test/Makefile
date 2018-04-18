CC        = gcc
FLAGS     = -g -Wall -O2

LINK_PATH = ..\
						../../libstringbuilder/

INCLUDE_PATH = ..

LINKS = -ljson -lstringbuilder
#SRC   = $(shell find . -name \*.c)
SRC   = main.c

all:
	$(CC) -o main $(SRC) \
		$(addprefix -L, $(LINK_PATH)) $(addprefix -I, $(INCLUDE_PATH)) \
		$(FLAGS) $(LINKS) 

run:
	@LD_LIBRARY_PATH=../:../../libstringbuilder/ ./main
