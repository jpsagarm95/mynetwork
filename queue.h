#include <stdio.h>

typedef struct queue_node_{
	struct queue_node_ * next;
	int time;
	int sequence_number;
}queue_node;

typedef struct queue_
{
	queue_node * front;
	queue_node * rear;
	int num_of_elements;
}queue;

queue* add_element(int time, int sequence_number, queue* Q);
queue* remove_element(queue* Q);
queue* queue_init();
queue* remove_specific_element(int sequence_number, queue* Q);