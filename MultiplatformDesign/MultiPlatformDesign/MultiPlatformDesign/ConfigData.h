#pragma once

#include "LogicsImp.h"
#include "DataItems.h"

//------------------------------------------------------
// DEfine
//------------------------------------------------------

#define MAX_NUM_OF_INPUTS 10
#define MAX_NUM_OF_OUTPUTS 10

//------------------------------------------------------
// Table (CF)
//------------------------------------------------------

typedef void(*FunctionPtr)(const void*, void*);

typedef struct FunctionSignatureTableRecord
{
	FunctionPtr function;
	DataItemsEnum inputDataItems[MAX_NUM_OF_INPUTS];
	DataItemsEnum outputDataItems[MAX_NUM_OF_OUTPUTS];
}FunctionTableRecordStruct;

//------------------------------------------------------
// 169
//------------------------------------------------------

struct FunctionSignatureTableRecord g_functionsTable169[] =
{
	{ (FunctionPtr)CalcSpeed, { DI_GS_GPS, DI_GS_AHRS }, { DI_CALC_SPEED } },
	{ (FunctionPtr)IsOnGround169, { DI_GS_GPS, DI_GS_AHRS, DI_ALT }, { DI_CALC_ON_GROUND } }
};

//------------------------------------------------------
// 139
//------------------------------------------------------

struct FunctionSignatureTableRecord g_functionsTable139[] =
{
	{ (FunctionPtr)CalcSpeed,{ DI_GS_GPS, DI_GS_AHRS },{ DI_CALC_SPEED } },
	{ (FunctionPtr)IsOnGround139,{ DI_GS_GPS, DI_GS_AHRS, DI_ALT, DI_ROLL },{ DI_CALC_ON_GROUND } }
};

//------------------------------------------------------