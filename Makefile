# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -std=c23 -I"C:/raylib64/include"

# Linker flags
LDFLAGS = -L"C:/raylib64/lib" -lraylib -lopengl32 -lgdi32 -lwinmm

# Executable name
TARGET = gltfview.exe

# Source files
SRCS = gltfview.c libs/c-vector/vec.c

# Default to debug mode
DEBUG = true

ifeq ($(DEBUG), true)
	CFLAGS += -g -DDEBUG
else
	CFLAGS += -O2
endif

# Default target (build)
all: $(TARGET)

# Build rule
$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET) $(LDFLAGS)

# Run rule
run: $(TARGET)
	./$(TARGET)

# Clean rule
clean:
	del $(TARGET)
