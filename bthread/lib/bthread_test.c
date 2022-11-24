#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include "bthread.h"

void* bthread1(void* arg);
void* bthread2(void* arg);
void* bthread3(void* arg);


int a, b, c;
bthread_t t1, t2, t3;


void* bthread1(void* arg)
{    

    for(a=0; a<5; a++) {
        printf("BThread1: %s %d\n", (const char*) arg, a);
		bthread_sleep(2000);
        bthread_yield();
    }
    printf("BThread1 exiting\n");
    return (void*)  13;
}

void* bthread2(void* arg)
{
    for(b=0; b<5; b++) {
        printf("BThread2: %s %d\n", (const char*) arg, b);
        bthread_yield();
    }
    printf("BThread2 exiting\n");
    return (void*)  17;
}

void* bthread3(void* arg)
{
    for(c=0; c<5; c++) {
        printf("BThread3: %s %d\n", (const char*) arg, c);
        bthread_yield();
    }
    printf("BThread3 exiting\n");
    return (void*)  42;
}

int main(int argc, char *argv[])
{
    int *r1, *r2, *r3;
    uintptr_t rv1, rv2, rv3;

    printf("Creating Alpha thread...\n");
    bthread_create(&t1, NULL, &bthread1, (void*) "Alpha");
    printf("Creating Beta thread...\n");
    bthread_create(&t2, NULL, &bthread2, (void*) "Beta");
    printf("Creating Gamma thread...\n");
    bthread_create(&t3, NULL, &bthread3, (void*) "Gamma");

    printf("Joining BThread 1\n");
    bthread_join(t1, (void **)(&r1));
    printf("Joining BThread 2\n");
    bthread_join(t2, (void **)(&r2));
    printf("Joining BThread 3\n");
    bthread_join(t3, (void **)(&r3));

    rv1 = (uintptr_t)r1;
    rv2 = (uintptr_t)r2;
    rv3 = (uintptr_t)r3;

    printf("End of run: %ld=%d,%d  %ld=%d,%d  %ld=%d,%d\n", t1, a, (int) rv1, t2, b, (int) rv2, t3, c, (int) rv3);


    return 0;
}
