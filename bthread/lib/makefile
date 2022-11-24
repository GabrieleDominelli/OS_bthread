CFLAGS = -ffixed-r8 -ffixed-r9 -ffixed-r10 -ffixed-r11
CC = gcc -O0 -g -Wall -std=gnu11 $(CFLAGS) -Wno-discarded-qualifiers
CC_OBJ = $(CC) -c $(CFLAGS)
CORE_OBJ = tqueue.o bthread.o
LIBS = -lm

all: bthread.o bthread_test tqueue_test

tqueue.o: tqueue.c tqueue.h
	$(CC_OBJ) $< $(LIBS)
	
bthread.o: bthread.c bthread.h tqueue.o
	$(CC_OBJ) $< $(LIBS)

bthread_test: bthread_test.c $(CORE_OBJ)
	$(CC) -o $@ $< $(LIBS) $(CORE_OBJ) 
	

tqueue_test: tqueue_test.c tqueue.o
	$(CC) -o $@ $(LIBS) $^ 
	
style:
	astyle -A8 *.c
	astyle -A8 *.h
	rm -f *.orig
	
clean:
	rm -rf bthread_test tqueue_test
	rm -rf *.o
	
	
