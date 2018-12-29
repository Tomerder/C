cc=gcc
cFLAGS=-g -pedantic -ansi -Wall

exe: lib.a libDynamic.so 
	

lib.a: *.o
	ar rcs $@ *.o

libDynamic.so: *.o
	gcc *.o -shared -o lib.so 

