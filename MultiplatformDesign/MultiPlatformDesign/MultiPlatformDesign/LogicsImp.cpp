#include "stdafx.h"
#include "LogicsImp.h"


//------------------------------------------------------
//Functions Imp.
//------------------------------------------------------

void CalcSpeed(const CalcSpeedInputStruct* _inputs, CalcSpeedOutputStruct* _outputs)
{
	_outputs->calcSpeed = (_inputs->ahrsGs + _inputs->gpsGs) / 2;
}

//------------------------------------------------------

void IsOnGround169(const IsOnGroundInput169Struct* _inputs, IsOnGroundOutputStruct* _outputs)
{
	if (_inputs->ahrsGs < 50 && _inputs->gpsGs < 50 && _inputs->altitude < 10)
	{
		_outputs->isOnGround = true;
	}
	else
	{ 
		_outputs->isOnGround = false;
	}
}

//------------------------------------------------------

void IsOnGround139(const IsOnGroundInput139Struct* _inputs, IsOnGroundOutputStruct* _outputs)
{
	if (_inputs->ahrsGs < 100 && _inputs->gpsGs < 20 && _inputs->altitude < 7 && _inputs->roll < 5)
	{
		_outputs->isOnGround = true;
	}
	else
	{
		_outputs->isOnGround = false;
	}
}

//------------------------------------------------------

