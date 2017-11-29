#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../../../src/queue/bounded_queue.h"

void producer(BoundedQueue *);
void consumer(BoundedQueue *);
int main(void){
	BoundedQueue * queue = boundedqueue_init(5);
	pthread_t con, pro;
	pthread_create(&con, NULL, (void *)consumer,  queue);
	pthread_create(&pro, NULL, (void *)producer, queue);
	pthread_join(pro, NULL);
	pthread_join(con, NULL);
	boundedqueue_destroy(queue);
	return 0;
}
void producer(BoundedQueue * queue){
	for(int i = 0; i < 10; ++i){
		ELEMENTTYPE * element = malloc(sizeof(ELEMENTTYPE));
		element->data = malloc(2*sizeof(char));
		element->size = 2;
		((char *)element->data)[0] = 'A' + i;
		((char *)element->data)[1] = '\0';
		boundedqueue_enque(queue, element);
		printf("producer: %s enqueued\n", (char *)element->data);
		sleep(3);
	}
}
void consumer(BoundedQueue * queue){
	for(int i = 0; i < 10; ++i){
		ELEMENTTYPE * element = boundedqueue_deque(queue);
		printf("%s on %dth loop\n", (char *)element->data, i);
		free(element->data);
		free(element);
		sleep(1);
	}
}
