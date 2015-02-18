#include <stdio.h>

typedef struct queue_node_{
	struct queue_node_ * next;
	int value;
}queue_node;

typedef struct queue_
{
	queue_node * front;
	queue_node * rear;
	int num_of_elements;
}queue;

queue* add_element(int value, queue* Q);
queue* remove_element(queue* Q);
queue* queue_init();