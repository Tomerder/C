/*****************************************************************************************************************
*    AUTHOR: Tomer Dery                                                                                 
*    DATE: 18.02.14                                                                                              
*    LAST MODIFIED: 18.02.14                                                                                  
*    DESCRIPTION: Sync Objs
*****************************************************************************************************************/
#ifndef __SYNC_H__
#define __SYNC_H__

/*------------------------------------------------------------*/

typedef struct SyncObjs SyncObjs;		
typedef struct SyncObj SyncObj;

enum{ DOWN = -1 , UP = 1 };
enum{ LOCK = -1 , UNLOCK = 1 };

typedef enum{ SEM , MUTEX } SyncType;

/*------------------------------------------------------------*/

/*inits arr of _numOfObjs SyncObjs starting from mem*/
SyncObjs* SyncInit(char* _mem, int _numOfObjs);

int ObjInit(SyncObjs* _syncObjs, int _numOfObj, int _type, int _val);

int SyncObjsNeededSize(int _numOfObjs);

void SyncDestroy(SyncObjs* _syncObjs);

int SyncObjOp(SyncObjs* _syncObjs, int _numOfObj , int _op);

/*------------------------------------------------------------*/

#endif /* __SYNC_H__ */