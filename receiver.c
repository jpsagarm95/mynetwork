#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

extern int debug_mode;
extern int port_number;
extern int MAX_PACKETS;
extern int seq_num_bits;
extern int WINDOW_SIZE;
extern int BUFFER_SIZE;
extern double PACKET_ERROR_RATE;
extern int base_seq_num;
extern int* packets_received;
extern int new_window_start;

void initialize(int argc, char * argv[]){
	srand(time(NULL));
	int i = 0;
	debug_mode = 0;
	seq_num_bits = 3;
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
			seq_num_bits = atoi(argv[i]);
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
	base_seq_num = 0;
	new_window_start = 1;
	packets_received = (int *)malloc(sizeof(int) * WINDOW_SIZE);
	for(i = 0 ; i < WINDOW_SIZE ; i++){
		packets_received[i] = 0;
	}
	printf(" %d %d %d %d %d %d %f\n", debug_mode, port_number, MAX_PACKETS, seq_num_bits, WINDOW_SIZE, BUFFER_SIZE, PACKET_ERROR_RATE);
	return;
}

// returns 1 to drop, 0 not to drop
int drop_or_not(){
	double x = 100.0 * PACKET_ERROR_RATE;
	int i = rand() % 100 + 1;
	if(i < x){
		return 1;
	}else{
		return 0;
	}
}

void new_window(){
	int tot = pow(2, seq_num_bits);
	base_seq_num = (base_seq_num + WINDOW_SIZE) % tot;
	int i;
	for(i = 0 ; i < WINDOW_SIZE ; i++){
		packets_received[i] = 0;
	}
	new_window_start = 1;
}