CC=gcc
CFLAGS+=-Wall -pedantic -ansi -g
ARFLAGS = -rv

OBJS = main.o  
ARC_LIB = count.a 
EXE = lib


$(EXE) : $(OBJS) $(ARC_LIB)
	$(CC) $(CFLAGS) $(OBJS) $(ARC_LIB) -o $(EXE)
	./$(EXE)


$(ARC_LIB): $(ARC_LIB:.a=.o)
	ar $(ARFLAGS) $@ $^


include depends
depends:
	gcc -MM $(OBJS:.o=.c) > depends


clean    :
	rm *.o


