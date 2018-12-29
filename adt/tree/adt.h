/**************************************************************************************
    Author : Tomer Dery
    Creation date :      08.01.14
    Date last modified : 09.01.14
    Description : ADT common definitions
***************************************************************************************/
#ifndef __ADT_H__
#define __ADT_H__

/*----------------------------------------------------------------------------------*/

typedef void* Data;
typedef void* Parm;

/* user supplied compere function - 
   return 0 : items are equal
   return < 0 : items 1 is smaller
   return > 0 : items 1 is bigger
 */
typedef int (*CompFunc)(Data _data1, Data _data2);

/* user supplied compere function - being used for input parameter to ForEach function*/
typedef int (*DoFunc)(Data _data, Parm _parm);

/*----------------------------------------------------------------------------------*/

#endif /* #ifndef __ADT_H__ */
