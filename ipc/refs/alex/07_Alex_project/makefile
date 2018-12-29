CC=gcc
CFLAGS=-ansi -pedantic -Werror -Wall -g
LFLAGS=-pthread

PRODUCER=ping
CONSUMER=pong

PRODMAIN=Ping.o
CONSMAIN=Pong.o

OBJSCOMMON=PingPong.o MsgQueue.o SharedMem.o Sync.o MPoolNew.o QueueNew.o


all: $(PRODUCER) $(CONSUMER)

$(PRODUCER): $(PRODMAIN) $(OBJSCOMMON)
	$(CC) $(LFLAGS) $^ -o $@

$(CONSUMER): $(CONSMAIN) $(OBJSCOMMON)
	$(CC) $(LFLAGS) $^ -o $@


include depends
depends:
	$(CC) $(CFLAGS) -MM $(OBJS:.o=.c) $(PRODMAIN:.o=.c) $(CONSMAIN:.o=.c) > depends

	
clean:
	rm *.o

