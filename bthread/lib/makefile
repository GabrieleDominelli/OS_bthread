# https://solver.assistedcoding.eu/makefilegen

OBJS	= main.o bthread_private.o bthread.o tqueue.o
SOURCE	= main.c bthread_private.c bthread.c tqueue.c
HEADER	= bthread_private.h bthread.h tqueue.h
OUT	= main
CC	 = gcc
FLAGS	 = -g -c -Wall
LFLAGS	 = 

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

main.o: main.c
	$(CC) $(FLAGS) main.c 

bthread_private.o: bthread_private.c
	$(CC) $(FLAGS) bthread_private.c 

bthread.o: bthread.c
	$(CC) $(FLAGS) bthread.c 

tqueue.o: tqueue.c
	$(CC) $(FLAGS) tqueue.c 


clean:
	rm -f $(OBJS) $(OUT)