CC = gcc
CFLAGS = -Wall -std=gnu17
LIBS = -lraylib -lm -ldl -lpthread -lGL -lrt -lX11
SRC = main.c vec.c
OUT = main

all: $(OUT)

$(OUT): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(OUT) $(LIBS)

run: $(OUT)
	./$(OUT)

clean:
	rm -f $(OUT)
