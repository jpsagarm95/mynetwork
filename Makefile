all: udpserver udpclient

udpserver: udpserver.o receiver.o
	gcc -o udpserver udpserver.o receiver.o -lm
	
udpclient: udpclient.o generator.o queue.o receive_ack.o
	gcc -o udpclient udpclient.o generator.o queue.o receive_ack.o -lm -pthread
	
udpclient.o: udpclient.c
	gcc -c -g -w udpclient.c
	
udpserver.o: udpserver.c
	gcc -c -g -w udpserver.c

generator.o: generator.c
	gcc -c -g -w generator.c

queue.o: queue.c
	gcc -c -g -w queue.c

receive_ack.o: receive_ack.c
	gcc -c -g -w receive_ack.c

receiver.o: receiver.c
	gcc -c -g receiver.c

clean:
	rm *.o udpclient udpserver
