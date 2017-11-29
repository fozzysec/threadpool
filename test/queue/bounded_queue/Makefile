#bsdmake Makefile
CC=clang
LIBS=-pthread
CFALGS=-g

all: bounded_queue.o test_bounded_queue.o
	$(CC) -o boundedqueue $(LIBS) $(CFLAGS) $(.ALLSRC)
test_bounded_queue.o: bounded_queue.c
	$(CC) -o $(.TARGET) $(CFLAGS) -c $(.ALLSRC)
bounded_queue.o: ../../../src/queue/bounded_queue.c
	$(CC) -o $(.TARGET) $(CFLAGS) -c $(.ALLSRC)
clean: 
	rm test_bounded_queue.o bounded_queue.o
