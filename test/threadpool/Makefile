#bsdmake Makefile
CC=clang
LIBS=-pthread -L/usr/local/lib
INC=-I/usr/local/include
CFALGS=-g

all: test.o threadpool.o bounded_queue.o
	$(CC) -o threadpool $(LIBS) $(CFLAGS) $(.ALLSRC)
test.o: test.c
	$(CC) -o $(.TARGET) $(INC) $(CFLAGS) -c $(.ALLSRC)
threadpool.o: ../../src/threadpool/threadpool.c
	$(CC) -o $(.TARGET) $(INC) $(CFLAGS) -c $(.ALLSRC)
bounded_queue.o: ../../src/queue/bounded_queue.c
	$(CC) -o $(.TARGET) $(INC) $(CFLAGS) -c $(.ALLSRC)
clean: 
	rm -f test.o threadpool.o bounded_queue.o
