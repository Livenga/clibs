CC 		  		= gcc
FLAGS   		= -g -Wall -O2
LINK    		= 
INCLUDE 		= -I./
LINKER_PATH = -L./

PRJC = libhtml
SRC  = $(filter-out %main.c,$(shell find . -name \*.c))

all:
	$(CC) -o $(PRJC).so -fPIC -shared $(SRC) $(INCLUDE) -D__DEBUG__


test:
	make
	$(CC) -o main main.c $(INCLUDE) $(LINKER_PATH) -lhtml
	LD_LIBRARY_PATH=. ./main
