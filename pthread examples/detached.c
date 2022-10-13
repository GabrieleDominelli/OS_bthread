// gcc detached.c -o detached -lpthread
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void *mythread (void *name) {
	printf("Hello, I'm a new thread %s\n", (char*) name);
	sleep(3);
	return (void*) 42;
}

int main() {
	pthread_t thread;
	pthread_attr_t attr;
	int i;

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

	pthread_create(&thread, &attr, &mythread, "Alfred");
	sleep(2);
	pthread_join(thread, (void**) &i); // Error, cannot join detached thread
	printf("Return value is %d\n", i); // Return value is bogus
	return 0;
}