// MultiPlatformDesign.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Engine.h"

//------------------------------------------------------
//main
//------------------------------------------------------

int main()
{
	DoEngineInit();

	DoEngineCycle();

	TestEngine();

	return 0;
}

//------------------------------------------------------
