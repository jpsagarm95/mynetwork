#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "generator.h"
#include "queue.h"

extern int debug_mode;
extern char ip_name[100];
extern int port_number;
extern int seq_num_bits;
extern int MAX_PACKET_LENGTH;
extern int PACKET_GEN_RATE;
extern int MAX_PACKETS;
extern int WINDOW_SIZE;
extern int BUFFER_SIZE;
extern int HEADER_SIZE;	// just the number of bytes req for the seq number
extern char** buffer;
extern int* len_of_packets_in_buf;
extern int* buffer_free_info;
extern int curr_seq_num;
extern int* seq_num_pos_in_buf;
extern int* packets_need_resend;
extern queue* timer_queue;
extern int timeout;
extern long total_suc_transmissions;
extern int temp_timeout;

extern pthread_mutex_t buffer_lock;
extern pthread_mutex_t ack_lock;

// need to change to the uniform distribution
int size_gen(){
	int i = rand() % (MAX_PACKET_LENGTH - 40);
	return (40 + i);
}

void initializer(int argc, char* argv[]){
	srand(time(NULL));
	int i = 0;
	timeout = 300;
	total_suc_transmissions = 0;
	temp_timeout = 0;
	timer_queue = (queue*)malloc(sizeof(queue));
	debug_mode = 0;
	seq_num_bits = 3;
	for(i = 1 ; i < argc ; i++){
		if(strcmp(argv[i],"-d") == 0){
			debug_mode = 1;
		}else if(strcmp(argv[i],"-s") == 0){
			i++;
			strcpy(ip_name, argv[i]);
		}else if(strcmp(argv[i],"-p") == 0){
			i++;
			port_number = atoi(argv[i]);
		}else if(strcmp(argv[i],"-n") == 0){
			i++;
			seq_num_bits = atoi(argv[i]);
		}else if(strcmp(argv[i],"-L") == 0){
			i++;
			MAX_PACKET_LENGTH = atoi(argv[i]);
		}else if(strcmp(argv[i],"-R") == 0){
			i++;
			PACKET_GEN_RATE = atoi(argv[i]);
		}else if(strcmp(argv[i],"-N") == 0){
			i++;
			MAX_PACKETS = atoi(argv[i]);
		}else if(strcmp(argv[i],"-W") == 0){
			i++;
			WINDOW_SIZE = atoi(argv[i]);
		}else if(strcmp(argv[i],"-B") == 0){
			i++;
			BUFFER_SIZE = atoi(argv[i]);
		}
	}

	// change this
	if(seq_num_bits < (sizeof(int) * 8)){
		HEADER_SIZE = sizeof(int) ;
	}
	int tot = pow(2, seq_num_bits);
	buffer = (char **)malloc(sizeof(char *) * BUFFER_SIZE);
	len_of_packets_in_buf = (int *)malloc(sizeof(int) * BUFFER_SIZE);
	buffer_free_info = (int *)malloc(sizeof(int) * BUFFER_SIZE);
	seq_num_pos_in_buf = (int *)malloc(sizeof(int) * tot);
	packets_need_resend = (int *)malloc(sizeof(int) * WINDOW_SIZE);
	for(i = 0 ; i < WINDOW_SIZE ; i++){
		packets_need_resend[i] = 1;
	}
	for(i = 0 ; i < BUFFER_SIZE ; i++){
		buffer[i] = (char *)malloc(sizeof(char) * (HEADER_SIZE + MAX_PACKET_LENGTH));
		buffer_free_info[i] = 0;
		len_of_packets_in_buf[i] = 0;
	}
	for(i = 0; i < tot; i++){
		seq_num_pos_in_buf[i] = -1;
	}
	return;
}

int get_free_buf_position(){
	int i;
	for(i = 0; i < BUFFER_SIZE; i++){
		if( buffer_free_info[i] == 0 ){
			return i;
		}
	}
	return -1;
}

void buffer_allocation(char* packet, int packet_length){
	int temp_position = get_free_buf_position();
	if(temp_position == -1){
		return;
	}
	int temp_seq_num = curr_seq_num;
	int tot = pow(2, seq_num_bits);
	curr_seq_num = (curr_seq_num + 1) % tot;
	// printf("seq number %d at %d\n", curr_seq_num, temp_position);
	strncpy(buffer[temp_position], (char *)&temp_seq_num, HEADER_SIZE);
	strncpy(buffer[temp_position] + HEADER_SIZE, packet, packet_length);
	buffer_free_info[temp_position] = 1;
	len_of_packets_in_buf[temp_position] = packet_length;
	seq_num_pos_in_buf[temp_seq_num] = temp_position;
	return;
}


void* packet_generator(void* param){

//	printf("%d %s %d %d %d %d %d %d %d\n", debug_mode, ip_name, port_number, seq_num_bits, MAX_PACKET_LENGTH, PACKET_GEN_RATE, MAX_PACKETS, WINDOW_SIZE, BUFFER_SIZE);
	FILE* fp = fopen("check_file.txt","r");
	char buf[HEADER_SIZE + MAX_PACKET_LENGTH];
	int temp_packet_length = 0;
	int i = 0;
	int g = 0, read_bytes;
	while(1){
		// printf("%s\n", "Starting");
		// for(i = 0 ; i < pow(2, seq_num_bits) ; i++){
		// 	printf("%d\n", seq_num_pos_in_buf[i]);
		// }
		temp_packet_length = size_gen();
		read_bytes = fread(buf, sizeof(char), temp_packet_length, fp);
		if(read_bytes == 0)
			break;
		
		pthread_mutex_lock(&buffer_lock);
		buffer_allocation(buf, temp_packet_length);
		pthread_mutex_unlock(&buffer_lock);
//		printf("%s\n","I am out");

		double temp = (1.0/PACKET_GEN_RATE) * 1000000;
//		printf("%f\n",temp);
		usleep(temp);
		g++;
	}

	// printf("%s\n", "Its generator time");
	// for(i = 0; i < BUFFER_SIZE; i++){
	// 	fwrite((buffer[i] + HEADER_SIZE), sizeof(char), (len_of_packets_in_buf[i]), stdout);
	// 	printf("\n");
	// }

//	printf("%d %s %d %d %d %d %d %d %d\n", debug_mode, ip_name, port_number, seq_num_bits, MAX_PACKET_LENGTH, PACKET_GEN_RATE, MAX_PACKETS, WINDOW_SIZE, BUFFER_SIZE);
	printf("\n");
	printf("%s\n", "File over");
	return NULL;
}
