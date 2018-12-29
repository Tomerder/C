CC=gcc
CFLAGS+=-Wall -pedantic -ansi -g
MORE_FLAGS=-lpthread 

OBJS=queue.o MyMalloc.o Sync.o MsgQueue.o 
PING_OBJ=ping.o 
PONG_OBJ=pong.o 

PING_EXE=ping
PONG_EXE=pong

ping:
$(PING_EXE) : $(OBJS) $(PING_OBJ)
	$(CC) $(CFLAGS) $(OBJS) $(PING_OBJ) -o $(PING_EXE) $(MORE_FLAGS) 
	make clean

pong:
$(PONG_EXE) : $(OBJS) $(PONG_OBJ)
	$(CC) $(CFLAGS) $(OBJS) $(PONG_OBJ) -o $(PONG_EXE) $(MORE_FLAGS) 
	make clean

clean    :
	rm *.o


