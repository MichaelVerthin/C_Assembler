#
# Makefile for Assembler in ANSI-C
# Compiler: GNU C Compiler
#
CC = gcc
src = $(wildcard *.c)
obj = $(src:.c=.o)

LDFLAGS = -g -Wall -ansi -pedantic -fno-pie

myprog: $(obj)
		$(CC) -o $@ $^ $(LDFLAGS)

globals.o: globals.c
		$(CC) -o $@ -c $^ $(LDFLAGS)

pass.o: pass.c
		$(CC) -o $@ -c $^ $(LDFLAGS)

files.o: files.c
		$(CC) -o $@ -c $^ $(LDFLAGS)

macro.o: macro.c
		$(CC) -o $@ -c $^ $(LDFLAGS)

encode.o: encode.c
		$(CC) -o $@ -c $^ $(LDFLAGS)

misc.o: misc.c
		$(CC) -o $@ -c $^ $(LDFLAGS)

hash.o: hash.c
		$(CC) -o $@ -c $^ $(LDFLAGS)

math.o: math.c
		$(CC) -o $@ -c $^ $(LDFLAGS)

symbols.o: symbols.c
		$(CC) -o $@ -c $^ $(LDFLAGS)

opcodes.o: opcodes.c
		$(CC) -o $@ -c $^ $(LDFLAGS)

line.o: line.c
		$(CC) -o $@ -c $^ $(LDFLAGS)

asmbl.o: asmbl.c
		$(CC) -o $@ -c $^ $(LDFLAGS)

.PHONY: clean
clean:
		rm -f *.o myprog *.ob *.ext *.ent *.gch