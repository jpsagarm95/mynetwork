#include <stdio.h>

void* packet_generator(void* param);
int size_gen();
void initializer(int argc, char* argv[]);
int get_free_buf_position();
void buffer_allocation(char* packet, int packet_length);