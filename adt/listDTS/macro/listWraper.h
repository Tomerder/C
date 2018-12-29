#ifndef __LIST_WRAPER_H__
#define __LIST_WRAPER_H__

#include "list.h"


/*search and replace all DATA on list.h to TYPE => DATA_List.h   */
/*#define LIST_DECLARE(TYPE) (list.h : DATA=TYPE)*/
#define LIST_DECLARE(TYPE)  \
							    typedef void* TYPE##ListItr;   \
								typedef struct TYPE##List TYPE##List;\
							\
								TYPE##List* TYPE##ListCreate();       \
								void TYPE##ListDestroy(TYPE##List* _list);\
								ADTERR TYPE##ListPushHead(TYPE##List* _list , TYPE* _data);\
								ADTERR TYPE##ListPushTail(TYPE##List* _list , TYPE* _data);\
								ADTERR TYPE##ListPopHead(TYPE##List* _list , TYPE** _data);\
								ADTERR TYPE##ListPopTail(TYPE##List* _list , TYPE** _data);\
						        size_t TYPE##ListCountItems(TYPE##List* _list);\
								int TYPE##ListIsEmpty(TYPE##List* _list);\
								void TYPE##ListPrint(TYPE##List* _list , PrintFunc _printFunc);\
								TYPE##ListItr TYPE##ListBegin(TYPE##List* _list);\
								TYPE##ListItr TYPE##ListEnd(TYPE##List* _list);\
								TYPE##ListItr TYPE##ListNext(TYPE##ListItr _itr);\
								TYPE##ListItr TYPE##ListPrev(TYPE##ListItr _itr);\
								TYPE##ListItr TYPE##ListGetData(TYPE##ListItr _itr);\
								TYPE##ListItr TYPE##ListInsert(TYPE##ListItr _insertBefore , TYPE* _data);\
								TYPE* TYPE##ListRemove(TYPE##ListItr _toRemove);\
								TYPE##ListItr TYPE##ListFindFirst(TYPE##ListItr _from, TYPE##ListItr _to, MatchFunc _MatchFunc, TYPE* _data);\
								TYPE##ListItr TYPE##ListForEach(TYPE##ListItr _from, TYPE##ListItr _to, DoFunc _DoFunc, Parm _parm);



/*search and replace all DATA on list.c to TYPE => DATA_List.c */
#define LIST_DEFINE(TYPE)  \
								TYPE##List* TYPE##ListCreate() {  return (TYPE##List*)ListCreate();   }       \
								\
								void TYPE##ListDestroy(TYPE##List* _list)    {  ListDestroy((List*)_list)       ;   }   \
								\
								ADTERR TYPE##ListPushHead(TYPE##List* _list , TYPE* _data)  { return ListPushHead((List*)_list , (Data)_data);   }  \
								\
								ADTERR TYPE##ListPushTail(TYPE##List* _list , TYPE* _data)   { return ListPushTail((List*)_list , (Data)_data);  } \
								\
								ADTERR TYPE##ListPopHead(TYPE##List* _list , TYPE** _data) { return ListPopHead((List*)_list , (Data*) _data);  }   \
								\
								ADTERR TYPE##ListPopTail(TYPE##List* _list , TYPE** _data) { return ListPopHead((List*)_list , (Data*) _data);  }   \
								\
								size_t TYPE##ListCountItems(TYPE##List* _list)   { return ListCountItems((List*)_list);   }  \
								\
								int TYPE##ListIsEmpty(TYPE##List* _list)   { return ListIsEmpty((List*)_list); }\
								\
								void TYPE##ListPrint(TYPE##List* _list , PrintFunc _printFunc)   { ListPrint( (List*)_list ,  _printFunc);  } \
								\
								TYPE##ListItr TYPE##ListBegin(TYPE##List* _list)    {  return ListBegin( (List*)_list);  }\
								\
								TYPE##ListItr TYPE##ListEnd(TYPE##List* _list)  {  return ListEnd( (List*)_list);  }  \
								\
								TYPE##ListItr TYPE##ListNext(TYPE##ListItr _itr) {return ListNext( _itr);	}\
								\
								TYPE##ListItr TYPE##ListPrev(TYPE##ListItr _itr)  {return ListPrev( _itr);	}\
								\
								TYPE##ListItr TYPE##ListGetData(TYPE##ListItr _itr)   {return ListGetData( _itr);  }\
								\
								TYPE##ListItr TYPE##ListInsert(TYPE##ListItr _insertBefore , TYPE* _data) \
									{return ListInsert(_insertBefore , ((Data)_data) ); }\
								\
								TYPE* TYPE##ListRemove(TYPE##ListItr _toRemove)  { return (Data)ListRemove(_toRemove); }\
								\
								TYPE##ListItr TYPE##ListFindFirst(TYPE##ListItr _from,TYPE##ListItr _to, MatchFunc _MatchFunc, TYPE* _data) \
												{return ListFindFirst( _from,  _to,  _MatchFunc,  (Data)_data);   }\
								\
								TYPE##ListItr TYPE##ListForEach(TYPE##ListItr _from, TYPE##ListItr _to, DoFunc _DoFunc, Parm _parm) \
												{ return ListForEach( _from,  _to,  _DoFunc,  _parm); }								





#endif /* #ifndef __LIST_WRAPER_H__ */
