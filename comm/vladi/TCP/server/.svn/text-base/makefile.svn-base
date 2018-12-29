CC =gcc
CFLAGS = -g -Wall -Werror -I../include -DDEBUG

EXE = server
OBJS = server.o

$(EXE): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@


include depends

depends:
	gcc -MM $(OBJS: .o=.c) > depends


.PHONY: clean todo

clean:
	rm *~ $(EXE) $(OBJS)

todo:
	grep -n TODO *.cpp *.h > todo.txt
	cat todo.txt
