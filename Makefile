CC = gcc
CFLAGS = -Wall -std=gnu2x
LIBS = -lraylib -lm -ldl -lpthread -lGL -lrt -lX11
SRC = gltfview.c libs/c-vector/vec.c
OUT = gltfview

all: $(OUT)

$(OUT): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(OUT) $(LIBS)

run: $(OUT)
	./$(OUT)

clean:
	rm -f $(OUT)
