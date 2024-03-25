# #
# # Makefile for Assembler in C
# # Compiler: GNU C Compiler
# #
# CC = gcc
# src = $(wildcard *.c)
# obj = $(src:.c=.o)
# LDFLAGS = -Wall -ansi -pedantic
# myprog: $(obj)
# 		$(CC) -o $@ $^ $(LDFLAGS)
# .PHONY: clean
# clean:
# 		rm -f $(obj) myprog
# Makefile for C program

# Compiler: GNU C Compiler
CC = gcc

# Source files
SRC = $(wildcard *.c)

# Object files (use .o extension)
OBJ = $(SRC:.c=.o)

# Compiler flags (enable warnings)
CFLAGS = -Wall

# Executable name
TARGET = myprog

# Rule to build the program
$(TARGET): $(OBJ)
    $(CC) $(CFLAGS) -o $@ $^

# Rule to clean object files and executable
clean:
    rm -f $(OBJ) $(TARGET)
