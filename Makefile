# Compiler and Compiler Flags
CC = gcc
# CFLAGS for compilation:
# -g: Include debug symbols
# -Wall: Enable all warnings
# -Wextra: Enable extra warnings
# -std=c99: Use the C99 standard (or c11, c17 as needed)
# -I.: Add current directory to include path (for header files)
CFLAGS = -g -Wall -Wextra -std=c99 -I.
# Source files
SRCS = main.c chunk.c memory.c

# Object files (derived from source files)
OBJS = $(SRCS:.c=.o)

# Target executable name
TARGET = output.exe

# Default target: build all
all: $(TARGET)

# Rule to link object files into the target executable
$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $(TARGET) $(OBJS)

# Rule to compile .c files into .o files
# $< is the first prerequisite (the .c file)
# $@ is the target (the .o file)
# The .h files are listed as dependencies. If any of these headers change,
# the corresponding .c file will be recompiled.
# For a more robust dependency handling, especially for larger projects,
# consider using `gcc -MMD -MP` to auto-generate dependencies.

main.o: main.c common.h chunk.h memory.h
	$(CC) $(CFLAGS) -c $< -o $@

chunk.o: chunk.c chunk.h common.h
	$(CC) $(CFLAGS) -c $< -o $@

memory.o: memory.c memory.h common.h
	$(CC) $(CFLAGS) -c $< -o $@

# Phony target: clean
# Removes object files and the target executable
.PHONY: clean
clean:
	rm -f $(OBJS) $(TARGET)

# Phony target: run
# Builds and runs the executable
.PHONY: run
run: all
	./$(TARGET)
