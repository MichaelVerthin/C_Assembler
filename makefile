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
		$(CC) -c $< -o $@ $(LDFLAGS)

pass.o: pass.c
		$(CC) -c $< -o $@ $(LDFLAGS)

files.o: files.c
		$(CC) -c $< -o $@ $(LDFLAGS)

macro.o: macro.c
		$(CC) -c $< -o $@ $(LDFLAGS)

encode.o: encode.c
		$(CC) -c $< -o $@ $(LDFLAGS)

misc.o: misc.c
		$(CC) -c $< -o $@ $(LDFLAGS)

hash.o: hash.c
		$(CC) -c $< -o $@ $(LDFLAGS)

math.o: math.c
		$(CC) -c $< -o $@ $(LDFLAGS)

symbols.o: symbols.c
		$(CC) -c $< -o $@ $(LDFLAGS)

opcodes.o: opcodes.c
		$(CC) -c $< -o $@ $(LDFLAGS)

line.o: line.c
		$(CC) -c $< -o $@ $(LDFLAGS)

asmbl.o: asmbl.c
		$(CC) -c $< -o $@ $(LDFLAGS)

.PHONY: clean
clean:
		rm -f *.o myprog *.ob *.ext *.ent *.gch