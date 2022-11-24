#ifndef __BTHREADPRIVATE_H__
#define __BTHREADPRIVATE_H__

#include <assert.h>
#include <stdlib.h>
#include <signal.h>
#include <setjmp.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/time.h>
#include "bthread.h"
#include "tqueue.h"

#define STACK_SIZE 64000000

typedef enum { __BTHREAD_READY = 0, __BTHREAD_BLOCKED, __BTHREAD_SLEEPING, __BTHREAD_ZOMBIE } bthread_state;

typedef struct {
    bthread_t tid;
    bthread_routine body;
    void* arg;
    bthread_state state;
    bthread_attr_t attr;
    char* stack;
    jmp_buf context;
    void* retval;
	double wake_up_time;
} __bthread_private;

typedef struct {
    TQueue queue;
    TQueue current_item;
    jmp_buf context;
    bthread_t current_tid;
} __bthread_scheduler_private;

__bthread_scheduler_private* bthread_get_scheduler();


#endif
