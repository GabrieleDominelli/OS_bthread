#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>

int account_balance = 100;
pthread_mutex_t mutex;

void* thread1(void* arg) {
	int wallet = 0;
	pthread_mutex_lock(&mutex);
	
	if (account_balance >= 100) {
		account_balance -= 100;
		pthread_mutex_unlock(&mutex);
		wallet = 100;
	} else {
		pthread_mutex_unlock(&mutex);
		printf("Cannot draw money\n");
		pthread_exit((void*)-1);
	}
	assert(account_balance >= 0);
	
	pthread_mutex_lock(&mutex);
	account_balance += 50;
	pthread_mutex_unlock(&mutex);
	
	printf("Balance: %d\n", account_balance);
}

void* thread2(void* arg) {
	pthread_mutex_lock(&mutex);
	
	account_balance += 50;
	if (account_balance >= 100) {
		account_balance -= 100;
		pthread_mutex_unlock(&mutex);
		printf("Pay bills\n");
	} else {
		pthread_mutex_unlock(&mutex);
		printf("Not enough money\n");
	}
	assert(account_balance >= 0);
}

void main(void) {
	pthread_t t1, t2;

	pthread_mutex_init(&mutex, NULL);
	
	pthread_create(&t1, NULL, thread1, NULL);
	pthread_create(&t2, NULL, thread2, NULL);
	
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
}