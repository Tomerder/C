CC=gcc
CFLAGS+=-Wall -pedantic -ansi -g
MORE_FLAGS=-lpthread 

OBJS=prod.o cons.o main.o 
LIB=queue.a 
EXE=prodCons


$(EXE) : $(OBJS) $(LIB)
	$(CC) $(CFLAGS) $(OBJS) $(LIB) -o $(EXE) $(MORE_FLAGS) 
	make clean

clean    :
	rm *.o


