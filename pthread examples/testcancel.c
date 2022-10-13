// gcc testcancel.c -o testcancel -lpthread
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void *mythread (void *arg) {
	printf("Thread start\n");
	while (1) {
		pthread_testcancel(); /* Cancellation point */
	}
	printf("Exiting!\n"); /* This code is never executed */
	return (void*) 42; /* This code is never executed */
}

int main() {
	pthread_t thread;
	int i;

	pthread_create(&thread, NULL, &mythread, NULL);
	sleep(3);
	
	pthread_cancel(thread);
	sleep(4);
	
	pthread_join(thread, (void**) &i); /* The return value 'i' is PTHREAD_CANCELED (-1) */
	printf("Return value %d\n", i);
	return 0;
}