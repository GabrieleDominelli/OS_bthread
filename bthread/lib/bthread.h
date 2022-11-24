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

double get_current_time_millis();
/**
 * When calling bthread_sleep the state of the thread is set to __BTHREAD_SLEEPING and then the thread
 * must yield to the scheduler. The wake up time is computed from the current time, obtained using
 * get_current_time_millis().
 * 
 * Each time the thread is considered for execution the scheduler must first check if it's sleeping: in that
 * situation the scheduler compares the current time with the wake up time and if necessary changes the state
 * to __BTHREAD_READY to enable execution.
*/
void bthread_sleep(double ms);

#endif
