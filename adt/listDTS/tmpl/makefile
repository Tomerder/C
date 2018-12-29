CC=gcc
CFLAGS+=-Wall -pedantic -ansi -g


OBJS = list.o main.o DogList.o CatList.o
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


