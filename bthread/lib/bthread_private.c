#include "bthread_private.h"

__bthread_scheduler_private *bthread_get_scheduler() {
    static __bthread_scheduler_private *scheduler = NULL;
    if (scheduler == NULL) {
        scheduler = malloc(sizeof(__bthread_scheduler_private));
        scheduler->queue = NULL;
        scheduler->current_item = NULL;
        scheduler->current_tid = 0;
    }

    return scheduler;
}

/**
Checks whether the thread referenced by the parameter bthread has reached a zombie state. If
it's not the case the function returns 0. Otherwise the following steps are performed: if retval is
not NULL the exit status of the target thread (i.e. the value that was supplied to bthread_exit)
is copied into the location pointed to by *retval; the thread's stack is freed and the thread's
private data structure is removed from the queue (Note: depending on your implementation, you
might need to pay attention to the special case where the scheduler's queue pointer itself
changes!); finally the function returns 1.
*/
static int bthread_check_if_zombie(bthread_t bthread, void **retval) {
    TQueue temp = bthread_get_queue_at(bthread);
    __bthread_private *thread = (__bthread_private*)tqueue_get_data(temp);

    if (thread->state != __BTHREAD_ZOMBIE)
        return 0;
    else if (thread->retval != NULL) {
        *retval = thread->retval;
        free(thread->stack);
        thread->stack = NULL;

        tqueue_pop(&temp);
    }
    return 1;
}

static TQueue bthread_get_queue_at(bthread_t bthread) {
    __bthread_scheduler_private *scheduler = bthread_get_scheduler();
    ulong size = tqueue_size(scheduler->queue);
    for (int i = 0; i < size; ++i) {
        TQueue temp = tqueue_at_offset(scheduler->queue, i);
        __bthread_private *thread = (__bthread_private*)tqueue_get_data(temp);
        if (thread->tid == bthread)
            return temp;
    }
    return NULL;
}










