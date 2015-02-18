#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

queue* add_element(int time, int sequence_number, queue* Q){
	queue_node* temp = (queue_node*)malloc(sizeof(queue_node));
	temp->next = NULL;
	temp->time = time;
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
// 		Q = add_element(i, Q);
// 	}
// 	for (i = 0; i < 10; ++i)
// 	{
// 		/* code */
// 		printf("%d\n",Q->front->time );
// 		Q = remove_element(Q);
// 	}
// 	Q = remove_element(Q);
// }