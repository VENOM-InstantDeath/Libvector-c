CC=gcc
CFLAGS=--address=sanitize
LIBS=-lasan -lm

x: test.c vector.c
	$(CC) test.c vector.c -o x -g $(LIBS)
