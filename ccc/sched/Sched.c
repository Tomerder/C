/*************************************************************
 	Project:	Scheduler               			
 	Author:		Tomer Dery							
 	Created:	27/1/2112								
 	Updated:	27/1/2112							
*************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

#include "Sched.h"
#include "Task.h"
#include "heap.h"
#include "config.h"
#include "log.h"

/*--------------------------------------------------------------------*/

enum {FALSE, TRUE};

#define CONFIG_FILE "config.txt"
#define LOG_FILE "log.txt"

#define WRT_LOG(MODE, STR) ToLog(MODE, STR, __FILE__, __func__ );

/*--------------------------------------------------------------------*/

struct Sched{
	Vector* m_vec;
	Heap* m_tasks;
}; 

/*--------------------------------------------------------------------*/

static void SetLogModeFromConfFile();

/*--------------------------------------------------------------------*/

Sched* SchedCreate()
{
	WeakOrderFunc CompTasksFunc = CompTasks;
	
	Sched* sched = malloc (sizeof(Sched));
	if(sched == NULL){
		return NULL;
	}
	
	
	sched->m_vec = VectorCreate(VECTOR_DEFAULT_SIZE, VECTOR_DEFAULT_SIZE);
	if(sched->m_vec == NULL){
		free(sched);
		return NULL;
	}
	
	sched->m_tasks = HeapBuild(sched->m_vec, CompTasksFunc);
	if(sched->m_tasks == NULL){
		VectorDestroy(sched->m_vec);	
		free(sched);
		return NULL;
	}
	
	/*set logMode from config file*/
	SetLogModeFromConfFile();

	return sched;
}

/*--------------------------------------------------------------------*/

static int IsNumber(char* configValue)
{
	int i;
	for(i=0; i < strlen(configValue); i++){
		if(!isdigit(configValue[i])){
			return FALSE;
		}
	}
	return TRUE;
}

static void SetLogModeFromConfFile()
{
	int logMode = LOG_NONE;	
	char* configValue = NULL;

	/*get configs*/
	Config* config = ConfigCreate();
	
	ConfigRead(config, CONFIG_FILE);	

	configValue = ConfigGet(config, "LogMode");
	if(configValue == NULL ){
		logMode = LOG_NONE;
	}else{
		if(IsNumber(configValue)){
			logMode = atoi(configValue);
		}else{
			logMode = LOG_NONE;
		}
	}
	
	ConfigDestroy(config);

	/*open log*/
	LogCreate(LOG_FILE, logMode);

}

/*--------------------------------------------------------------------*/

void SchedDestroy(Sched* _sched)
{
	HeapDestroy(_sched->m_tasks);
	VectorDestroy(_sched->m_vec);
	
	free(_sched);

	LogDestroy();
}

/*--------------------------------------------------------------------*/

ADTERR SchedAdd(Sched* _sched, Func _func, Params _param, float _interval)
{
	Task* task = TaskCreate(_func, _param, _interval);
	if(task == NULL){
		WRT_LOG(LOG_ERR, "ALLOC_FAILED");
		return ALLOC_FAILED;
	}
	
	return HeapInsert(_sched->m_tasks, task);
}

/*--------------------------------------------------------------------*/

void SchedRun(Sched* _sched)
{
	char logMsg[LOG_MSG_LEN];
	Task* curTask = NULL;
	FuncStatus status;	

	while( ! HeapIsEmpty (_sched->m_tasks) ){
		curTask = HeapExtract(_sched->m_tasks);

		if( (status = TaskRun(curTask)) == FUNC_CONTINUE ){
			/*update task nextTimeToRun*/
			TaskSetNextTimeToRun(curTask);
			
			/*insert back to heap*/
			HeapInsert(_sched->m_tasks, curTask);
		}else{
			if(status == FUNC_ERROR){
				WRT_LOG(LOG_ERR, "Func Error");
			}

			/*task is finished and no longer neccesery*/
			TaskDestroy(curTask);
		}

		sprintf(logMsg, "Run Status: %d", status);
		WRT_LOG(LOG_DEB, logMsg);
	}

}

/*--------------------------------------------------------------------*/
