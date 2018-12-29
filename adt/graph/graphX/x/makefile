CC = gcc
CFLAGS = -pedantic -ansi -Wall

graph: vector.o graph.o graphMain.o
	$(CC) $(CFLAGS) vector.o graph.o graphMain.o -g -o graph

graph.o: err.h graph.h graph.c vector.h vector.c
	$(CC) $(CFLAGS) -c graph.c

vector.o: err.h vector.h vector.c
	$(CC) $(CFLAGS) -c vector.c

graphMain.o: err.h graph.h graph.c
	$(CC) $(CFLAGS) -c graphMain.c
	
clean:
	rm -f *.o *~ graph
