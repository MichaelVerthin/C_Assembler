#
# Makefile for Assembler in C
# Compiler: GNU C Compiler
#
CC = gcc
src = $(wildcard *.c)
obj = $(src:.c=.o)
LDFLAGS = -Wall -ansi -pedantic
myprog: $(obj)
		$(CC) -o $@ $^ $(LDFLAGS)
.PHONY: clean
clean:
		rm -f $(obj) myprog
