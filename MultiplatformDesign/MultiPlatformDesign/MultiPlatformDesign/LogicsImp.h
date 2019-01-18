#pragma once

/*** can be generated from table ***/

//------------------------------------------------------
//Functions IO 
//------------------------------------------------------

typedef struct IsOnGround169Input
{
	int gpsGs;
	int ahrsGs;
	int altitude;
}IsOnGroundInput169Struct;

typedef struct IsOnGround139Input
{
	int gpsGs;
	int ahrsGs;
	int altitude;
	int roll;
}IsOnGroundInput139Struct;

typedef struct  IsOnGroundOutput
{
	bool isOnGround;
}IsOnGroundOutputStruct;

typedef struct  CalcSpeedInput
{
	int gpsGs;
	int ahrsGs;
}CalcSpeedInputStruct;

typedef struct  CalcSpeedOutput
{
	int calcSpeed;
}CalcSpeedOutputStruct;

//------------------------------------------------------
// Implementation definitions (.h file)
//------------------------------------------------------

void IsOnGround169(const IsOnGroundInput169Struct*, IsOnGroundOutputStruct*);
void IsOnGround139(const IsOnGroundInput139Struct*, IsOnGroundOutputStruct*);
void CalcSpeed(const CalcSpeedInputStruct*, CalcSpeedOutputStruct*);

//------------------------------------------------------