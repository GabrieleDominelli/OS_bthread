#include "bthread_private.h"

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
