#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define NBYTES 10000

FILE* file;
char* buffer[NBYTES];
unsigned int bytes;

void* reader() {
	bytes = read(fileno(file), buffer, NBYTES);
}

void main(void) {
	pthread_t thread_reader;
	file = fopen("/var/log/syslog", "r");
	
	pthread_create(&thread_reader, NULL, &reader, NULL);
	printf("... reader thread is doing its work...\n");
	sleep(5);
	
	pthread_join(thread_reader, NULL);
	printf("Read finished, got %d bytes.\n", bytes);
	close(fileno(file));
}