#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include "../queue/bounded_queue.h"

typedef struct thread_wrapper_info{
	void * (*function)(void *);
	void * argument;
} thread_wrapper_info;

typedef struct ThreadPool{
	pthread_t * threads;
	size_t workers;
	size_t live_threads;
	unsigned short shutdown;
	pthread_mutex_t lock;
	pthread_cond_t cond;
	BoundedQueue * task_queue;
} ThreadPool;

void threadpool_waitall(ThreadPool * tp);
void threadpool_addthread(ThreadPool * tp);
void threadpool_initthread(ThreadPool * tp);

ThreadPool * threadpool_init(size_t workers){
	ThreadPool * tp = malloc(sizeof(ThreadPool));
	assert(tp);
	tp->workers = workers;
	tp->live_threads = 0;
	tp->shutdown = 1;

	tp->threads = malloc(workers * sizeof(pthread_t));
	memset(tp->threads, 0, sizeof(workers * sizeof(pthread_t)));

	pthread_mutex_init(&tp->lock, NULL);
	pthread_cond_init(&tp->cond, NULL);

	tp->task_queue = boundedqueue_init(workers);
	return tp;
}

void threadpool_destroy(ThreadPool * tp){
	threadpool_waitall(tp);
	free(tp->threads);
	boundedqueue_destroy(tp->task_queue);
	pthread_mutex_destroy(&tp->lock);
	pthread_cond_destroy(&tp->cond);
	free(tp);
	return;
}

void * thread_wrapper(void * tp_ptr){
	ThreadPool * tp = (ThreadPool *)tp_ptr;
	while(1){
		thread_wrapper_info * info = (thread_wrapper_info *)boundedqueue_deque(tp->task_queue);
		if(info == NULL){
			break;
		}
		(*info->function)(info->argument);
		free(info);
	}
	pthread_mutex_lock(&tp->lock);
	tp->live_threads--;
	pthread_cond_broadcast(&tp->cond);
	pthread_mutex_unlock(&tp->lock);
	return NULL;
}

void threadpool_addtask(ThreadPool * tp, void * function, void * arg){
	pthread_mutex_lock(&tp->lock);
	if(tp->shutdown){
		tp->shutdown = 0;
		threadpool_initthread(tp);
	}
	pthread_mutex_unlock(&tp->lock);

	thread_wrapper_info * info = malloc(sizeof(thread_wrapper_info));
	info->function = (void * (*)(void *))function;
	info->argument = arg;
	boundedqueue_enque(tp->task_queue, (void *)info);
}
void threadpool_endtask(ThreadPool * tp){
	for(size_t i = 0; i < tp->workers; ++i){
		boundedqueue_enque(tp->task_queue, NULL);
	}
	return;
}

void threadpool_initthread(ThreadPool * tp){
	for(size_t i = 0; i < tp->workers; ++i){
		threadpool_addthread(tp);
	}
	return;
}
void threadpool_addthread(ThreadPool * tp){
	pthread_mutex_lock(&tp->lock);
	assert(tp->live_threads < tp->workers);
	if(pthread_create(&tp->threads[tp->live_threads], NULL, (void *)thread_wrapper, (void *)tp)){
		fprintf(stderr, "pthread_create failed.\n");
	}
	else{
		tp->live_threads++;
	}
	pthread_mutex_unlock(&tp->lock);
	return;
}

void threadpool_waitall(ThreadPool * tp){
	threadpool_endtask(tp);
	pthread_mutex_lock(&tp->lock);
	tp->shutdown = 1;
	while(tp->live_threads > 0){
		pthread_cond_wait(&tp->cond, &tp->lock);
	}
	pthread_mutex_unlock(&tp->lock);
	return;
}
