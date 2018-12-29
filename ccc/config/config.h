/*****************************************************************************************************************
*    AUTHOR: Tomer Dery                                                                                 
*    DATE: 28.01.14                                                                                              
*    LAST MODIFIED: 28.01.14                                                                                  
*    DESCRIPTION: Configuration file			 
*****************************************************************************************************************/
#ifndef __CONFIG_H__
#define __CONFIG_H__

/*-----------------------------------------------------------------------------*/

typedef struct Config Config;

/*-----------------------------------------------------------------------------*/

Config* ConfigCreate();

void ConfigDestroy(Config* _config);

int ConfigRead(Config* _config, const char* _fileName);

char* ConfigGet(Config* _config, const char* _configToGet);

/*-----------------------------------------------------------------------------*/


#endif  /*__CONFIG_H__*/



