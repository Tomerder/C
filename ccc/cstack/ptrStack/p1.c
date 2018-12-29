#include <stdio.h>
#include <stdlib.h>

/*sudo -i
  echo 0 > proc/sys/kernel/randomize_va_space
  => stack segment will allways start at same address */

int main()
{
	int i=5;  /*will be on stack and not on register because next line => &i  */
	int *pi = &i;
	
	/*int x = 777;*/    /* will fuck the whole thing - if didnt exist on p2 as well */
	
	FILE *fp;
	
	fp = fopen("myfile" , "w");
	
	fprintf(fp, "%p\n", pi);
	
	fclose(fp);
	
	printf("pi = %x\n" , (unsigned)pi);
	printf("*pi = %d\n", *pi);
	
	/*sleep(10);*/
	
	return 0;
}


/*   
	STACK MAIN END - SP	 
	fp
	(x)
	pi
	j
	STACK MAIN START - FP
	
	space for return value - Tmp
	pc+sp+regs
	push func Parameters to stack
	
	STACK FUNC END - NEW SP	
	funcLoc2
	funcLoc1
	STACK FUNC START - NEW FP
*/

