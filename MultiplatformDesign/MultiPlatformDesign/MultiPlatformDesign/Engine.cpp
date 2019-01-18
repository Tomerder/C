#include <stdio.h>
#include <cstdlib>
//#include <stdlib.h>
#include <malloc.h>

#include "stdafx.h"

#include "Engine.h"
#include "ConfigData.h"

//------------------------------------------------------

#define PLATFORM169

#ifdef PLATFORM169
	#define FUNCTION_TABLE g_functionsTable169
#else
	#define FUNCTION_TABLE g_functionsTable139
#endif 

//------------------------------------------------------
//Engine globals 
//------------------------------------------------------

int* g_inputsBuffer;
int* g_outputsBuffer;

//------------------------------------------------------
//define inner implementation
//------------------------------------------------------

static void GetInputs(FunctionSignatureTableRecord _currFuncSignature);
static void SetOutputs(FunctionSignatureTableRecord _currFuncSignature);
static void HandleFunction(FunctionSignatureTableRecord _currFuncSignature);

//------------------------------------------------------
//Engine implementaion - APIs
//------------------------------------------------------

void DoEngineInit()
{
	//allocate buffers
	g_inputsBuffer = new int[MAX_NUM_OF_INPUTS];
	g_outputsBuffer = new int[MAX_NUM_OF_OUTPUTS];

}

void DoEngineCycle()
{
	int funcIndex = 0;
	int numOfFunctions= sizeof(FUNCTION_TABLE) / sizeof(FunctionTableRecordStruct);

	//for each function
	for (funcIndex = 0; funcIndex < numOfFunctions; funcIndex++)
	{
		//current function
		FunctionSignatureTableRecord currFuncSignature = FUNCTION_TABLE[funcIndex];
		
		//Handle function
		HandleFunction(currFuncSignature);
	}
}

//------------------------------------------------------

void TestEngine()
{
	int speed = GetDataItem(DI_CALC_SPEED);
	bool isOnGround = GetDataItem(DI_CALC_ON_GROUND);

#ifdef PLATFORM169
	bool expectedOnGround = true;
#else
	bool expectedOnGround = false;
#endif 

	if ( (speed == 25) && (isOnGround == expectedOnGround))
	{
		printf("PASS\n");
	}
	else
	{
		printf("FAIL\n");
	}
}

//------------------------------------------------------
//Inner implementaion 
//------------------------------------------------------

static void HandleFunction(FunctionSignatureTableRecord _currFuncSignature)
{
	//prepare inputs in buffer
	GetInputs(_currFuncSignature);

	//call function
	_currFuncSignature.function(g_inputsBuffer, g_outputsBuffer);

	//set data items according to output buffer
	SetOutputs(_currFuncSignature);
}

static void GetInputs(FunctionSignatureTableRecord _currFuncSignature)
{
	int inputIndex = 0, dataItemToGet = 0;

	while( (dataItemToGet = _currFuncSignature.inputDataItems[inputIndex]) != EMPTY_ENTRY )
	{
		g_inputsBuffer[inputIndex] = GetDataItem(dataItemToGet);
		inputIndex++;
	}
}

static void SetOutputs(FunctionSignatureTableRecord _currFuncSignature)
{
	int outputIndex = 0, dataItemToSet=0;
	while ( (dataItemToSet = _currFuncSignature.outputDataItems[outputIndex]) != EMPTY_ENTRY)
	{
		int dataToSet = g_outputsBuffer[outputIndex];
		SetDataItem(dataItemToSet, dataToSet);
		outputIndex++;
	}
}

//------------------------------------------------------