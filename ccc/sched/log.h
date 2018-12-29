/*****************************************************************************************************************
*    AUTHOR: Tomer Dery                                                                                 
*    DATE: 28.01.14                                                                                              
*    LAST MODIFIED: 28.01.14                                                                                  
*    DESCRIPTION: Logger			 
*****************************************************************************************************************/
#ifndef __LOG_H__
#define __LOG_H__

/*-----------------------------------------------------------------------------*/

#define LOG_MSG_LEN 32

/*#define WRT_LOG(MODE, STR) ToLog(MODE, STR, __FILE__, __func__ );*/

enum e_LogLevel {		/* logging levels 				*/
    LOG_NONE = 0x00,	/* silent mode					*/
    LOG_ERR  = 0x01,    /* error     messages			*/
    LOG_WAR  = 0x02,    /* warning   messages			*/
    LOG_DEB  = 0x04,    /* debugging messages			*/
    LOG_RMG  = 0x08     /* resource manager messages	*/
};
/*-----------------------------------------------------------------------------*/

int LogCreate(const char* _fileName, unsigned _loggerMode);

void LogDestroy();

void ToLog(unsigned _msgMode, const char* _msg, const char* _moduleName, const char* _funcName );

/*-----------------------------------------------------------------------------*/


#endif  /*__LOG_H__*/



