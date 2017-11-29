#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include "../../src/threadpool/threadpool.h"

#define WORKERS 48

void * myfunc(void * arg){
	int n = (int)arg;
	printf("thread %d running.\n", n);
	return NULL;
}

int main(void){
	ThreadPool * tp = threadpool_init(WORKERS);
	int ret;
	for(long i = 0; i < 100000; ++i){
		threadpool_addtask(tp, myfunc, (void *)i);
	}
	threadpool_waitall(tp);
	for(long i = 0; i < 100000; ++i){
		threadpool_addtask(tp, myfunc, (void *)i + 100000);
	}
	threadpool_destroy(tp);

	return 0;
}
