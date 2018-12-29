CC=gcc

SPECIAL_FLAGS=-D_XOPEN_SOURCE=600 -pthread

CFLAGS=-ansi -pedantic -Wall -Werror -g


barrier: barrier.c test.c
	$(CC) $(CFLAGS) barrier.c test.c $(SPECIAL_FLAGS) -o barrier.out 

	
clean:
	rm *.out
