# Makefile for Library Management System

CC = gcc
CFLAGS = -Wall -Wextra -std=c99
TARGET = library
SOURCE = library.c

# Default target
all: $(TARGET)

# Build the executable
$(TARGET): $(SOURCE) library.h
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE)

# Clean build artifacts
clean:
	rm -f $(TARGET) $(TARGET).exe

# Run the program
run: $(TARGET)
	./$(TARGET)

# Windows-specific targets
windows: $(SOURCE) library.h
	gcc $(CFLAGS) -o $(TARGET).exe $(SOURCE)

.PHONY: all clean run windows

