#ifndef _BOUNDED_QUEUE_HEADER_
#define _BOUNDED_QUEUE_HEADER_
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

BoundedQueue * boundedqueue_init(unsigned int);
void boundedqueue_enque(BoundedQueue *, ELEMENTTYPE *);
ELEMENTTYPE * boundedqueue_deque(BoundedQueue *);
void boundedqueue_destroy(BoundedQueue *);
#endif
