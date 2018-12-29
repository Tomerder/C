/*
	Message Queue over Shared Memory with 
	Single Writer and Multiple Readers

Project Requirements Overview

1. Implement the API defined in MsgQueue.h (attached) file.

2. Message transfer functions:
	MsgQueueSend: creates a new message, by copying the message buffer.
		If the queue is full, Send will block until a slot is available.
		Returns 0 in case of any error, number of bytes copied otherwise
	MsgQueueRecv: copies the first available message into specified buffer.
		If the queue is empty, it will block until a message is available.
		If not enough space for the message data, copies up to available space
		 (similar to MSG_NOERROR flag)
		Returns 0 in case of any error, number of bytes copied otherwise.

	Comment: Send/Recv operate in a simple producer/consumer pattern

3. Message Queue creation/destruction:
	MsgQueueCreate: 	creates and initializes a new message queue object,
		by creating shared memory and all other required shared and per-process
		private data variables
	MsgQueueConnect:	connects to already existing message queue object in
		an existing shared memory object, and initializes all per-process
		private data variables
	MsqQueueDestroy:	disconnects from message queue and destroys the 
		shared objects, if this process is a creator.

	Comment: Carefully examine the needs and define the required system and private
		objects and tools and access to them, required to be placed in both shared
		and per-process private memory.

4. Test Programs
	Write two test programs: Ping and Pong - 'make all' should build them
	Create two files MQping and MQpong for shared memory objects identification (ftok)

	Ping creates the shared memory object, hence it should be run first.
	Ping should destroy the shared memory object prior to exit.

4.1 Ping required options
	[-s sleep time between sends, default is 2]
	[-m number of messages for each Pong, default is 24]
	additional options to your discretion, like [-f queue file name] [-v]

4.2 Pong required option
	[-s sleep time between receives, default is 2]
 	additional options to your discretion, like [-f queue file name] [-v]

4.3 Modus Operandi
	Each Pong sends a handshake message to Ping and then reads messages until
	it receives a Bye-Bye message. 

	Ping waits for handshake and then starts to send [-m #] messages.
	If another handshake is received, then number of messages is increased by
	the same amount.

Design Constraints:
 
1.  All the necessary shared data should be placed in shared memory object,
	hence the need to define both shared data and per-process references to it.

2.	Maximum flexibility of the implementation 

3.	Synchronization tools are to be POSIX Thread library tools 

4.	Queue is to be based on circular Queue implementation with necessary adaptations

5.	Message data memory allocation is to be based on Continuous Allocation (MyMalloc)
	with necessary adaptations

6.	Bye-Bye message propagation to all active Pongs

Development Stages:

1.	Define the necessary memory layout and data structures
2.	Define the necessary modules (objects) and their API
3.	Define the necessary adaptations to Queue and MyMalloc
4. 	Implement the changes and unit-test them
5.	Implement creation/destruction of message queue
6.	Implement Send/Recv
7.	Implement/Adapt Pong (can be based on existing exercises)
8.	Implement/Adapt Ping (can be extended from existing exercises)
9.	Test/Debugging
10. Code Review!!

Bonus:
	Multiple Pings, with necessary additional options.

*/
