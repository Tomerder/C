CC=gcc
CFLAGS+=-Wall -pedantic -ansi -g


OBJS = list.o main.o dogList.o catList.o
EXE = listDTS


 $(EXE) : $(OBJS)
	$(CC) $(CFLAGS) *.o -o $(EXE)
	./$(EXE)
	make clean


include depends
depends:
	gcc -MM $(OBJS:.o=.c) > depends


clean    :
	rm *.o


