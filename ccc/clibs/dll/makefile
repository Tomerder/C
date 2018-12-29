CC=gcc
CFLAGS+=-Wall -pedantic -ansi -g

OBJS=main.c 
LIB=count.so 
EXE=lib


#3.creating executable - link
$(EXE) : $(OBJS) $(LIB)
	$(CC) $(CFLAGS) $(OBJS) $(LIB) -o $(EXE)
	./$(EXE)

#2.creating shared library
$(LIB) : $(LIB:.so=.o)
	$(CC) -shared -o $(LIB) $(LIB:.so=.o)

#1.creating object 
$(LIB).o : $(LIB).c
	$(CC) -c $(LIB).c
	
$(OBJS).o : $(OBJS).c
	$(CC) -c $(OBJS).c



clean    :
	rm *.o


