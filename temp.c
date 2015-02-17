void ack_received(int temp_seq_num){
	int temp_position = seq_num_pos_in_buf[temp_seq_num];
	buffer_free_info[temp_position] = 0;
	len_of_packets_in_buf[temp_position] = 0;
	seq_num_pos_in_buf[temp_seq_num] = -1;
	return;
}