include ../makeflags

a.out: main.o a.o b.o
	$(CC) main.o a.o b.o

include depends
depends:
	$(CC) -MM a.c b.c main.c > depends

clean:
	rm *.o
