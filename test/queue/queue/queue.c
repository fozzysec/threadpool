#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "../../../src/queue/queue.h"

void producer(Queue *);
void consumer(Queue *);
int main(void){
	Queue * queue = queue_init();
	pthread_t con, pro;
	pthread_create(&con, NULL, (void *)consumer,  queue);
	pthread_create(&pro, NULL, (void *)producer, queue);
	pthread_join(pro, NULL);
	pthread_join(con, NULL);
	queue_destroy(queue);
	return 0;
}
void producer(Queue * queue){
	for(int i = 0; i < 10; ++i){
		ELEMENTTYPE * element = malloc(sizeof(ELEMENTTYPE));
		element->data = malloc(2*sizeof(char));
		element->size = 2;
		((char *)element->data)[0] = 'A' + i;
		((char *)element->data)[1] = '\0';
		queue_enque(queue, element);
		printf("producer: %s enqueued\n", (char *)element->data);
		//sleep(3);
	}
}
void consumer(Queue * queue){
	for(int i = 0; i < 10; ++i){
		ELEMENTTYPE * element = queue_deque(queue);
		printf("%s on %dth loop\n", (char *)element->data, i);
		free(element->data);
		free(element);
	}
}
