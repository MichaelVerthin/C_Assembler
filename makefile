# Makefile for C program

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
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Rule to clean object files and executable
clean:
	rm -f $(OBJ) $(TARGET)
