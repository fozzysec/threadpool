#bsdmake Makefile
CC=clang
LIBS=-pthread
CFALGS=-g

all: testqueue.o queue.o
	$(CC) -o queue $(LIBS) $(CFLAGS) $(.ALLSRC)
testqueue.o: queue.c
	$(CC) -o $(.TARGET) $(CFLAGS) -c $(.ALLSRC)
queue.o: ../../../src/queue/queue.c
	$(CC) -o $(.TARGET) $(CFLAGS) -c $(.ALLSRC)
clean: 
	rm testqueue.o queue.o
