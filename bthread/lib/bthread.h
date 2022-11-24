#ifndef __BTHREAD_H__
#define __BTHREAD_H__

// Types
typedef void *(*bthread_routine) (void *);
typedef unsigned long int bthread_t;

typedef struct {
    // FUTURE PROOF :)
} bthread_attr_t;

int bthread_create(bthread_t *bthread, const bthread_attr_t *attr,
                   void *(*start_routine) (void *), void *arg);

int bthread_join(bthread_t bthread, void **retval);

void bthread_yield();

void bthread_exit(void *retval);

#endif
