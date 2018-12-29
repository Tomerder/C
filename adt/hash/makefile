CC=gcc
CFLAGS+=-Wall -pedantic -ansi -g


OBJS = hash.o list.o main.o  
EXE = hash


 $(EXE) : $(OBJS)
	$(CC) $(CFLAGS) *.o -o $(EXE)
	./$(EXE)
	make clean


include depends
depends:
	gcc -MM $(OBJS:.o=.c) > depends


clean    :
	rm *.o


