/*******************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-09-17    
    Last modified date:		2013-09-30
    Description: 	shared memory functions
********************************************************************************/

#ifndef __SHARED_MEM_H__
#define __SHARED_MEM_H__

/* shared memory data */
typedef struct Shmem Shmem_t;

/* create Shmem_t object for shared memory identified by _name */
/* create shared mem. if doesn't exist & _create set to 1 */
/* returns NULL for error */
Shmem_t*	ShmemGet	(const char* _name, size_t _size, int _create);

/* get process-specific address of shared memory */
void*	ShmemConnect	(Shmem_t* _shmem);

/* disconnect from shared memory */
void	ShmemDisconnect	(Shmem_t* _shmem);

/* destroy shared memory (takes effect only after all processes are disconnected ) */
void	ShmemDestroy	(Shmem_t* _shmem);


#endif /* __SHARED_MEM_H__ */
