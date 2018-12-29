CC=gcc
CFLAGS=-ansi -pedantic -Werror -Wall -g -D_XOPEN_SOURCE=500
LFLAGS=

OBJS_PRINT=SemPrint.o SemTest.o
OBJS_CONTROL=SemControl.o SemTest.o

RUNPRINT=semprint
RUNCONTROL=semcontrol


all: $(RUNPRINT) $(RUNCONTROL)
		

$(RUNPRINT): $(OBJS_PRINT)
	$(CC) $(LFLAGS) $^ -o $@

$(RUNCONTROL): $(OBJS_CONTROL)
	$(CC) $(LFLAGS) $^ -o $@


SemPrint.o: SemPrint.c SemTest.h
	$(CC) $(CFLAGS) -c $< -o $@

SemControl.o: SemControl.c SemTest.h
	$(CC) $(CFLAGS) -c $< -o $@

SemTest.o: SemTest.c SemTest.h
	$(CC) $(CFLAGS) -c $< -o $@
	
	
clean:
	rm *.o

