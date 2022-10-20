#ifndef __BTHREAD_H__
#define __BTHREAD_H__

#include <stdint.h>

/**
“Container” structure for the thread context.
It references the private data "_bthread_private", hiding the structure to the end-user.
*/
typedef unsigned long int bthread_t;

/**
Stores additional thread attributes.
*/
typedef struct {} bthread_attr_t;

/**
Specifies the signature of the thread body routine.
*/
typedef void *(*bthread_routine) (void *);

/**
Creates a new thread structure and puts it at the end of the queue. The thread identifier (stored
in the buffer pointed by bthread) corresponds to the position in the queue. The thread is not
started when calling this function. Attributes passed through the attr argument are ignored
(thus it is possible to pass a NULL pointer). The stack pointer for new created threads is NULL.
*/
int bthread_create(bthread_t *bthread, const bthread_attr_t *attr, void *(*start_routine) (void *), void *arg);

/**
Waits for the thread specified by bthread to terminate (i.e. __BTHREAD_ZOMBIE state), by
scheduling all the threads.
*/
int bthread_join(bthread_t bthread, void **retval);

/**
Saves the thread context and restores (long-jumps to) the scheduler context. Saving the thread
context is achieved using sigsetjmp, which can save the signal mask if the provided additional 
parameter is not zero (to restore both the context and the signal mask the corresponding call is siglongjmp).
Saving and restoring the signal mask is required for implementing preemption.
*/
void bthread_yield();

/**
Terminates the calling thread and returns a value via retval that will be available to another
thread in the same process that calls bthread_join, then yields to the scheduler. Between
bthread_exit and the corresponding bthread_join the thread stays in the
__BTHREAD_ZOMBIE state.
*/
void bthread_exit(void *retval);

#endif