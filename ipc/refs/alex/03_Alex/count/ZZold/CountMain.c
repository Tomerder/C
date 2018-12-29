#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "Count.h"

int main(int argc, char **argv)
{
	int i;
	int n = (argc > 1) ? atoi(argv[1]) : 10;
	
	for(i=0; i<n; ++i)
	{
		count();
		printf("count out %d\n", g_index);
		sleep(1);
	}
	return 0;
}
