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

extern pthread_mutex_t buffer_lock;
extern pthread_mutex_t ack_lock;
extern pthread_mutex_t timer_lock;

void* receive_ack(void* param){
	
	int bytes_read;
	int recv_data[WINDOW_SIZE + 1];
	int i;
	int recv_base_seq_number;
	while(1){
		bytes_read = recvfrom(sock, (char *)recv_data, (WINDOW_SIZE + 1) * sizeof(int), 0, (struct sockaddr *) &server_addr, sizeof(struct sockaddr));
		for(i = 0 ; i < WINDOW_SIZE + 1 ; i++)
			printf("%d\n", recv_data[i]);
	}
	return NULL;
}
