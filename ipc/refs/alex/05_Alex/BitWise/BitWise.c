/***************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-08-22    
    Last modified date:		2013-09-14
    Description: 	bitwise operations
***************************************************************************/

#include <stdio.h>
#include <assert.h>


/*######################################################################*/
/*					Definitions & foreward declarations					*/
/*######################################################################*/

#define BITBYTE			8
#define BITINT			32

/* flip bit of integer X according to mask M */
#define FLIP(X,M)		if( (X) & (M) ) { (X) &= ~(M); } \
						else { (X) |= (M); }

/****** SWAP ******/

/*** swap in place ***/
void 	SwapInPlaceMask		 (char *_a, char *_b);
void 	SwapInPlaceXOR		 (char *_a, char *_b);
void 	SwapInPlacePlusMinus (char *_a, char *_b);

/****** MATH ******/

/* absolute value */
int		AbsBitCompPlus1		(int _num);
int		AbsBitFlipAboveLSB	(int _num);
int		AbsBitPower2Subt	(int _num);

/* is power of 2 */
int 	IsPowerOfTwoMask	(int _num);
int 	IsPowerOfTwoLUP		(int _num);

/* add */
int		BitAdd1			(int _a, int _b);
int		BitAdd2			(int _a, int _b);

/* print binary */
void 	PrintBinary			(int _num);

/****** COUNT ******/

/* count ones */
int		BitCountMask		(int _num);
int		BitCountLUP			(int _num);

/****** BIT MANIP ******/

/* flip: all bits b/w start, end */
void	BitFlipStartEnd	(int *_num, unsigned int _start, unsigned int _end);

/* flip: single bit */
void	BitFlipSingleLUP	(int *_num, unsigned int _bit);
void	BitFlipSingleMask	(int *_num, unsigned int _bit);

/* swap 2 bits */
void	BitSwapLUP	(int *_num, unsigned int _bit1, unsigned int _bit2);
void	BitSwapMask	(int *_num, unsigned int _bit1, unsigned int _bit2);

/* circular shift left */
void	ShiftCircular	(int *_num, unsigned int _nShifts);

/* circular shift RIGHT */
void	ShiftCircularR	(int *_num, unsigned int _nShifts);


/***	LOOKUP TABLES	***/

/* # of "on" bits in each possible 1-byte number */
const int bitsTable[256] =
	{	
		0x00, 0x01, 0x01, 0x02, 0x01, 0x02, 0x02, 0x03, 	0x01, 0x02, 0x02, 0x03, 0x02, 0x03, 0x03, 0x04,		/* 0   -  15 */
		0x01, 0x02, 0x02, 0x03, 0x02, 0x03, 0x03, 0x04,		0x02, 0x03, 0x03, 0x04, 0x03, 0x04, 0x04, 0x05,		/* 16  -  31 */
		0x01, 0x02, 0x02, 0x03, 0x02, 0x03, 0x03, 0x04,		0x02, 0x03, 0x03, 0x04, 0x03, 0x04, 0x04, 0x05,		/* 32  -  47 */
		0x02, 0x03, 0x03, 0x04, 0x03, 0x04, 0x04, 0x05,		0x03, 0x04, 0x05, 0x05, 0x04, 0x05, 0x05, 0x06,		/* 48  -  63 */
		
		0x01, 0x02, 0x02, 0x03, 0x02, 0x03, 0x03, 0x04,		0x02, 0x03, 0x03, 0x04, 0x03, 0x04, 0x04, 0x05,		/* 64  -  79 */
		0x02, 0x03, 0x03, 0x04, 0x03, 0x04, 0x04, 0x05,		0x03, 0x04, 0x05, 0x05, 0x04, 0x05, 0x05, 0x06,		/* 80  -  95 */
		0x02, 0x03, 0x03, 0x04, 0x03, 0x04, 0x04, 0x05,		0x03, 0x04, 0x05, 0x05, 0x04, 0x05, 0x05, 0x06,		/* 96  - 111 */
		0x03, 0x04, 0x04, 0x05, 0x04, 0x05, 0x05, 0x06,		0x04, 0x05, 0x06, 0x06, 0x05, 0x06, 0x06, 0x07,		/* 112 - 127 */
		
		0x01, 0x02, 0x02, 0x03, 0x02, 0x03, 0x03, 0x04,		0x02, 0x03, 0x03, 0x04, 0x03, 0x04, 0x04, 0x05,		/* 128 - 143 */
		0x02, 0x03, 0x03, 0x04, 0x03, 0x04, 0x04, 0x05,		0x03, 0x04, 0x05, 0x05, 0x04, 0x05, 0x05, 0x06,		/* 144 - 159 */
		0x02, 0x03, 0x03, 0x04, 0x03, 0x04, 0x04, 0x05,		0x03, 0x04, 0x05, 0x05, 0x04, 0x05, 0x05, 0x06,		/* 160 - 175 */
		0x03, 0x04, 0x04, 0x05, 0x04, 0x05, 0x05, 0x06,		0x04, 0x05, 0x06, 0x06, 0x05, 0x06, 0x06, 0x07,		/* 176 - 191 */
		
		0x02, 0x03, 0x03, 0x04, 0x03, 0x04, 0x04, 0x05,		0x03, 0x04, 0x05, 0x05, 0x04, 0x05, 0x05, 0x06,		/* 192 - 207 */
		0x03, 0x04, 0x04, 0x05, 0x04, 0x05, 0x05, 0x06,		0x04, 0x05, 0x06, 0x06, 0x05, 0x06, 0x06, 0x07,		/* 208 - 223 */
		0x03, 0x04, 0x04, 0x05, 0x04, 0x05, 0x05, 0x06,		0x04, 0x05, 0x06, 0x06, 0x05, 0x06, 0x06, 0x07,		/* 224 - 239 */
		0x04, 0x05, 0x05, 0x06, 0x05, 0x06, 0x06, 0x07,		0x05, 0x06, 0x07, 0x07, 0x06, 0x07, 0x07, 0x08		/* 240 - 255 */
	};
		
/*----------------------------------------------------------------------*/

/* 1-on-bit integers (which are powers of 2) */
const int Pow2Table[32] =
	{
		1,     1<<1,  1<<2,  1<<3,  1<<4,  1<<5,  1<<6,  1<<7,
		1<<8,  1<<9,  1<<10, 1<<11, 1<<12, 1<<13, 1<<14, 1<<15,
		1<<16, 1<<17, 1<<18, 1<<19, 1<<20, 1<<21, 1<<22, 1<<23,
		1<<24, 1<<25, 1<<26, 1<<27, 1<<28, 1<<29, 1<<30, 1<<31
	};
/*----------------------------------------------------------------------*/


/*######################################################################*/
/*								MAIN									*/
/*######################################################################*/
int main()
{
	char A = 'a', B = 'b';
	int x = -5, y = 8, z = 0;
	int yes = 1<<18;
	int no = 50000;
	int flip = (1<<20) + 60005;
	int flipStart = 10, flipEnd = 25;
	int bit1 = 18, bit2 = 4;
	
	printf("\n------Testing Swap------\t");
	
	printf("A = %c, B = %c\n", A, B);
	SwapInPlaceMask(&A, &B);
	printf("swapped, 1st method:\t\tA = %c, B = %c\n", A, B);
	SwapInPlaceXOR(&A, &B);
	printf("swapped back, 2nd method:\tA = %c, B = %c\n", A, B);
	SwapInPlacePlusMinus(&A, &B);
	printf("swapped again, 3rd method:\tA = %c, B = %c\n", A, B);
	
	
	printf("\n------Testing Absolute value------\n");
	
	printf("\n- 1st method:\n");
	printf("abs( %d ) = %d\n", x, AbsBitCompPlus1(x));
	printf("abs( %d ) = %d\n", y, AbsBitCompPlus1(y));
	printf("abs( %d ) = %d\n", z, AbsBitCompPlus1(z));
	
	printf("\n- 2nd method:\n");
	printf("abs( %d ) = %d\n", x, AbsBitFlipAboveLSB(x));
	printf("abs( %d ) = %d\n", y, AbsBitFlipAboveLSB(y));
	printf("abs( %d ) = %d\n", z, AbsBitFlipAboveLSB(z));
	
	printf("\n- 3rd method:\n");
	printf("abs( %d ) = %d\n", x, AbsBitPower2Subt(x));
	printf("abs( %d ) = %d\n", y, AbsBitPower2Subt(y));
	printf("abs( %d ) = %d\n", z, AbsBitPower2Subt(z));
	
	printf("\n------Testing IsPowerOf2------\n");
	
	printf("\n- 1st method:\n");
	printf("%d %s a power of 2\n", yes, (IsPowerOfTwoMask(yes)) ? "_is_" : "_isn't_");
	printf("%d %s a power of 2\n", no, (IsPowerOfTwoMask(no)) ? "_is_" : "_isn't_");
	
	printf("\n- 2nd method:\n");
	printf("%d %s a power of 2\n", yes, (IsPowerOfTwoLUP(yes)) ? "_is_" : "_isn't_");
	printf("%d %s a power of 2\n", no, (IsPowerOfTwoLUP(no)) ? "_is_" : "_isn't_");
	
	printf("\n------Testing AddBinary------\n");
	
	printf("\n- 1st method:\n");
	printf("%d + %d = %d\n", no, y, BitAdd1(no, y));
	printf("%d + %d = %d\n", yes, x, BitAdd1(yes, x));
	
	printf("\n- 2nd method:\n");
	printf("%d + %d = %d\n", no, y, BitAdd2(no, y));
	printf("%d + %d = %d\n", yes, x, BitAdd2(yes, x));
	
	
	printf("\n------Testing PrintBinary------\n");
	printf("%d in binary is:  \n", yes);
	PrintBinary(yes);
	printf("%d in binary is:  \n", no);
	PrintBinary(no);
	
	
	printf("\n------Testing BitCount------\n");
	
	printf("\n- 1st method:\n");
	printf("%d has %d ones\n", yes, BitCountMask(yes));
	printf("%d has %d ones\n", no, BitCountMask(no));
	
	printf("\n- 2nd method:\n");
	printf("%d has %d ones\n", yes, BitCountLUP(yes));
	printf("%d has %d ones\n", no, BitCountLUP(no));
	
	
	printf("\n------Testing FlipStartEnd------\n");
	
	printf("before:\t\t\t");
	PrintBinary(flip);
	BitFlipStartEnd(&flip, flipStart, flipEnd);
	printf("flipped bits %d-%d:\t", flipStart, flipEnd);
	PrintBinary(flip);
	
	
	printf("\n------Testing BitFlipSingle------\n");
	
	++flipStart;
	--flipEnd;
	
	printf("\n- 1st method:\n");
	printf("before:\t\t\t");
	PrintBinary(flip);
	BitFlipSingleLUP(&flip, flipStart);
	printf("flipped bit #%d:\t", flipStart);
	PrintBinary(flip);
	
	printf("\n- 2nd method:\n");
	printf("before:\t\t\t");
	PrintBinary(flip);
	BitFlipSingleMask(&flip, flipEnd);
	printf("flipped bit #%d:\t", flipEnd);
	PrintBinary(flip);
	
	
	printf("\n------Testing BitSwap------\n");
	
	printf("\n- 1st method:\n");
	printf("before:\t\t\t");
	PrintBinary(yes);
	BitSwapLUP(&yes, bit1, bit2);
	printf("swapped bits #%d, #%d:\t", bit1, bit2);
	PrintBinary(yes);
	
	printf("\n- 2nd method:\n");
	printf("before:\t\t\t");
	PrintBinary(yes);
	BitSwapMask(&yes, bit1, bit2);
	printf("swapped bits #%d, #%d:\t", bit1, bit2);
	PrintBinary(yes);
	
	printf("\n------Testing ShiftCircular------\n");
	printf("before:\t\t\t");
	PrintBinary(flip);
	ShiftCircular(&flip, flipStart);
	printf("shifted %d places left:\t", flipStart);
	PrintBinary(flip);
	
	printf("\n");
	return 0;
}
/*----------------------------------------------------------------------*/



/*######################################################################*/
/*							Swap In Place								*/
/*######################################################################*/


/*----------------------------------------------------------------------*/
/* swap bytes in place: using a mask */
/* for each bit, if it is not the same in _a and _b - switch */
void SwapInPlaceMask(char *_a, char *_b)
{
	unsigned char mask;
	int i;
	
	for(i=0, mask=1; i<BITBYTE; ++i, mask<<=1)
	{	/* if same - continue */
		if( (*_a & mask) == (*_b & mask) ) {
			continue;
		}
		/* else: flip both */
		FLIP(*_a, mask);
		FLIP(*_b, mask);	
	} /* end for */
}
/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*/
/* swap bytes in place: xor, using the fact that (a^b)^b = a */
void SwapInPlaceXOR(char *_a, char *_b)
{
	*_a = *_a ^ *_b;
	*_b = *_a ^ *_b;	/* now b == original a */
	*_a = *_a ^ *_b;	/* now a == original b */
}
/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*/
/* swap bytes in place: using add & subtract */
void SwapInPlacePlusMinus(char *_a, char *_b)
{
	*_a = *_a + *_b;
	*_b = *_a - *_b;	/* now b == orig. a */
	*_a = *_a - *_b;	/* now a == orig. b */
}
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/


/*######################################################################*/
/*							Absolute Value								*/
/*######################################################################*/

/*----------------------------------------------------------------------*/
/* absolute value (integers): taking complement and adding 1 */
int	AbsBitCompPlus1(int _num)
{
	return ( _num >= 0 ) ? _num : ~_num + 1;
}
/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*/
/* absolute value (integers): flipping all bits left to least significant "on" bit */
int	AbsBitFlipAboveLSB(int _num)
{
	int mask, i;
	
	/* return if non-negative */
	if( _num >= 0 )
	{
		return _num;
	}
	
	/* reach least significant 1 */
	for( i=0, mask=1; (_num & mask) == 0; mask <<= 1, ++i);
	++i;

	/* invert _num */
	_num = ~_num;
	/* set LSB back to 1 */
	_num |= mask;

	/* set mask to all-1 from LSB up, all-0 below */
	mask = (~(unsigned int)0) << (i-1);

	return _num & mask;
}
/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*/
/* absolute value (integers): subtract from next power of 2 */
int	AbsBitPower2Subt(int _num)
{
	long long power2 = 1 << (BITINT);
	long long num = (long long)_num;
		
	/* return if non-negative */
	if( _num >= 0 ) {
		return _num;
	}
	
	return power2 - num;
}
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/


/*######################################################################*/
/*								Is Power of Two							*/
/*######################################################################*/

/*----------------------------------------------------------------------*/
/* check if an integer is a power of 2: using mask */
int IsPowerOfTwoMask(int _num)
{
	int power2 = 1;	/* power of 2 */
	
	if( 0 >= _num)
	{
		return 0;
	}	
	
	for( power2=1; power2 <= _num && power2 < ~((unsigned int)0); power2 <<= 1 )
	{	
		if( (_num & power2) == _num )
			return 1;
	}
	
	return 0;
}
/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*/
/* check if an integer is a power of 2: using LookUp Table */
int IsPowerOfTwoLUP(int _num)
{
	unsigned int num = _num;
	unsigned char* numChar = (unsigned char*)&num;
	int i;
	int sum = 0;

	/* sum # of on bits in each byte */
	for(i=0; i<sizeof(int); ++i)
	{
		sum += bitsTable[ *(numChar+i) ];
	}
	
	return (sum == 1);
}
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/


/*######################################################################*/
/*									Add									*/
/*######################################################################*/

/*----------------------------------------------------------------------*/
/* add (integers): bit by bit */
int BitAdd1(int _a, int _b)
{
	int mask = 1;
	int i;
	int carry = 0;
	int result = 0;
	
	for( i=0; i < BITINT; ++i, mask <<= 1)
	{
		if( (mask & _a) && (mask & _b) && !carry )
		{
			if(!carry) {
				carry = 1;
			}
			else {
				result |= mask;
			}
			continue;
		}
		
		if( (mask & _a) || (mask & _b) )
		{
			if(!carry) {
				result |= mask;
			}
			continue;
		}
		
		if(carry)
		{
			result |= mask;
			carry = 0;
		}
	}	/* end for */
	
	return result;
}
/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*/
/* add (integers): bit by bit */
int BitAdd2(int _a, int _b)
{
	int xor = _a^_b;
	int xor2 = xor;
	int and = (_a&_b)<<1;
	
	while( and != 0 )
	{
		xor2 = xor^and;
		and = (xor & and) << 1;
		xor = xor2;
	}
	
	return xor;
}
/*----------------------------------------------------------------------*/


/*######################################################################*/
/*								Print Binary							*/
/*######################################################################*/

/*----------------------------------------------------------------------*/
/* print an integer in binary */
void PrintBinary(int _num)
{
	unsigned int mask = 1 << (BITINT - 1);
	int i;
	
	for(i=0 ; mask > 0; mask >>= 1, ++i )
	{
		if( !(i%8) && i) {
			printf("|");
		}
		printf("%d", ( (_num & mask) != 0 ) );
	}
	printf("\n");
}
/*----------------------------------------------------------------------*/


/*######################################################################*/
/*							Count Ones									*/
/*######################################################################*/

/*----------------------------------------------------------------------*/
/* count number of "on" bits in an integer - using a mask */
int BitCountMask(int _num)
{
	unsigned int mask = 1 << (BITINT - 1);
	int sum = 0;
	
	for( ; mask > 0; mask >>= 1 )
	{
		sum += ((_num & mask) != 0 );
	}
	
	return sum;
}
/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*/
/* count number of "on" bits in an integer - using lookup table */
int BitCountLUP(int _num)
{
	unsigned int num = _num;
	unsigned char* numChar = (unsigned char*)&num;
	int i;
	int sum = 0;

	for(i=0; i<sizeof(int); ++i)
	{
		sum += bitsTable[ *(numChar+i) ];
	}
	
	return sum;
}
/*----------------------------------------------------------------------*/



/*######################################################################*/
/*							Flip Bits									*/
/*######################################################################*/


/*----------------------------------------------------------------------*/
/* flip all bits b/w start, end */
void	BitFlipStartEnd	(int *_num, unsigned int _start, unsigned int _end)
{
	int inverse = ~(*_num);
	unsigned int mask = ~( (unsigned int)0 );
	
	assert(_end < BITINT && _start <= _end);
	
	/* mask: contains 1's b/w start & end, 0's elsewhere */
	mask <<= ( BITINT - (_end-_start) );
	mask >>= (BITINT-_end);
	
	inverse &= mask;
	*_num 	&= ~(mask);
	
	*_num |= inverse;
}
/*----------------------------------------------------------------------*/



/*----------------------------------------------------------------------*/
/* flip single bit: lookup table */
void	BitFlipSingleLUP	(int *_num, unsigned int _bit)
{
	assert(_bit <= BITINT);
	
	FLIP(*_num, Pow2Table[_bit-1]);
}
/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*/
/* flip single bit: mask */
void	BitFlipSingleMask	(int *_num, unsigned int _bit)
{
	int mask = 1 << (_bit-1);
	
	assert(_bit < BITINT);
		
	FLIP(*_num, mask);
}
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/



/*######################################################################*/
/*							Swap Bits									*/
/*######################################################################*/

/* swap 2 bits: lookup table */
void	BitSwapLUP	(int *_num, unsigned int _bit1, unsigned int _bit2)
{
	assert(_bit1 < BITINT && _bit2 < BITINT);
	
	if( (*_num & Pow2Table[_bit1]) != (*_num & Pow2Table[_bit2]) )
	{
		FLIP(*_num, Pow2Table[_bit1]);
		FLIP(*_num, Pow2Table[_bit2]);
	}
}
/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*/
/* swap 2 bits: masks */
void	BitSwapMask	(int *_num, unsigned int _bit1, unsigned int _bit2)
{
	int mask1 = 1<<_bit1;
	int mask2 = 1<<_bit2;
	
	assert(_bit1 < BITINT && _bit2 < BITINT);
	
	if( (*_num & mask1) != (*_num & mask2) )
	{
		FLIP(*_num, mask1);
		FLIP(*_num, mask2);
	}
}
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/


/*######################################################################*/
/*						Circular Left Shift								*/
/*######################################################################*/

/*----------------------------------------------------------------------*/
/* circular shift left */
void	ShiftCircular	(int *_num, unsigned int _nShifts)
{
	int cycle = *_num >> (BITINT - _nShifts);
	
	*_num <<= _nShifts;
	*_num |= cycle;
}
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/


/*----------------------------------------------------------------------*/
/* circular shift RIGHT */
void	ShiftCircularR	(int *_num, unsigned int _nShifts)
{
	int cycle = *_num << (BITINT - _nShifts);
	
	*_num >>= _nShifts;
	*_num |= cycle;
}
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/

