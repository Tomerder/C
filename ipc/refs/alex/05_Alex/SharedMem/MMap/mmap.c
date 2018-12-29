/***************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-09-08    
    Last modified date:		2013-09-08
    Description: 	shared memory - memory mapped file
****************************************************************************/

#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE 500
#endif


#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
	int file;
	int size;
	char* data;
	int i;
	
	size = getpagesize();
	
	if( -1 == (file = open("mmapfile", O_RDWR | O_CREAT)) )
	{
		perror("open");
		return errno;
	}
	
	if( (void*)-1 == (data = mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED, file, size)) )
	{
		perror("mmap");
		return errno;
	}
	
	for(i=0; i<10; ++i)
	{
		data[i] = 'A' + i;
	}
	
	
	return 0;
}


