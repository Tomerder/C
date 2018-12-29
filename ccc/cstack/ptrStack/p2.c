#include <stdio.h>
#include <stdlib.h>

/*sudo -i
  echo 0 > proc/sys/kernel/randomize_va_space
  => stack segment will allways start at same address */

int main()
{
	volatile int j = 400;    /*volatile so j will be forced to be on stack and not on register*/
	int *pi = 0;
	
	/*int x = 777;*/   /* will fuck the whole thing - if didnt exist on p1 as well */
	
	FILE *fp;
	
	fp = fopen("myfile" , "r");
	
	fscanf(fp, "%p", &pi); /*pi will point on adress of first var on stack => j */
	
	fclose(fp);
	
	printf("&j = %x\n" , (unsigned)&j);
	printf("pi = %x\n" , (unsigned)pi);
	printf("*pi = %d\n", *pi);
	printf("j =%d\n" , j);
	
	printf("j += *pi;\n");
	j += *pi;   /* 400+400 */
	
	printf("*pi = %d\n", *pi);	
 	printf("j =%d\n" , j);
	
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

