/**************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-08-04
    Last modified date:		2013-08-04
    Description: macros for generic list wrapper
***************************************************************************/

#define LIST_DECLARE(TYPE)		TYPE##List*	TYPE##ListCreate();												\
								void		TYPE##ListDestroy	(TYPE##List* _list);						\
								ADTErr		TYPE##ListPushHead	(TYPE##List *_list, TYPE##Data _data);		\
								ADTErr		TYPE##ListPushTail	(TYPE##List *_list, TYPE##Data _data); 		\
								TYPE##Data 	TYPE##ListPopHead	(TYPE##List *_list); 						\
								TYPE##Data 	TYPE##ListPopTail	(TYPE##List *_list); 						\
								int 		TYPE##ListIsEmpty	(TYPE##List *_list);  						\
								void		TYPE##ListPrint		(TYPE##List *_list, PrintFunc _printFunc);	\
								

/* ----------------------------------------------------------- */

#define LIST_DEFINE(TYPE)		TYPE##List*	TYPE##ListCreate()												\
								{																			\
									return (CatsList*)ListCreate();											\
								}																			\
								void		TYPE##ListDestroy	(TYPE##List* _list)							\
								{																			\
									ListDestroy((List*)_list);												\
								}																			\
								ADTErr		TYPE##ListPushHead	(TYPE##List *_list, TYPE##Data _data)		\
								{																			\
										return ListPushHead((List*)_list, (ListData)_data);					\
								}																			\
								ADTErr		TYPE##ListPushTail	(TYPE##List *_list, TYPE##Data _data) 		\
								{																			\
									return ListPushTail((List*)_list, (ListData)_data);						\
								}																			\
								TYPE##Data 	TYPE##ListPopHead	(TYPE##List *_list) 						\
								{																			\
									return (TYPE##Data*)ListPopHead((List*)_list);							\
								}																			\
								TYPE##Data 	TYPE##ListPopTail	(TYPE##List *_list) 						\
								{																			\
									return (TYPE##Data*)ListPopTail((List*)_list);							\
								}																			\
								int 		TYPE##ListIsEmpty	(TYPE##List *_list)  						\
								{																			\
									return ListIsEmpty((List*)_list);										\
								}																			\
								void		TYPE##ListPrint		(TYPE##List *_list, PrintFunc _printFunc)	\
								{																			\
									ListPrint((List*)_list, _printFunc);									\
								}																			\


/* ----------------------------------------------------------- */

