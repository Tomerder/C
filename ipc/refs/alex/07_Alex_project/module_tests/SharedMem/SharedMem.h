/*******************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-09-17    
    Last modified date:		2013-09-18
    Description: 	shared memory functions
********************************************************************************/

#ifndef __SHARED_MEM_H__
#define __SHARED_MEM_H__


/* get ID of shared memory segment, creating it if doesn't exist and flags include IPC_CREAT */
/* returns ID, or -1 for error */
int		ShmemGet		(const char* _name, int _keygen, size_t _size, int _flags);

/* connect to shared memory segment and get process specific address, or NULL for errors */
void*	ShmemConnect	(int _shmid, int _flags);

/* disconnect from shared memory, and destroy if _destroy = true */
/* ( destroy takes effect only after all processes are disconnected ) */
void	ShmemDisconnect	(void* _shmem, int _shmid, int _destroy);


#endif /* __SHARED_MEM_H__ */
