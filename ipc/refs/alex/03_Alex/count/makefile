CC=gcc
CFLAGS=-ansi -Werror -Wall -pedantic -g
LFLAGS=-g

OBJ=Count.o
MAINIMP=CountClientImp.o
MAINEXP=CountClientExp.o

DIRSTAT=static
DIRDYN=dir1
DIRDYN2=dir2
DIRBIN=bin

LIBSTAT=libStatic.a
LIBDYN=libShared.so


#create static, dynamic, explicit executables, define path
all: countstat countdyn countexp $(DIRDYN2)/$(LIBDYN)
	export LD_LIBRARY_PATH=dir1
	mv countstat countdyn countexp bin/.
	
#static

countstat: $(MAINIMP) $(DIRSTAT)/$(LIBSTAT)
	$(CC) $(LFLAGS) $< -lStatic -L$(DIRSTAT) -o $@

$(DIRSTAT)/$(LIBSTAT): $(OBJ)
	ar -cr $@ $^


#dynamic
countdyn: $(MAINIMP) $(DIRDYN)/$(LIBDYN)
	$(CC) $(LFLAGS) $< -lShared -L$(DIRDYN) -o $@


#dynamic & static
$(MAINIMP): $(MAINIMP:.o=.c)
	$(CC) $(CFLAGS) -c $^ -o $@


#explicit

countexp: $(MAINEXP) $(DIRDYN)/$(LIBDYN)
	$(CC) $(LFLAGS) $< -ldl -o $@

$(MAINEXP): $(MAINEXP:.o=.c)
	$(CC) $(CFLAGS) -c $^ -o $@


#dynamic & explicit
$(DIRDYN)/$(LIBDYN): $(OBJ)
	 $(CC) -shared $^ -o $@


#all
$(OBJ): $(OBJ:.o=.c)
	$(CC) $(CFLAGS) -c $^ -o $@


#2nd dynamic lib

$(DIRDYN2)/$(LIBDYN): $(DIRDYN2)/$(OBJ)
	 $(CC) -shared $^ -o $@

$(DIRDYN2)/$(OBJ): $(DIRDYN2)/$(OBJ:.o=.c)
	$(CC) $(CFLAGS) -c $^ -o $@



# clean

clean:
	rm $(OBJ) $(DIRDYN2)/$(OBJ) $(MAINIMP) $(MAINEXP)
	
cleanlib:
	rm $(DIRDYN)/$(LIBDYN) $(DIRDYN2)/$(LIBDYN) $(DIRSTAT)/$(LIBSTAT) countstat countdyn countexp
	
cleanall:
	$(MAKE) clean
	$(MAKE) cleanlib

