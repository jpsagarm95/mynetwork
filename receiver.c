#include<stdio.h>
#include<string.h>
#include<stdlib.h>
int debug_mode;
int port_number;
int MAX_PACKETS;
int seq_num;
int WINDOW_SIZE;
int BUFFER_SIZE;
double PACKET_ERROR_RATE;
int main(int argc, char * argv[]){
	int i = 0;
	debug_mode = 0;
	seq_num = 3;
	for(i = 1 ; i < argc ; i++){
		if(strcmp(argv[i],"-d") == 0){
			debug_mode = 1;
		}else if(strcmp(argv[i],"-p") == 0){
			i++;
			port_number = atoi(argv[i]);
		}else if(strcmp(argv[i],"-N") == 0){
			i++;
			MAX_PACKETS = atoi(argv[i]);
		}else if(strcmp(argv[i],"-n") == 0){
			i++;
			seq_num = atoi(argv[i]);
		}else if(strcmp(argv[i],"-W") == 0){
			i++;
			WINDOW_SIZE = atoi(argv[i]);
		}else if(strcmp(argv[i],"-B") == 0){
			i++;
			BUFFER_SIZE = atoi(argv[i]);
		}else if(strcmp(argv[i],"-e") == 0){
			i++;
			PACKET_ERROR_RATE = atof(argv[i]);
		}
	}
//	printf(" %d %d %d %d %d %d %f\n", debug_mode, port_number, MAX_PACKETS, seq_num, WINDOW_SIZE, BUFFER_SIZE, PACKET_ERROR_RATE);
	return 0;
}
