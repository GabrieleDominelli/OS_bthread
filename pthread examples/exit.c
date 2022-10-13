// gcc exit.c -o exit -lpthread
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void *mythread (void *name) {
	printf("Hello, I'm a new thread %s\n", (char*) name);
	sleep(3);
	pthread_exit((void*) 13);
	return (void*) 42;
}

int main() {
	pthread_t thread;
	int i;

	pthread_create(&thread, NULL, &mythread, "Alfred");
	sleep(2);
	
	pthread_join(thread, (void**) &i);
	printf("Return value is %d\n", i);
	return 0;
}