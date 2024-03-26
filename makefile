# Makefile for your assembler (assuming C source files)

# Compiler
CC = gcc

# Source files
SRC = $(wildcard *.c)

# Object files (use .o extension for C)
OBJ = $(SRC:.c=.o)

# Executable name
TARGET = myprog

# Compiler flags
CFLAGS = -g -Wall -ansi -pedantic

# Rule to build the executable
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# Phony target for clean
.PHONY: clean

# Clean rule
clean:
	rm -f $(OBJ) $(TARGET)
