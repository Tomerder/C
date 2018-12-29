/*-----------------------------------------Tomer Dery-----------------------------------------*/
#include <stdio.h>
#include <stdlib.h>

#include "XXXList.h"


XXXList* XXXListCreate() {  return (XXXList*)ListCreate();   }       

void XXXListDestroy(XXXList* _list)    {  ListDestroy((List*)_list)       ;   }   

ADTERR XXXListPushHead(XXXList* _list , XXX* _data)  { return ListPushHead((List*)_list , (Data)_data);   }  

ADTERR XXXListPushTail(XXXList* _list , XXX* _data)   { return ListPushTail((List*)_list , (Data)_data);  } 

ADTERR XXXListPopHead(XXXList* _list , XXX** _data) { return ListPopHead((List*)_list , (Data*) _data);  }   

ADTERR XXXListPopTail(XXXList* _list , XXX** _data) { return ListPopHead((List*)_list , (Data*) _data);  }   

size_t XXXListCountItems(XXXList* _list)   { return ListCountItems((List*)_list);   }  

int XXXListIsEmpty(XXXList* _list)   { return ListIsEmpty((List*)_list); }

void XXXListPrint(XXXList* _list , PrintFunc _printFunc)   { ListPrint( (List*)_list ,  _printFunc);  } 

XXXListItr XXXListBegin(XXXList* _list)    {  return ListBegin( (List*)_list);  }

XXXListItr XXXListEnd(XXXList* _list)  {  return ListEnd( (List*)_list);  }  

XXXListItr XXXListNext(XXXListItr _itr) {return ListNext( _itr);	}

XXXListItr XXXListPrev(XXXListItr _itr)  {return ListPrev( _itr);	}

XXXListItr XXXListGetData(XXXListItr _itr)   {return ListGetData( _itr);  }

XXXListItr XXXListInsert(XXXListItr _insertBefore , XXX* _data) 
	{return ListInsert(_insertBefore , ((Data)_data) ); }

XXX* XXXListRemove(XXXListItr _toRemove)  { return (Data)ListRemove(_toRemove); }

XXXListItr XXXListFindFirst(XXXListItr _from,XXXListItr _to, MatchFunc _MatchFunc, XXX* _data) 
				{return ListFindFirst( _from,  _to,  _MatchFunc,  (Data)_data);   }

XXXListItr XXXListForEach(XXXListItr _from, XXXListItr _to, DoFunc _DoFunc, Parm _parm) 
				{ return ListForEach( _from,  _to,  _DoFunc,  _parm); }								






