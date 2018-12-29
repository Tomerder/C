/*************************************************************/
/** 	Project:	Free List               				**/
/** 	Author:		Tomer Dery								**/
/** 	Created:	27/1/2112								**/
/** 	Updated:	27/1/2112								**/
/*************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>

#include "log.h"
/*--------------------------------------------------------------------*/

enum{ FALSE , TRUE };

/*--------------------------------------------------------------------*/

typedef struct Log{
	FILE* m_file;
	unsigned m_loggerMode;
}Log;

static Log log;

/*--------------------------------------------------------------------*/

int LogCreate(const char* _fileName, unsigned _loggerMode)
{
	log.m_file = fopen(_fileName,"a");

	if(log.m_file == 0){
		return FALSE;
	}

	log.m_loggerMode = _loggerMode;

	return TRUE;
}

/*--------------------------------------------------------------------*/

void LogDestroy()
{
	fclose(log.m_file);
}

/*--------------------------------------------------------------------*/

void ToLog(unsigned _msgMode, const char* _msg, const char* _moduleName, const char* _funcName )
{	
	if(_msgMode & log.m_loggerMode == 0){
		return;
	}
  
	fprintf (log.m_file, "%s => %-15s , %-15s , %s\n", __TIME__  , _moduleName, _funcName, _msg);
}

/*
	time_t rawtime;
 	struct tm* timeinfo;
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    asctime (timeinfo)*/
/*--------------------------------------------------------------------*/











