/**************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-08-04
    Last modified date:		2013-08-04
    Description: wrapper for generic list into cat list
***************************************************************************/

#ifndef __CATSLIST_H__
#define __CATSLIST_H__

#include "ADTDefs.h"
#include "LListGen.h"
#include "ListWrapper.h"

typedef 	struct Cats		Cats;
typedef 	List			CatsList;
typedef 	void* 			CatsData;

LIST_DECLARE(Cats)

#endif  /* __CATSLIST_H__ */

