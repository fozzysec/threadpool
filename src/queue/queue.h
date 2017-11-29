#ifndef _QUEUE_HEADER_
#define _QUEUE_HEADER_
#include <pthread.h>
#include "../elementtype.h"

typedef struct QueueNode{
	ELEMENTTYPE * element;
	struct QueueNode * prev;
	struct QueueNode * next;
} QueueNode;
typedef struct Queue{
	size_t size;
	QueueNode * head;
	QueueNode * tail;
	pthread_mutex_t lock;
	pthread_cond_t cond;
} Queue;

Queue * queue_init(void);
void queue_enque(Queue *, ELEMENTTYPE *);
ELEMENTTYPE * queue_deque(Queue *);
void queue_destroy(Queue *);
#endif
