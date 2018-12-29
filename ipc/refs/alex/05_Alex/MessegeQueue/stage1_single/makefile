CC=gcc
CFLAGS=-ansi -pedantic -Werror -Wall -g -D_POSIX_C_SOURCE=199309L
LFLAGS=

OBJS_PING=Ping.o PingPong.o
OBJS_PONG=Pong.o PingPong.o

RUNPING=ping
RUNPONG=pong


all: $(RUNPING) $(RUNPONG)
		

$(RUNPING): $(OBJS_PING)
	$(CC) $(LFLAGS) $^ -o $@

$(RUNPONG): $(OBJS_PONG)
	$(CC) $(LFLAGS) $^ -o $@


Ping.o: Ping.c PingPong.h
	$(CC) $(CFLAGS) -c $< -o $@

Pong.o: Pong.c PingPong.h
	$(CC) $(CFLAGS) -c $< -o $@

PingPong.o: PingPong.c PingPong.h
	$(CC) $(CFLAGS) -c $< -o $@
	
	
clean:
	rm *.o

