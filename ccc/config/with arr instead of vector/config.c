/*************************************************************/
/** 	Project:	Free List               				**/
/** 	Author:		Tomer Dery								**/
/** 	Created:	27/1/2112								**/
/** 	Updated:	27/1/2112								**/
/*************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "config.h"
/*--------------------------------------------------------------------*/

#define MAX_LINE_LEN 256
#define MAX_CONFIG_LEN 64
#define MAX_VALUE_LEN 256

enum{ FALSE , TRUE };

/*--------------------------------------------------------------------*/

typedef struct Config{
	char** m_configs;
	char** m_values;
	size_t m_numOfConfigs;
}Config; 

static Config config;

/*--------------------------------------------------------------------*/

static int AllocateConfig(size_t _numOfConfigs);

/*--------------------------------------------------------------------*/

void ConfigDestroy()
{
	int i;
	
	for(i=0; i < config.m_numOfConfigs; i++){
		free(config.m_configs[i]);
		free(config.m_values[i]);
	}

	free(config.m_configs);
	free(config.m_values);
}

/*--------------------------------------------------------------------*/

static int AllocateConfig(size_t _numOfConfigs)
{
	int i;

	ConfigDestroy();

	config.m_numOfConfigs = _numOfConfigs;

	config.m_configs = malloc(sizeof(char*) * _numOfConfigs);
	if(config.m_configs == NULL){
		return FALSE;
	}	
	
	config.m_values = malloc(sizeof(char*) * _numOfConfigs);
	if(config.m_values == NULL){
		free(config.m_configs);
		return FALSE;
	}	

	for(i=0; i < config.m_numOfConfigs; i++){
		config.m_configs[i] = calloc(1, sizeof(char) * MAX_CONFIG_LEN);
		config.m_values[i] = calloc(1, sizeof(char) * MAX_VALUE_LEN);

		if(config.m_values[i] == NULL || config.m_configs[i]==NULL){
			for( ; i>=0; i--){
				free(config.m_values[i]);
				free(config.m_configs[i]);
			}
			
			free(config.m_configs);
			free(config.m_values);

			return FALSE;
		}
	}

	return TRUE;
}

/*--------------------------------------------------------------------*/

char* ConfigGet(const char* _getConfig)
{
	int i;

	for(i=0; i < config.m_numOfConfigs; i++){
		if( strcmp(config.m_configs[i], _getConfig) == 0 ){
			return config.m_values[i];
		}
	}

	return NULL;
}

/*--------------------------------------------------------------------*/
int ConfigRead(const char* _fileName)
{
	char line[MAX_LINE_LEN];
	char configName[MAX_CONFIG_LEN], value[MAX_VALUE_LEN];	
	int numOfConfigs=0;
	int configNum=0;

	FILE* file = fopen(_fileName,"r");
	if(!file){
		return FALSE;
	}	
	
	/*first pass for getting num of configs*/
	while(fgets(line, MAX_LINE_LEN, file) != NULL)
	{
	    /*skip remarks*/
	    if(line[0] == '#'){
			continue;
		}

	    if(sscanf(line, "%s %s", configName, value) != 2){
	    	continue;
	    }

	    ++numOfConfigs;
	}

	/*allocate configsArr and valuesArr*/
	if (numOfConfigs==0  ||  !AllocateConfig(numOfConfigs) ){
		return FALSE;
	}

	/*back to file begin*/
	fseek(file, 0, 0);

	/*second pass for getting configs and values*/
	while(fgets(line, MAX_LINE_LEN, file) != NULL)
	{
	    /*skip remarks*/
	    if(line[0] == '#'){
			continue;
		}

	    if(sscanf(line, "%s %s", configName, value) != 2){
	    	continue;
	    }

		strcpy(config.m_configs[configNum] , configName);
		strcpy(config.m_values[configNum] , value);

	    ++configNum;

		if(configNum == numOfConfigs){
			break;
		}
	}

	fclose(file);

	return TRUE;
}
/*--------------------------------------------------------------------*/











