/*-----------------------------------------Tomer Dery-----------------------------------------*/
#include <stdio.h>
#include <stdlib.h>

#include "CatList.h"


CatList* CatListCreate() {  return (CatList*)ListCreate();   }       

void CatListDestroy(CatList* _list)    {  ListDestroy((List*)_list)       ;   }   

ADTERR CatListPushHead(CatList* _list , Cat* _data)  { return ListPushHead((List*)_list , (Data)_data);   }  

ADTERR CatListPushTail(CatList* _list , Cat* _data)   { return ListPushTail((List*)_list , (Data)_data);  } 

ADTERR CatListPopHead(CatList* _list , Cat** _data) { return ListPopHead((List*)_list , (Data*) _data);  }   

ADTERR CatListPopTail(CatList* _list , Cat** _data) { return ListPopHead((List*)_list , (Data*) _data);  }   

size_t CatListCountItems(CatList* _list)   { return ListCountItems((List*)_list);   }  

int CatListIsEmpty(CatList* _list)   { return ListIsEmpty((List*)_list); }

void CatListPrint(CatList* _list , PrintFunc _printFunc)   { ListPrint( (List*)_list ,  _printFunc);  } 

CatListItr CatListBegin(CatList* _list)    {  return ListBegin( (List*)_list);  }

CatListItr CatListEnd(CatList* _list)  {  return ListEnd( (List*)_list);  }  

CatListItr CatListNext(CatListItr _itr) {return ListNext( _itr);	}

CatListItr CatListPrev(CatListItr _itr)  {return ListPrev( _itr);	}

CatListItr CatListGetData(CatListItr _itr)   {return ListGetData( _itr);  }

CatListItr CatListInsert(CatListItr _insertBefore , Cat* _data) 
	{return ListInsert(_insertBefore , ((Data)_data) ); }

Cat* CatListRemove(CatListItr _toRemove)  { return (Data)ListRemove(_toRemove); }

CatListItr CatListFindFirst(CatListItr _from,CatListItr _to, MatchFunc _MatchFunc, Cat* _data) 
				{return ListFindFirst( _from,  _to,  _MatchFunc,  (Data)_data);   }

CatListItr CatListForEach(CatListItr _from, CatListItr _to, DoFunc _DoFunc, Parm _parm) 
				{ return ListForEach( _from,  _to,  _DoFunc,  _parm); }								






