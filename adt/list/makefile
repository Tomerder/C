CC=gcc
CFLAGS+=-Wall -pedantic -ansi -g

xGraph   : err.o list.o listMain.o  
	$(CC) $(CFLAGS) *.o -o list
	./list
	make clean

err.o : err.h  

list.o  : err.h  list.h  list.c

listMain.o   : err.h  list.h  listMain.c

clean    :
	rm *.o


