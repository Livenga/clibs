#!/usr/bin/bash

gcc -o main main.c -I../ -L../ -lstringbuilder

if [ $? -eq 0 ]
then
  LD_LIBRARY_PATH=.. ./main
fi
