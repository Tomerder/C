#include<stdio.h>
#include<stdlib.h>

#define SIZE_OF_BITMAP 50    /*for function create in main*/

typedef struct Bitmap{
	int numOfBits;
	int* bits;
}Bitmap;

typedef int (*bitmapFunc) (Bitmap* bitmap,int n);


int create(Bitmap* bitmap,int n);
int destroy(Bitmap* bitmap,int n);
int bitOn(Bitmap* bitmap,int n);
int bitOff(Bitmap* bitmap,int n);
int isOn(Bitmap* bitmap,int n);
int printBitmap(Bitmap* bitmap,int n);

/*enum funcName{START,create,bitOn,bitOff,isOn,destroy,END};*/

int main(int argc,char** argv)
{	
	bitmapFunc funcNameArr[5]={create,bitOn,bitOff,isOn,printBitmap,destroy};
	Bitmap bitmap;		
	int i=0;

	
	if((funcNameArr[0])(&bitmap,SIZE_OF_BITMAP) == -1){        /*create*/
		printf("failed creating\n");
	}
		

	int cont = 1;			
	unsigned int option;
	unsigned int bitNum;
	
	while (cont) {
		printf("Choose option: \n");
		printf("1: Turn Bit On  \n");
		printf("2: Turn Bit Off    \n");
		printf("3: Is Bit On  \n");
		printf("4: Print Bitmap  \n");
		printf("Any another number - stop \n");

		scanf("%d", &option);
		
		if(option>0 && option<5){
			if(option>0 && option<4){
				printf("Which bit: \n");
				scanf("%d", &bitNum);
			}
			if(funcNameArr[option](&bitmap,bitNum) == 1)    
				printf("\nBit is on\n\n");						/*for answer*/			
		}else{
			break;
		}
		
	}


	destroy(&bitmap,0);	                   /*free*/
	
	return 0;

}

/*---------------------------------------------------------------------*/

int create(Bitmap* bitmap,int n){
	int numOfInts= (n/(sizeof(int)*8)) + 1;
	bitmap->bits = (int*)malloc((numOfInts*sizeof(int)));
	if(bitmap->bits == NULL){
		return -1;
	}
	bitmap->numOfBits=n;

	*(bitmap->bits)=0;
	
	return 0;
}

/*---------------------------------------------------------------------*/

int destroy(Bitmap* bitmap,int n){
	free(bitmap->bits);
	return 0;
}

/*---------------------------------------------------------------------*/

int bitOn(Bitmap* bitmap,int n){
	int bitwise=1;	
	int leftShift=0;
	int indexInArr=0;
	
	if(n>bitmap->numOfBits){
		return -1;
	}
	
	leftShift=n%(sizeof(int)*8);
	indexInArr=n/(sizeof(int)*8);

	bitwise<<=leftShift;
	bitmap->bits[indexInArr] = bitmap->bits[indexInArr] | bitwise;

	return 0;
}


/*---------------------------------------------------------------------*/

int bitOff(Bitmap* bitmap,int n){
	int bitwise=1;	   
	int leftShift=0;
	int indexInArr=0;

	if(n>bitmap->numOfBits){
		return -1;
	}
	
	leftShift=n%(sizeof(int)*8);
	bitwise<<=leftShift;	/*    00000001 -> 00010000    */
	bitwise=~bitwise;	    /*    00010000 -> 11101111    */

	indexInArr=n/(sizeof(int)*8);

	bitmap->bits[indexInArr] = bitmap->bits[indexInArr] & bitwise;

	return 0;

}

/*---------------------------------------------------------------------*/

int isOn(Bitmap* bitmap,int n){

	int bitwise=1;	
	int rightShift=0;
	int indexInArr=0;
	int shiftResult=0;
	int answer=0;

	if(n>bitmap->numOfBits){
		return -1;
	}

	rightShift=n%(sizeof(int)*8);
	indexInArr=n/(sizeof(int)*8);

	shiftResult = bitmap->bits[indexInArr]>>rightShift;

	answer = shiftResult & 1;

	return answer;

}

/*---------------------------------------------------------------------*/


int printBitmap(Bitmap* bitmap,int n){
	int i=bitmap->numOfBits-1;
	while(i>=0){
		printf("%d",isOn(bitmap,i));
		i--;
	}
	printf("\n\n");
}









