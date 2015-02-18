/* udpclient.c */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include "generator.h"

int debug_mode;
char ip_name[100];
int port_number;
int seq_num_bits;
int MAX_PACKET_LENGTH;
int PACKET_GEN_RATE;
int MAX_PACKETS;
int WINDOW_SIZE;
int BUFFER_SIZE;
int HEADER_SIZE;	// just the number of bytes req for the seq number
char** buffer;
int* len_of_packets_in_buf;
int* buffer_free_info;
int curr_seq_num;
int* seq_num_pos_in_buf;
int base_seq_num;
// three way communication -1 packet acked, 0 wait, 1 resend
int* packets_need_resend;


pthread_mutex_t buffer_lock;
pthread_mutex_t ack_lock;
// struct thread_info {    
//    pthread_t thread_id;     
//    int       thread_num;    
//    char     *argv_string;   
// };

/*
Command Line Arguments
Port No :argv[1]
IP address :argv[2]
 */

int new_window(){
	int tot = pow(2, seq_num_bits);
	base_seq_num = base_seq_num % tot;
	int i;
	for(i = 0 ; i < WINDOW_SIZE ; i++){
		packets_need_resend[i] = 1;
	}
}

int main(int argc, char *argv[]) {
    int sock;
    struct sockaddr_in server_addr, client_addr;
    struct hostent *host;
    char send_data[1024];
    char recv_data[1024];
    int addr_len, bytes_read;
    
    if (argc < 2) {
        printf("Enter PortNo");
        exit(0);
    }

    if (pthread_mutex_init(&buffer_lock, NULL) != 0){
        printf("\n mutex init failed\n");
        return 1;
    }

    initializer(argc, argv);

    host = (struct hostent *) gethostbyname(ip_name);

    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port_number);
    server_addr.sin_addr = *((struct in_addr *) host->h_addr);
    bzero(&(server_addr.sin_zero), 8);
    addr_len = sizeof (struct sockaddr);

    
    printf("%s\n", "Initialized");
    pthread_t gen;
    pthread_create(&gen, NULL, &packet_generator, NULL);
    //packet_generator(NULL);
    int i, total_ack, check;
    printf("%s\n", "thread created");
    while (1) {
    	total_ack = 1;
		for(i = 0; i < WINDOW_SIZE; i++){
			pthread_mutex_lock(&ack_lock);

			check = packets_need_resend[i];
			pthread_mutex_unlock(&ack_lock);
			if(check == 1){
				total_ack = 0;
				int pos = seq_num_pos_in_buf[base_seq_num + i];
				if(pos == -1){
					i--;
					continue;
				}
				pthread_mutex_lock(&buffer_lock);
				int total_packet_length = len_of_packets_in_buf[pos] + HEADER_SIZE;
				fwrite(buffer[pos] + sizeof(int), sizeof(char), total_packet_length - sizeof(int) , stdout);
				printf("\n");
				sendto(sock, buffer[pos], total_packet_length, 0, (struct sockaddr *) &server_addr, sizeof (struct sockaddr));
				pthread_mutex_unlock(&buffer_lock);

				pthread_mutex_lock(&ack_lock);
				packets_need_resend[i] = 0;
				pthread_mutex_unlock(&ack_lock);
			}else if(check == 0){
				total_ack = 0;
			}
		}
		if(total_ack == 1){
			pthread_mutex_lock(&ack_lock);
			new_window();
			pthread_mutex_unlock(&ack_lock);
		}
    }
    pthread_mutex_destroy(&buffer_lock);
    return 0;
}
