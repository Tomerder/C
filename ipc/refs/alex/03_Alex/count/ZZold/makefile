CC=gcc
CFLAGS=-ansi -Werror -Wall -pedantic -g
LFLAGS=-g

OBJ=Count.o
MAINOBJ=CountClient.o

DIRSTAT=static
DIRDYN=dir1
DIRDYN2=dir2

LIBSTAT=libStatic.a
LIBDYN=libShared.so

#create both static & dynamic executables, define path
all: countstat countdyn $(DIRDYN2)/$(LIBDYN)
	export LD_LIBRARY_PATH=dir1
	
#static

countstat: $(MAINOBJ) $(DIRSTAT)/$(LIBSTAT)
	$(CC) $(LFLAGS) $< -lStatic -L$(DIRSTAT) -o $@

$(DIRSTAT)/$(LIBSTAT): $(DIRSTAT)/$(OBJ)
	ar -cr $@ $^

$(DIRSTAT)/$(OBJ): $(DIRSTAT)/$(OBJ:.o=.c)
	$(CC) $(CFLAGS) -c $^ -o $@

#dynamic

countdyn: $(MAINOBJ) $(DIRDYN)/$(LIBDYN)
	$(CC) $(LFLAGS) $< -lShared -L$(DIRDYN) -o $@

$(DIRDYN)/$(LIBDYN): $(DIRDYN)/$(OBJ)
	 $(CC) -shared $^ -o $@

$(DIRDYN)/$(OBJ): $(DIRDYN)/$(OBJ:.o=.c)
	$(CC) $(CFLAGS) -c $^ -o $@

#both

$(MAINOBJ): $(MAINOBJ:.o=.c)
	$(CC) $(CFLAGS) -c $^ -o $@


#2nd dynamic lib

$(DIRDYN2)/$(LIBDYN): $(DIRDYN2)/$(OBJ)
	 $(CC) -shared $^ -o $@

$(DIRDYN2)/$(OBJ): $(DIRDYN2)/$(OBJ:.o=.c)
	$(CC) $(CFLAGS) -c $^ -o $@


clean:
	rm $(DIRDYN)/$(OBJ) $(DIRDYN2)/$(OBJ) $(DIRSTAT)/$(OBJ) $(MAINOBJ)
	
cleanlib:
	rm $(DIRDYN)/$(LIBDYN) $(DIRDYN2)/$(LIBDYN) $(DIRSTAT)/$(LIBSTAT) countstat countdyn
	
cleanall:
	$(MAKE) clean
	$(MAKE) cleanlib

