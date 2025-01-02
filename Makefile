CC = gcc
CFLAGS = -Wall -std=gnu23
LIBS = -lraylib -lm -ldl -lpthread -lGL -lrt -lX11

# Default to debug mode
DEBUG = true

ifeq ($(DEBUG), true)
    CFLAGS += -g -DDEBUG
else
    CFLAGS += -O2
endif

SRC = gltfview.c libs/c-vector/vec.c
OUT = gltfview

all: $(OUT)

$(OUT): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(OUT) $(LIBS)

run: $(OUT)
	./$(OUT)

clean:
	rm -f $(OUT)
