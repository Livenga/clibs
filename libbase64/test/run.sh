#!/usr/bin/bash


gcc -o test main.c -I.. -L.. -lbase64 && LD_LIBRARY_PATH=.. ./test
