CC=gcc
CFLAGS+=-Wall -pedantic -ansi -g
MORE_FLAGS=-lpthread 

OBJS=barrier.o main.o 
EXE=bar


$(EXE) : $(OBJS) $(LIB)
	$(CC) $(CFLAGS) $(OBJS) -o $(EXE) $(MORE_FLAGS) 
	make clean

clean    :
	rm *.o


