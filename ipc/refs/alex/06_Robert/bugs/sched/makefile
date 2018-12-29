CC=gcc
CFLAGS=-ansi -pedantic -Werror -Wall -g -D_POSIX_C_SOURCE=199309L

OBJS=SchedulerMain.o Sched.o TimeSched.o Task.o HeapGeneric.o VectorGeneric.o
RUN=scheduler


$(RUN): $(OBJS)
	$(CC) $^ -o $@ -lrt


include depends

depends:
	$(CC) $(CFLAGS) -c -MM $(OBJS:.o=.c) > depends


clean:
	rm $(OBJS)

