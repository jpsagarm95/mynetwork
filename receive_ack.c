#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include "queue.h"

extern int sock;
extern struct sockaddr_in server_addr;
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
extern int base_seq_num;
// three way communication -1 packet acked, 0 wait, 1 resend
extern int* packets_need_resend;
extern queue* timer_queue;
extern int timeout;
extern long total_suc_transmissions;
extern int temp_timeout;

extern pthread_mutex_t buffer_lock;
extern pthread_mutex_t ack_lock;
extern pthread_mutex_t timer_lock;

void* receive_ack(void* param){
	int bytes_read;
	int recv_data[WINDOW_SIZE + 1];
	int i;
	int recv_base_seq_number;
	int acks_got[WINDOW_SIZE];
	for(i = 0 ; i < WINDOW_SIZE ; i++){
		acks_got[i] = 0;
	}
	while(1){


		fd_set rfds;
		struct timeval tv;
		int retval;

		/* Watch stdin (fd 0) to see when it has input. */
		FD_ZERO(&rfds);
		FD_SET(sock, &rfds);

		/* Wait up to five seconds. */
		tv.tv_sec = 0;
		tv.tv_usec = 1;

		retval = select(FD_SETSIZE, &rfds, NULL, NULL, &tv);
		// printf("%d\n",retval );
		/* Don't rely on the value of tv now! */
		if (retval == 0 || retval == -1){
			pthread_mutex_lock(&timer_lock);
			if(timer_queue->front != NULL){
				struct timeval timenow;
				gettimeofday(&timenow, NULL);
				if(timer_queue->front->time < ((timenow.tv_sec  % 1000)* 1000 + (timenow.tv_usec / 1000))){
					pthread_mutex_lock(&ack_lock);
					// printf("Later checking\n");
					// printf("%d\n", timer_queue->front->time);
					// printf("%d\n", ((timenow.tv_sec  % 1000)* 1000 + (timenow.tv_usec / 1000)));
					packets_need_resend[timer_queue->front->sequence_number] = 1;
					timer_queue = remove_element(timer_queue);
					pthread_mutex_unlock(&ack_lock);
				}
			}
			pthread_mutex_unlock(&timer_lock);
		}
		else{
			// printf("%s\n", "inside it");
			bytes_read = recvfrom(sock, (char *)recv_data, (WINDOW_SIZE + 1) * sizeof(int), 0, (struct sockaddr *) &server_addr, sizeof(struct sockaddr));
			recv_base_seq_number = recv_data[0];
			printf("%s\n", "Starting recv_data");
			for(i = 0 ; i < (WINDOW_SIZE + 1) ; i++){
				printf("%d ", recv_data[i]);
			}
			printf("\n");
			total_suc_transmissions += 1;
			pthread_mutex_lock(&buffer_lock);
			pthread_mutex_lock(&ack_lock);
			pthread_mutex_lock(&timer_lock);
			clear_buffer(recv_base_seq_number, recv_data + 1);
			pthread_mutex_unlock(&buffer_lock);
			pthread_mutex_unlock(&ack_lock);
			pthread_mutex_unlock(&timer_lock);
			// printf("%s\n", "Starting");
			// for(i = 0 ; i < pow(2, seq_num_bits) ; i++){
			// 	printf("%d ", seq_num_pos_in_buf[i]);
			// }
			printf("\n\n\n\n\n\n");
		}
	}	
	return NULL;
}

void clear_buffer(int num, int* arr){
	int i = 0;
	int act_num, pos;
	int tot = pow(2, seq_num_bits);
	for(i = 0 ; i < WINDOW_SIZE ; i++){
		act_num = ( num + i ) % tot;

		pos = seq_num_pos_in_buf[act_num];
		printf("%s\n", "Clear Buffer");
		printf("%d %d\n",act_num , pos);
		if(pos == -1){
			continue;
		}

		if(arr[i] == 1){
			packets_need_resend[i] = -1;
			buffer_free_info[pos] = 0;
			len_of_packets_in_buf[pos] = 0;
			seq_num_pos_in_buf[act_num] = -1;
			timer_queue = remove_specific_element(i, timer_queue);
		}
	}
}