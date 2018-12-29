/*-----------------------------------------Tomer Dery-----------------------------------------*/
#include <stdio.h>
#include <stdlib.h>

#include "DogList.h"


DogList* DogListCreate() {  return (DogList*)ListCreate();   }       

void DogListDestroy(DogList* _list)    {  ListDestroy((List*)_list)       ;   }   

ADTERR DogListPushHead(DogList* _list , Dog* _data)  { return ListPushHead((List*)_list , (Data)_data);   }  

ADTERR DogListPushTail(DogList* _list , Dog* _data)   { return ListPushTail((List*)_list , (Data)_data);  } 

ADTERR DogListPopHead(DogList* _list , Dog** _data) { return ListPopHead((List*)_list , (Data*) _data);  }   

ADTERR DogListPopTail(DogList* _list , Dog** _data) { return ListPopHead((List*)_list , (Data*) _data);  }   

size_t DogListCountItems(DogList* _list)   { return ListCountItems((List*)_list);   }  

int DogListIsEmpty(DogList* _list)   { return ListIsEmpty((List*)_list); }

void DogListPrint(DogList* _list , PrintFunc _printFunc)   { ListPrint( (List*)_list ,  _printFunc);  } 

DogListItr DogListBegin(DogList* _list)    {  return ListBegin( (List*)_list);  }

DogListItr DogListEnd(DogList* _list)  {  return ListEnd( (List*)_list);  }  

DogListItr DogListNext(DogListItr _itr) {return ListNext( _itr);	}

DogListItr DogListPrev(DogListItr _itr)  {return ListPrev( _itr);	}

DogListItr DogListGetData(DogListItr _itr)   {return ListGetData( _itr);  }

DogListItr DogListInsert(DogListItr _insertBefore , Dog* _data) 
	{return ListInsert(_insertBefore , ((Data)_data) ); }

Dog* DogListRemove(DogListItr _toRemove)  { return (Data)ListRemove(_toRemove); }

DogListItr DogListFindFirst(DogListItr _from,DogListItr _to, MatchFunc _MatchFunc, Dog* _data) 
				{return ListFindFirst( _from,  _to,  _MatchFunc,  (Data)_data);   }

DogListItr DogListForEach(DogListItr _from, DogListItr _to, DoFunc _DoFunc, Parm _parm) 
				{ return ListForEach( _from,  _to,  _DoFunc,  _parm); }								






