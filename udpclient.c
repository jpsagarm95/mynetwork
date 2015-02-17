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

/*
Command Line Arguments
Port No :argv[1]
IP address :argv[2]
 */

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

    host = (struct hostent *) gethostbyname((char *) argv[2]);

    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[1]));
    server_addr.sin_addr = *((struct in_addr *) host->h_addr);
    bzero(&(server_addr.sin_zero), 8);
    addr_len = sizeof (struct sockaddr);

    while (1) {
        printf("Type a word to send(q or Q to quit):");
        scanf("%s", send_data);

        if ((strcmp(send_data, "q") == 0) || strcmp(send_data, "Q") == 0)
            break;

        else
            sendto(sock, send_data, strlen(send_data), 0,
                (struct sockaddr *) &server_addr, sizeof (struct sockaddr));
                
            bytes_read = recvfrom(sock, recv_data, 1024, 0,
                (struct sockaddr *) &server_addr, &addr_len);
            recv_data[bytes_read] = '\0';
            printf("Data recveived: %s\n", recv_data);
    }
}
