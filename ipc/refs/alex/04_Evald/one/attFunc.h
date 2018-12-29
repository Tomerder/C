/**************************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-08-07    
    Last modified date:		2013-08-07
    Description: header file for 
***************************************************************************************/

#ifndef __ATT__
#define __ATT__


void __attribute__ ((constructor)) LoadFunc(void);
void __attribute__ ((destructor)) UnloadFunc(void);

void foo(int i);


#endif /* __ATT__ */
