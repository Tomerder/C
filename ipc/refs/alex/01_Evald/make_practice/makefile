CC=gcc
CFLAGS=-ansi -pedantic -Wall -Werror

a.out: main.o a.o b.o
	$(CC) main.o a.o b.o
main.o: main.c a.h b.h
	$(CC) $(CFLAGS) -c main.c
a.o: a.c a.h b.h
	$(CC) $(CFLAGS) -c a.c
b.o: b.c b.h
	$(CC) $(CFLAGS) -c b.c
	
clean:
	rm *.o
