#include "bthread_private.h"

__bthread_scheduler_private *bthread_get_scheduler() {
    if (scheduler == NULL) {
        scheduler = malloc(sizeof(__bthread_scheduler_private));
        scheduler->queue = NULL;
        scheduler->current_item = NULL;
        scheduler->current_tid = NULL;
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
    __bthread_private *temp = (__bthread_private *) bthread_get_queue_at(bthread);

    if (temp != NULL && temp->state != __BTHREAD_ZOMBIE)
        return 0;
    else if (temp->tid == bthread) {
        if (temp->retval == NULL) {
            temp->retval = *retval;
            temp->stack = NULL;

            //TQueue *prev = (TQueue *) tqueue_at_offset(scheduler->queue,  i- 1);
            //TQueue *next = (TQueue *) tqueue_at_offset(scheduler->queue, i + 1);
            //((TQueueNode)(*prev))->next = next;
            //tqueue_pop((TQueue *) temp);
        }
    }

}

static TQueue bthread_get_queue_at(bthread_t bthread) {
    __bthread_scheduler_private *scheduler = bthread_get_scheduler();
    for (int i = 0; i < tqueue_size(scheduler->queue); ++i) {
        __bthread_private *temp = (__bthread_private *) tqueue_at_offset(scheduler->queue, i);
        if (temp->tid == bthread)
            return (TQueue) temp;
    }
    return NULL;
}










