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

    /* More General Errors */    
	ERR_EMPTY,						/** trying to access an empty data structure			*/
	ERR_NOT_THERE,					/** trying to access nonexistent item					*/
	ERR_ALREADY_EXISTS,				/** attempt at duplication when not supported			*/
	ERR_INVALID_INPUT,				/** invalid input										*/

	/* Graph Errors */
	ERR_WRONG_MODE,					/** wighted/un-, directed/un- etc.						*/
	    
    /* Hash Errors */
	ERR_NOT_EFFECTIVE				/** hash table is too full to be effective				*/

} ADTErr;

#endif /*__ADTDEFS_H__*/











