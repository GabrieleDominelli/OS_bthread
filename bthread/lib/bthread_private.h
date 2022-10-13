#ifndef __BTHREAD_PRIVATE_H__
#define __BTHREAD_PRIVATE_H__

#include "bthread.h"
#include "tqueue.h"

/**
Keep track of their execution state.
*/
typedef enum {
	__BTHREAD_READY = 0,	// The thread is ready to be executed
	__BTHREAD_BLOCKED,		// The thread is currently blocked due to some synchronization primitive (lock)
	__BTHREAD_SLEEPING,		// The thread is sleeping for some amount of time.
	__BTHREAD_ZOMBIE		// The thread has exited but is waiting for another thread to join
} bthread_state;

/**
Contains all the information regarding a thread.
*/
typedef struct {
	bthread_t tid;			// Thread identifier
	bthread_routine body;	// Body routine
	void* arg;				// Body routine's arguments
	bthread_state state;	// Execution state
	bthread_attr_t attr;	// Thread attributes
	char* stack;			// Stack
	jmp_buf context;		// Stack's context
	void* retval;			// Return value
} __bthread_private;

/**
This is responsible for initializing and scheduling threads.
*/
typedef struct {
	TQueue queue;				// Queue of threads
	TQueue current_item;		// Current executing thread in queue
	jmp_buf context;
	bthread_t current_tid;		// Current executing thread id
} __bthread_scheduler_private;

/**
This private function creates, maintains and returns a static pointer to the singleton instance of
__bthread_scheduler_private. Fields of this structure need to be initialized as NULL.
Other functions will call this method to obtain this pointer. This function should not be accessible
outside the library.
*/
__bthread_scheduler_private* bthread_get_scheduler();

void bthread_cleanup();

/**
Checks whether the thread referenced by the parameter bthread has reached a zombie state. If
it's not the case the function returns 0. Otherwise the following steps are performed: if retval is
not NULL the exit status of the target thread (i.e. the value that was supplied to bthread_exit)
is copied into the location pointed to by *retval; the thread's stack is freed and the thread's
private data structure is removed from the queue (Note: depending on your implementation, you
might need to pay attention to the special case where the scheduler's queue pointer itself
changes!); finally the function returns 1.
*/
static int bthread_check_if_zombie(bthread_t bthread, void **retval);

/**
Returns a "view" on the queue beginning at the node containing data for the thread identified by
bthread. If the queue is empty or doesn't contain the corresponding data this function returns
NULL.
*/
static TQueue bthread_get_queue_at(bthread_t bthread);

/**
The two macros that simplify saving and restoring an execution context.
*/
#define save_context(CONTEXT) sigsetjmp(CONTEXT, 1)
#define restore_context(CONTEXT) siglongjmp(CONTEXT, 1)

#endif