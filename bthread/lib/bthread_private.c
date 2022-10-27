#include "bthread_private.h"

__bthread_scheduler_private* bthread_get_scheduler() {
    if(scheduler == NULL) {
        scheduler = malloc(sizeof(__bthread_scheduler_private));
        scheduler->queue = NULL;
        scheduler->current_item = NULL;
        scheduler->current_tid = NULL;
    }

    return scheduler;
} 