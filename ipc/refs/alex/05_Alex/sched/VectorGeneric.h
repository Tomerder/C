/**************************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-06-12    
    Last modified date:		2013-07-14
    Description: header file for GENERIC vector handling functions.
***************************************************************************************/

#include "ADTDefs.h"

#ifndef __VECTOR_H__
#define __VECTOR_H__

typedef void(*PrintFunc)(void*, int);

typedef struct Vector Vector;

				/**  create & initialize vector: array & metadata  **/
Vector*	VectorCreate	(int _size, int _extend);	
				/**  delete vector, free everything	**/
void 	VectorDestroy	(Vector *_vector);				

				/**  change value of existing item	**/
ADTErr 	VectorSet		(Vector *_vector, int _index, void *_item);
				/**  get value of item	**/
ADTErr	VectorGet		(Vector *_vector, int _index, void **_item);
				/**  add item to end of array **/
ADTErr	VectorAdd		(Vector *_vector, void *_item);
				/**  delete last item (& get its value) **/
ADTErr	VectorDelete	(Vector *_vector, void **_item);				

				/**  get no. of items, -1 for error	**/
ADTErr	VectorNumItems	(Vector *_vector, size_t *_size);			
				/**  print all items  **/
void 	VectorPrint		(Vector *_vector, PrintFunc _printFunc);


#endif /*__VECTOR_H__*/

