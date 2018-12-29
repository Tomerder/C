CC=gcc
CFLAGS=-ansi -pedantic -Werror -Wall -g
LFLAGS=-pthread

EXEFILE=prodcons
OBJS=ProdConsMain.o Producer.o Consumer.o GenericQueue.o LListGen.o


$(EXEFILE): $(OBJS)
	$(CC) $(LFLAGS) $^ -o $@

$(OBJS): $(OBJS:.o=.c)

include depends
depends:
	$(CC) $(CFLAGS) -MM $(OBJS:.o=.c) > depends

	
clean:
	rm *.o

