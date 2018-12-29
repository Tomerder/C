/********************************************************
Author:	Alex Katz
Date: 	04/11/2012
Last modified date: 
Description: .
*********************************************************/
#ifndef _DATA_PARAM_COMP_DO
#define _DATA_PARAM_COMP_DO
typedef void* Data;
typedef const void*	Param;
typedef int		(*DataComp) (Data, Data);  /* 0 if equal */
typedef int		(*DoFunc)   (Data, Param); /* 0=success */
/*typedef void	(*PrintFunc)(Data);*/
#endif /* _DATA_PARAM_COMP_DO */
