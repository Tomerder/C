/**************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-08-04
    Last modified date:		2013-08-04
    Description: wrapper for generic list into cat list
***************************************************************************/

#include "LListGen.h"
#include "CatsList.h"


LIST_DEFINE(Cats)


/******* temp ********

CatsList*	CatsListCreate()
{
	return (CatsList*)ListCreate();
}

void 		CatsListDestroy		(CatsList* _list)
{
	ListDestroy((List*)_list);
}

ADTErr 		CatsListPushHead	(CatsList *_list, CatsData _data)
{
	return ListPushHead((List*)_list, (ListData)_data);
}

ADTErr 		CatsListPushTail		(CatsList *_list, CatsData _data)
{
	return ListPushTail((List*)_list, (ListData)_data);
}

CatsData 	CatsListPopHead		(CatsList *_list)
{
	return (CatsData*)ListPopHead((List*)_list);
}

CatsData 	CatsListPopTail		(CatsList *_list)
{
	return (CatsData*)ListPopTail((List*)_list);
}

int 		CatsListIsEmpty		(CatsList *_list)
{
	return ListIsEmpty((List*)_list);
}

void 		CatsListPrint		(CatsList *_list, PrintFunc _printFunc)
{
	ListPrint((List*)_list, _printFunc);
}
*/
