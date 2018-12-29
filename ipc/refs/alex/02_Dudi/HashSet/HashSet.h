/**************************************************************************************
    Author :	Stav Ofer
    Creation date :      	2013-07-24
    Date last modified :	2013-07-24
    Description : Hash set - for integers only
***************************************************************************************/

#ifndef __HASHSET_H__
#define __HASHSET_H__

typedef struct Hash Hash;

/* create hash, return NULL for failure */
Hash*	HashCreate		(size_t _size);

/* destroy hash */
void	HashDestroy		(Hash* _hash);

/* return the index for found, 0 for not found */
/* if the index is 0, returns array size */
int		HashIsFound		(Hash* _hash, int _key);

/* insert in hash, return success/error messege */
ADTErr	HashInsert		(Hash* _hash, int _key);

/* remove from hash */
ADTErr	HashRemove		(Hash* _hash, int _key);

/* return # of items */
int		HashNumItems	(Hash* _hash);

/* is hash too full to be effective? 1 for yes, 0 for no */
int		HashIsEffective	(Hash* _hash);

/* print - for testing */
void	HashPrint		(Hash* _hash);

#endif  /* __HASHSET_H__ */

