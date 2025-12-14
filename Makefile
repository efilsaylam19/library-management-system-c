CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -pedantic
TARGET = library
SOURCES = main.c library.c
OBJECTS = $(SOURCES:.c=.o)

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)

%.o: %.c library.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET) library.dat

run: $(TARGET)
	./$(TARGET)

