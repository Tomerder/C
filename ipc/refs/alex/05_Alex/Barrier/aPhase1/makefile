CC=gcc
CFLAGS=-ansi -pedantic -Werror -Wall -g -D_POSIX_C_SOURCE=199309L
LFLAGS=-pthread

EXEFILE=bar1
OBJS=BarrierTest.o Barrier.o


$(EXEFILE): $(OBJS)
	$(CC) $(LFLAGS) $^ -o $@

$(OBJS): $(OBJS:.o=.c)

include depends
depends:
	$(CC) $(CFLAGS) -MM $(OBJS:.o=.c) > depends

	
clean:
	rm *.o

