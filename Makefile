# Compiler and Compiler Flags
CC = gcc
# CFLAGS for compilation:
# -g: Include debug symbols
# -Wall: Enable all warnings
# -Wextra: Enable extra warnings
# -std=c99: Use the C99 standard (or c11, c17 as needed)
# -I.: Add current directory to include path (for header files)
# -MMD -MP: Generate Make dependency files (.d)
CFLAGS = -g -Wall -Wextra -std=c99 -I. -MMD -MP
# Linker flags (if any, e.g., -lm for math library)
LDFLAGS =

# Target executable name (remains in the project root)
TARGET = output.exe

# Build directory for intermediate files
BUILD_DIR = build

# Automatically find all .c files in the current directory
SRCS = $(wildcard *.c)

# Prepend build directory to object file paths
# e.g., main.c -> build/main.o
OBJS = $(addprefix $(BUILD_DIR)/,$(SRCS:.c=.o))

# Dependency files will also be in the build directory
# e.g., build/main.o -> build/main.d
DEPS = $(OBJS:.o=.d)

# Default target: build all
all: $(TARGET)

# Rule to link object files into the target executable
# $(OBJS) already contains paths like build/main.o
$(TARGET): $(OBJS)
	@echo Linking $@
	$(CC) $(LDFLAGS) -o $@ $^
# Note: $^ expands to all prerequisites (e.g., build/chunk.o build/debug.o etc.)

# Rule to compile .c files into .o files in the BUILD_DIR
# e.g., to make build/main.o from main.c
$(BUILD_DIR)/%.o: %.c
	@echo Compiling $< to $@
	@if not exist "$(subst /,\,$(@D))" mkdir "$(subst /,\,$(@D))"
	$(CC) $(CFLAGS) -c $< -o $@

# Phony target: clean
# Removes the build directory and the target executable
.PHONY: clean
clean:
	@echo Cleaning project...
	@if exist "$(subst /,\,$(BUILD_DIR))" rd /s /q "$(subst /,\,$(BUILD_DIR))"
	@if exist "$(subst /,\,$(TARGET))" del /f /q "$(subst /,\,$(TARGET))"

# Phony target: run
# Builds and runs the executable
.PHONY: run
run: all
	.\$(TARGET)
	@echo Cleaning up build directory after run...
	@if exist "$(subst /,\,$(BUILD_DIR))" rd /s /q "$(subst /,\,$(BUILD_DIR))"

# Include the dependeny files from the build directory.
# The hyphen '-' allows Make to continue even if a .d file doesn't exist yet.
-include $(DEPS)
