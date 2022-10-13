// gcc alternate-stack.c -o alternate-stack -lpthread
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#define STACK_SIZE 2<<15

void *mythread (void *name) {
	printf("Hello, I'm a new thread %s\n", (char*) name);
	sleep(3);
	return (void*) 42;
}

int main() {
	pthread_t thread;
	pthread_attr_t attr;
	int i;
	void *sp;

	pthread_attr_init(&attr);

	sp = malloc(STACK_SIZE);
	pthread_attr_setstack(&attr, sp, STACK_SIZE);
	pthread_create(&thread, &attr, &mythread, "Alfred");

	sleep(2);
	pthread_join(thread, (void**) &i); // Error, cannot join detached thread
	free(sp);

	printf("Return value is %d\n", i); // Return value is bogus
	return 0;
}