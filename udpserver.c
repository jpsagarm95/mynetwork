/* udpserver.c */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "receiver.h"


int debug_mode;
int port_number;
int MAX_PACKETS;
int seq_num_bits;
int WINDOW_SIZE;
int BUFFER_SIZE;
double PACKET_ERROR_RATE;
int* packets_received;
int base_seq_num;
int new_window_start;
/*
Port No has to be given as command line argument 
 */

int main(int argc, char *argv[]) {
    int sock;
    int addr_len, bytes_read;
    char recv_data[1024];
    
    initialize(argc, argv);

    struct sockaddr_in server_addr, client_addr;

    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("Socket");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port_number);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    bzero(&(server_addr.sin_zero), 8);

    if (bind(sock, (struct sockaddr *) &server_addr,
            sizeof (struct sockaddr)) == -1) {
        perror("Bind");
        exit(1);
    }

    addr_len = sizeof (struct sockaddr);

    printf("\nUDPServer Waiting for client on port %d\n", port_number);
    fflush(stdout);
    int i = 0;
    int seq, drop, tot = pow(2, seq_num_bits);
    i = 0;
    int j = 0;
    int send_data[WINDOW_SIZE + 1];
    while (1) {
    	i++;
        j = 0;
        bytes_read = recvfrom(sock, recv_data, 1024, 0, (struct sockaddr *) &client_addr, &addr_len);
        drop = drop_or_not();
        printf("Dropping %d\n", drop);
        if(drop == 1){
            continue;
        }
        strncpy((char *)&seq, recv_data, sizeof(int));
        if((seq < base_seq_num) && (new_window_start != 1)){
            printf("%s %d\n", "In the discusssion", seq);
            packets_received[tot - base_seq_num + seq] = 1;
        }else if(seq < base_seq_num){

        }else{
            packets_received[seq - base_seq_num] = 1;
        }
        
        //printf("Bytes sent %d\n", bytes_read);
        //recv_data[bytes_read] = '\0';
        //printf("\n(%s , %d) said : ", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
        //fwrite(recv_data + sizeof(int), sizeof(char), bytes_read - sizeof(int), stdout);
        printf("%d\n", base_seq_num);
        fflush(stdout);
        send_data[0] = base_seq_num;
        printf("%d\n", send_data[0]);
        for(i = 0 ; i < WINDOW_SIZE ; i++){
            send_data[i + 1] = packets_received[i];
            printf("%d ", send_data[i+1]);
        }
        printf("\n");
        sendto(sock, (char* )send_data, (WINDOW_SIZE + 1) * sizeof(int), 0, (struct sockaddr *) &client_addr, sizeof (struct sockaddr));
        new_window_start = 0;
        for(i = 0 ; i < WINDOW_SIZE ; i++){
            if(packets_received[i] == 1){
                j++;
            }
        }
        if(j == WINDOW_SIZE){
            new_window();
        }
    }
    
    return 0;
}
