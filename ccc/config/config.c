/*************************************************************/
/** 	Project:	Config file               				**/
/** 	Author:		Tomer Dery								**/
/** 	Created:	27/1/2112								**/
/** 	Updated:	27/1/2112								**/
/*************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "config.h"
#include "vec.h"
/*--------------------------------------------------------------------*/

#define MAX_LINE_LEN 256
#define MAX_CONFIG_LEN 64
#define MAX_VALUE_LEN 256

enum{ FALSE , TRUE };

/*--------------------------------------------------------------------*/

struct Config{
	Vector* m_configs;
	Vector* m_values;
}; 

/*--------------------------------------------------------------------*/

Config* ConfigCreate()
{
	Config* config = malloc(sizeof(Config));
	if(config == NULL){
		return NULL;
	}

	config->m_configs = VectorCreate(VECTOR_DEFAULT_SIZE, VECTOR_DEFAULT_SIZE); 
	if(config->m_configs == NULL){
		free(config);
		return NULL;
	}

	config->m_values = VectorCreate(VECTOR_DEFAULT_SIZE, VECTOR_DEFAULT_SIZE); 
	if(config->m_values == NULL){
		free(config->m_configs);
		free(config);
		return NULL;
	}

	return config;
}

/*--------------------------------------------------------------------*/

void ConfigDestroy(Config* _config)
{
	Data toFree = NULL;
	int i;
	
	for(i=0; i < VectorItemsNum(_config->m_configs)  ; i++){
		VectorGet(_config->m_configs , i ,&toFree);
		free(toFree);
		
		VectorGet(_config->m_values , i ,&toFree);
		free(toFree);
	}

	VectorDestroy(_config->m_configs);
	VectorDestroy(_config->m_values);

	free(_config);
}


/*--------------------------------------------------------------------*/

char* ConfigGet(Config* _config, const char* _configToGet)
{
	int i;
	char* ConfToComp = NULL;
	char* toGet = NULL;


	for(i=0; i < VectorItemsNum(_config->m_configs) ; i++){
		VectorGet(_config->m_configs , i  , (Data*)&ConfToComp);
		if( strcmp(ConfToComp, _configToGet) == 0 ){
			VectorGet(_config->m_values , i ,(Data*)&toGet);
			return toGet;
		}
	}

	return NULL;
}

/*--------------------------------------------------------------------*/

int ConfigRead(Config* _config, const char* _fileName)
{
	char line[MAX_LINE_LEN] , configNameTmp[MAX_CONFIG_LEN] , valueTmp[MAX_VALUE_LEN];
	char* configName = NULL;
	char* value = NULL;	

	FILE* file = fopen(_fileName,"r");
	if(!file){
		return FALSE;
	}	

	/*second pass for getting configs and values*/
	while(fgets(line, MAX_LINE_LEN, file) != NULL)
	{
	    /*skip remarks*/
	    if(line[0] == '#'){
			continue;
		}

	    if(sscanf(line, "%s %s", configNameTmp, valueTmp) != 2){
	    	continue;
	    }

		configName = malloc(MAX_CONFIG_LEN * sizeof(char));
		value = malloc(MAX_VALUE_LEN * sizeof(char));

		strcpy(configName, configNameTmp);
		strcpy(value, valueTmp);

		VectorInsert(_config->m_configs, configName);
		VectorInsert(_config->m_values, value);
	}
	

	fclose(file);

	return TRUE;
}
/*--------------------------------------------------------------------*/











