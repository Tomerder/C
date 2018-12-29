CC=gcc
CFLAGS+=-Wall -pedantic -ansi -g -D_POSIX_C_SOURCE=199309L

OBJS = Sched.o Task.o Time.o vec.o heap.o log.o config.o main.o  
EXE = sched

$(EXE) : $(OBJS) 
	$(CC) $(CFLAGS) $(OBJS) -lrt -O2 -o $(EXE)
	./$(EXE)

include depends
depends:
	gcc -MM $(OBJS:.o=.c) > depends

clean    :
	rm *.o


