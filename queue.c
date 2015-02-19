#include <stdio.h>
#include <stdlib.h>
#include "queue.h"


// typedef struct queue_node_{
// 	struct queue_node_ * next;
// 	int time;
// 	int sequence_number;
// }queue_node;

// typedef struct queue_
// {
// 	queue_node * front;
// 	queue_node * rear;
// 	int num_of_elements;
// }queue;

queue* add_element(float time, int sequence_number, float timeout, queue* Q){
	queue_node* temp = (queue_node*)malloc(sizeof(queue_node));
	temp->next = NULL;
	temp->time = time;
	temp->timeout = timeout;
	temp->sequence_number = sequence_number;
	if(Q->num_of_elements == 0){
		Q->front = temp;
	}else{
		Q->rear->next = temp;
	}
	Q->rear = temp;
	Q->num_of_elements += 1;
	return Q;
}

queue* remove_element(queue* Q){
	queue_node* temp;
	temp = Q->front;
	if(Q->num_of_elements == 1){
		Q->rear = NULL;
	}else{

	}
	if(Q->num_of_elements == 0){
		return Q;
	}
	Q->front = Q->front->next;
	free(temp);
	Q->num_of_elements -= 1;
	return Q;
}

queue* remove_specific_element(int sequence_number, queue* Q){
	if(Q->front == NULL)
		return Q;
	if(Q->front->sequence_number == sequence_number){
		return remove_element(Q);
	}
	queue_node* prev = Q->front;
	queue_node* temp = Q->front->next;
	while(temp != NULL){
		if(temp->sequence_number == sequence_number){
			prev->next = temp->next;
			if(temp->next == NULL){
				Q->rear = prev;
			}
			free(temp);
			Q->num_of_elements -= 1;
			return Q;
		}
		prev = temp;
		temp = temp->next;
	}
	return Q;
}


float get_specific_element(int sequence_number, queue* Q){
	float value;
	if(Q->front == NULL)
		return 0;
	if(Q->front->sequence_number == sequence_number){
		return (Q->front->time - Q->front->timeout);
	}
	queue_node* prev = Q->front;
	queue_node* temp = Q->front->next;
	while(temp != NULL){
		if(temp->sequence_number == sequence_number){
			value = temp->time - temp->timeout;
			// printf("%d %d\n",temp->time, temp->timeout );
			return value;
		}
		prev = temp;
		temp = temp->next;
	}
	return 0;
}

queue* queue_init(){
	queue* Q = (queue*)malloc(sizeof(queue));
	Q->num_of_elements = 0;
	Q->front = NULL;
	Q->rear = NULL;
}

// int main(){
// 	queue* Q;
// 	Q = queue_init();
// 	int i;
// 	for(i = 0 ; i < 10; i++){
// 		Q = add_element(0, i, Q);
// 	}
// 	Q = remove_specific_element(0, Q);
// 	for (i = 0; i < 10; ++i)
// 	{
// 		/* code */
// 		if(i!=9){
// 			printf("%d\n",Q->front->sequence_number );
// 		}
// 		Q = remove_element(Q);
// 	}

// }