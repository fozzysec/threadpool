#ifndef _THREADPOOL_HEADER_
#define _THREADPOOL_HEADER_
#include <pthread.h>
#include "../queue/bounded_queue.h"

typedef struct ThreadPool{
	pthread_t * threads;
	size_t workers;
	size_t live_threads;
	pthread_mutex_t lock;
	pthread_cond_t cond;
	BoundedQueue * task_queue;
} ThreadPool;

ThreadPool * threadpool_init(size_t workers);
void threadpool_destroy(ThreadPool * tp);
void threadpool_addtask(ThreadPool * tp, void * function, void * arg);
void threadpool_waitall(ThreadPool * tp);
#endif
