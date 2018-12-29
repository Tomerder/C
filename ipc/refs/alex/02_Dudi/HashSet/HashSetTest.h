/**************************************************************************************
    Author :	Stav Ofer
    Creation date :      	2013-07-24
    Date last modified :	2013-07-24
    Description : Hash set - for integers only
***************************************************************************************/

#ifndef __HASHSET_TEST_H__
#define __HASHSET_TEST_H__


/* create & destroy a hash */
int 	HashTestCreateDestroy();

/* test HashInsert */
int 	HashTestInsert();

/* test HashIsFound */
int 	HashTestIsFound();

/* test HashRemove */
int 	HashTestRemove();

/* test IsEffective & NumItems */
int HashTestNumEffect();


#endif  /* __HASHSET_TEST_H__ */

