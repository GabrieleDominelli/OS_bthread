#include "bthread_private.h"



/**
Creates a new thread structure and puts it at the end of the queue. The thread identifier (stored
in the buffer pointed by bthread) corresponds to the position in the queue. The thread is not
started when calling this function. Attributes passed through the attr argument are ignored
(thus it is possible to pass a NULL pointer). The stack pointer for new created threads is NULL.
*/
int bthread_create(bthread_t *bthread, const bthread_attr_t *attr, void *(*start_routine) (void *), void *arg){
    __bthread_scheduler_private* scheduler = bthread_get_scheduler();
    scheduler->queue = malloc(sizeof(TQueue));

    __bthread_private* thread = malloc(sizeof(__bthread_private));

    thread->tid = scheduler->current_tid;
    thread->stack = NULL;
    thread->arg = arg;
    thread->attr = *attr;
    thread->body = start_routine;

    *bthread = scheduler->current_tid++;

	tqueue_enqueue(&scheduler->queue, thread);

}

int bthread_join(bthread_t bthread, void **retval) {
	volatile __bthread_scheduler_private* scheduler = bthread_get_scheduler();
	scheduler->current_item = scheduler->queue;
	save_context(scheduler->context);

	if (bthread_check_if_zombie(bthread, retval)) return 0;
	
	__bthread_private* tp;
	do {
		scheduler->current_item = tqueue_at_offset(scheduler->current_item, 1);
		tp = (__bthread_private*) tqueue_get_data(scheduler->current_item);
	} while (tp->state != __BTHREAD_READY);
	
	// Restore context or setup stack and perform first call
	if (tp->stack) {
		restore_context(tp->context);
	} else {
		tp->stack = (char*) malloc(sizeof(char) * STACK_SIZE);
		#if __x86_64__
			asm __volatile__("movq %0, %%rsp" :: "r"((intptr_t) (tp->stack + STACK_SIZE - 1)));
		#else
			asm __volatile__("movl %0, %%esp" :: "r"((intptr_t) (tp->stack + STACK_SIZE - 1)));
		#endif

		bthread_exit(tp->body(tp->arg));
	}
}

/**
Saves the thread context and restores (long-jumps to) the scheduler context. Saving the thread
context is achieved using sigsetjmp, which can save the signal mask if the provided additional
parameter is not zero (to restore both the context and the signal mask the corresponding call is siglongjmp).
Saving and restoring the signal mask is required for implementing preemption.
*/
void bthread_yield(){
    __bthread_scheduler_private* scheduler = bthread_get_scheduler();
    __bthread_private* currThread = (__bthread_private*) tqueue_get_data(scheduler->current_item);

    if(!save_context(currThread->context)) {
        restore_context(scheduler->context);
    }
}

/**
Termina il thread chiamante e restituisce un valore tramite retval che sarà disponibile per un altro thread nello stesso processo che chiama bthread_join,
quindi cede allo scheduler. Tra bthread_exit e il corrispondente bthread_join il thread rimane nello stato __BTHREAD_ZOMBIE.
*/
void bthread_exit(void *retval) {
    __bthread_scheduler_private* scheduler = bthread_get_scheduler();
    __bthread_private* currThread = (__bthread_private*) tqueue_get_data(scheduler->current_item);
    currThread->state = __BTHREAD_ZOMBIE;

    currThread->retval = retval;
    bthread_yield();
}