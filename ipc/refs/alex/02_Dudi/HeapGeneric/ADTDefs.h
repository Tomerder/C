/**************************************************************************************
    Author: Dudi Kahana
    Creation date :      1.9.10
    Last modified date:  6.6.12
    Description : ADT General definitions.
***************************************************************************************/
#ifndef __ADTDEFS_H__
#define __ADTDEFS_H__

#define TRUE  1
#define FALSE 0

typedef enum ADTErr
{
    /* General ADT Errors */

    ERR_OK							=0,
    ERR_GENERAL,					 
    ERR_NOT_INITIALIZED,			/**	trying to write in an unallocated array				*/
    ERR_ALLOCATION_FAILED,			/**	allocated pointer is NULL 							*/
	ERR_REALLOCATION_FAILED,		/**	reallocated pointer is NULL 						*/
    ERR_UNDERFLOW,					/**	trying to write in index < 1 						*/
    ERR_OVERFLOW,					/**	trying to write in index > size 					*/
    ERR_WRONG_INDEX,				/**	trying to view (not change) index>size or index <=0	*/


} ADTErr;

void HandleErr(ADTErr errNum, char *msg);						/**  list of error messeges 						**/
void ErrorPrint(ADTErr _errNum, char* _thisFile, int _lineNum); /**  print error messeges with file & line info		**/
void ErrorCheck(ADTErr _actual, ADTErr _expected, char* _thisFile, int _lineNum);	/**  print "Failure" if received wrong error **/

#endif /*__ADTDEFS_H__*/











