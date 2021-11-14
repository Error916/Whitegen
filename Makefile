CFLAGS=-Wall -Wextra -std=c11 -pedantic -O3 -ggdb
LIBS=-lSDL2
SRC=src/main.c
CC=gcc

whitegen: $(SRC)
	$(CC) $(CFLAGS) -o whitegen $(SRC) $(LIBS)
