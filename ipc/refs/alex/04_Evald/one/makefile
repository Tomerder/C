CC=gcc
CFLAGS=-ansi -Werror -Wall -pedantic -g
LFLAGS=-g

MAIN=attMain.o
OBJ=attFunc.o
EXEFILE=att
LIB=libShared.so

all: $(EXEFILE) $(LIB)

$(EXEFILE): $(MAIN)
	$(CC) $(LFLAGS) $^ -ldl -o $@

$(LIB): $(OBJ)
	$(CC) -shared $(OBJ) -o $(LIB)


include depends

depends:
	$(CC) -MM $(OBJ:.o=.c) $(MAIN:.o=.c) > depends
	
clean:
	rm $(OBJ) $(MAIN)

