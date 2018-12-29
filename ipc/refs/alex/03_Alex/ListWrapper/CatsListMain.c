/**************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-08-04
    Last modified date:		2013-08-04
    Description: some testing for CatsList
***************************************************************************/

#include <stdio.h>

#include "CatsList.h"


typedef enum Color
{
	BLACK,
	WHITE,
	GINGER
} Color;

char *colors[3] = {"Black", "White", "Ginger"};

struct Cats
{
	char	m_name[16];
	Color	m_color;
};

/*----------------------------------------------------------*/

void catPrint(CatsData _cat)
{
	printf("Name: %s, Color: %s\n", (*(Cats*)_cat).m_name, colors[(*(Cats*)_cat).m_color]);
}
/*----------------------------------------------------------*/

int main()
{
	Cats myCats[3] = {{"Tula", WHITE}, {"Inana", GINGER}, {"Oin", BLACK}};
	CatsList* catList = NULL;
	
	catList = CatsListCreate();
	if(!catList) {
		return 1;
	}
	
	CatsListPushHead(catList, &myCats[0]);
	CatsListPushHead(catList, &myCats[1]);
	CatsListPushTail(catList, &myCats[2]);
	
	CatsListPrint(catList, catPrint);
	
	CatsListDestroy(catList);
	
	return 0;
}
/*----------------------------------------------------------*/
