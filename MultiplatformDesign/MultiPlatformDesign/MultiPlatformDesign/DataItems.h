#pragma once

typedef enum DataItems
{
	EMPTY_ENTRY,
	DI_GS_AHRS,
	DI_GS_GPS,
	DI_ALT,
	DI_ROLL,
	DI_CALC_SPEED,
	DI_CALC_ON_GROUND,
	NUM_OF_DATA_ITEMS
}DataItemsEnum;

static int dataItemsValues[NUM_OF_DATA_ITEMS] = { 0, 20, 30, 5 , 4 , 0, 0 };

int GetDataItem(int _index) { return dataItemsValues[_index]; }
void SetDataItem(int _index, int _value) { dataItemsValues[_index] = _value; }
