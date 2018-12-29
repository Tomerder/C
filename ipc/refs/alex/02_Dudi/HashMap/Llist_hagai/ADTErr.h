/**************************************************************************************
    Author: 
    Creation date :      
    Last modified date:  
    Description : ADT General definitions.
***************************************************************************************/
#ifndef __ADTDEFS_H__
#define __ADTDEFS_H__


#define TRUE  1
#define FALSE 0



typedef enum
{
    /* General ADT Errors */
    ERR_OK                            =0,
    ERR_GENERAL,
    ERR_NOT_INITIALIZED,
    ERR_ALLOCATION_FAILED,
    ERR_REALLOCATION_FAILED,
    ERR_UNDERFLOW,
    ERR_OVERFLOW,
    ERR_WRONG_INDEX

    /* Vector Errors */

    /* Stack  Errors */

    /* LinkedList  Errors */

    /* Bit Vector*/


} ADTErr;

/*
void HandleErr(ADTErr errNum, char *msg)
{

	switch(errNum){

		case (0):{
			msg= ("ok");
			break;
		}
		case (1):{
			msg= ("unknown error");
			break;
		}
		case (2):{
			msg= ("Location is empty, no value has been stored");
			break;
		}
		case (3):{
			msg= ("could not create vector, allocation failed");
			break;
		}
		case (4):{
			msg= ("could not extand vector, reallocation failed");
			break;
		}
		case (5):{
			msg= ("desired location is out side vector bounderies");
			break;
		}
		case (6):{
			msg= ("desired location is out side vector bounderies");
			break;
		}
		case (7):{
			msg= ("Location does not exist");
			break;
		}


		default :{
			break;
		}

	}

	printf ("\n%s",msg);
}*/	/*HandleErr*/


#endif /*__ADTDEFS_H__*/


