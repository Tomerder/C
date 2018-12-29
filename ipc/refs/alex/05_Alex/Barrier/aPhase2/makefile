CC=gcc
CFLAGS=-ansi -pedantic -Werror -Wall -g
LFLAGS=-pthread

EXEFILE=bar2
OBJS=BarrierPh2Test.o BarrierPh2.o


$(EXEFILE): $(OBJS)
	$(CC) $(LFLAGS) $^ -o $@

$(OBJS): $(OBJS:.o=.c)

include depends
depends:
	$(CC) $(CFLAGS) -MM $(OBJS:.o=.c) > depends

	
clean:
	rm *.o

