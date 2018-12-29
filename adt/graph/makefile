CC=gcc
CFLAGS+=-Wall -pedantic -ansi -g

xGraph   : err.o  vec.o heapD.o queue.o graph.o graphMain.o  
	$(CC) $(CFLAGS) *.o -o graph
	./graph
	make clean

err.o : err.h  

vec.o : err.h  vec.c

heapD.o : err.h  heapD.c

queue.o  : err.h  queue.c

graph.o  : err.h  queue.h  vec.h  heapD.h  graph.c

graphMain.o   : err.h  vec.h  graph.h   graphMain.c

clean    :
	rm *.o


