#include "bthread_private.h"

static int bthread_check_if_zombie(bthread_t bthread, void **retval);
static TQueue bthread_get_queue_at(bthread_t bthread);

#define save_context(CONTEXT) setjmp(CONTEXT)
#define restore_context(CONTEXT) longjmp(CONTEXT, 1)

static TQueue bthread_get_queue_at(bthread_t bthread)
{
    volatile __bthread_scheduler_private* scheduler = bthread_get_scheduler();
    volatile TQueue item = scheduler->queue;
    if (item == NULL) {
        return NULL;
    }
    do {
        __bthread_private* t = tqueue_get_data(item);
        if (t->tid == bthread) {
            return item;
        } else {
            item = tqueue_at_offset(item, 1);
        }
    } while (item != scheduler->queue);
    return NULL;
}

__bthread_scheduler_private* bthread_get_scheduler()
{
    static __bthread_scheduler_private* scheduler;
    if (scheduler == NULL) {
        scheduler = (__bthread_scheduler_private*)
                    malloc(sizeof(__bthread_scheduler_private));
        scheduler->queue = NULL;
        scheduler->current_item = NULL;
        scheduler->current_tid = 0;
    }
    return scheduler;
}

int bthread_create(bthread_t *bthread, const bthread_attr_t *attr,
                   void *(*start_routine) (void *), void *arg)
{
    __bthread_scheduler_private* scheduler = bthread_get_scheduler();
    __bthread_private* bthread_priv = (__bthread_private*) malloc(sizeof(__bthread_private));
    bthread_priv->body = start_routine;
    bthread_priv->arg = arg;
    if (attr != NULL) {
        bthread_priv->attr = *attr;
    }
    bthread_priv->stack = NULL;
    bthread_priv->tid = ++scheduler->current_tid;
    tqueue_enqueue(&scheduler->queue, bthread_priv);
    bthread_priv->state = __BTHREAD_READY;
    *bthread = bthread_priv->tid;
    return 0;
}

int bthread_check_if_zombie(bthread_t bthread, void **retval)
{
    __bthread_scheduler_private* scheduler = bthread_get_scheduler();
    if (!scheduler->queue) {
        return 1;
    } else {
        __bthread_private* thread = (__bthread_private*) tqueue_get_data(scheduler->current_item);
        if (thread->tid == bthread && thread->state == __BTHREAD_ZOMBIE) {
            if (retval != NULL) {
                *retval = thread->retval;
            }
            TQueue head = bthread_get_queue_at(bthread);
            free(thread->stack);
            free(thread);
            tqueue_pop(&head);
            scheduler->queue = scheduler->current_item = head;
            return 1;
        } else {
            return 0;
        }
    }
}

int bthread_join(bthread_t bthread, void **retval)
{
    __bthread_scheduler_private* scheduler = bthread_get_scheduler();
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
        asm __volatile__("movq %0, %%rsp" ::
                         "r"((intptr_t) (tp->stack + STACK_SIZE - 1)));
#else
        asm __volatile__("movl %0, %%esp" ::
                         "r"((intptr_t) (tp->stack + STACK_SIZE - 1)));
#endif
        bthread_exit(tp->body(tp->arg));
    }
    assert(0);
    return 0;
}


void bthread_yield()
{
    __bthread_scheduler_private* scheduler = bthread_get_scheduler();
    __bthread_private* thread = (__bthread_private*) tqueue_get_data(scheduler->current_item);
    if (save_context(thread->context) == 0) {
        restore_context(scheduler->context);
    }
}

void bthread_exit(void *retval)
{
    volatile __bthread_scheduler_private* scheduler = bthread_get_scheduler();
    volatile  __bthread_private* thread = (__bthread_private*) tqueue_get_data(scheduler->current_item);
    thread->retval = retval;
    thread->state = __BTHREAD_ZOMBIE;
    bthread_yield();
}

