#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "../elementtype.h"

typedef struct QueueNode{
	ELEMENTTYPE * element;
	struct QueueNode * prev;
	struct QueueNode * next;
} QueueNode;

typedef struct BoundedQueue{
	size_t size;
	QueueNode * head;
	QueueNode * tail;
	size_t max;
	pthread_mutex_t lock;
	pthread_cond_t cond;
} BoundedQueue;

BoundedQueue * boundedqueue_init(unsigned int max){
	BoundedQueue * queue = calloc(1, sizeof(BoundedQueue));
	queue->max = max;
	queue->size = 0;
	pthread_mutex_init(&queue->lock, NULL);
	pthread_cond_init(&queue->cond, NULL);
	return queue;
}

void boundedqueue_enque(BoundedQueue * queue, ELEMENTTYPE * X){
	pthread_mutex_lock(&queue->lock);
	while(queue->size >= queue->max){
		pthread_cond_wait(&queue->cond, &queue->lock);
	}
	if(queue->size == 0){
		queue->head = queue->tail = malloc(sizeof(QueueNode));
		assert(queue->head && queue->tail);
		queue->head->prev = NULL;
		queue->head->next = NULL;
		queue->head->element = X;
	}
	else{
		queue->tail->next = malloc(sizeof(QueueNode));
		assert(queue->tail->next);
		queue->tail->next->prev = queue->tail;
		queue->tail->next->element = X;
		queue->tail->next->next = NULL;
		queue->tail = queue->tail->next;
	}
	queue->size++;

	pthread_cond_broadcast(&queue->cond);
	pthread_mutex_unlock(&queue->lock);
	return;
}

ELEMENTTYPE * boundedqueue_deque(BoundedQueue * queue){
	pthread_mutex_lock(&queue->lock);
	while(queue->size == 0){
		pthread_cond_wait(&queue->cond, &queue->lock);
	}
	ELEMENTTYPE * element = queue->head->element;
	if(queue->head == queue->tail){
		free(queue->head);
		queue->head = queue->tail = NULL;
	}
	else{
		QueueNode * newhead = queue->head->next;
		free(queue->head);
		queue->head = newhead;
		queue->head->prev = NULL;
	}
	queue->size--;

	pthread_cond_broadcast(&queue->cond);
	pthread_mutex_unlock(&queue->lock);
	return element;
}

void boundedqueue_destroy(BoundedQueue * queue){
	QueueNode * iter = queue->head;
	while(iter){
		QueueNode * next = iter->next;
		free(iter);
		iter = next;
	}
	pthread_cond_destroy(&queue->cond);
	pthread_mutex_destroy(&queue->lock);
	free(queue);
}
