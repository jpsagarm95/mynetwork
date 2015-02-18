all: udpserver udpclient

udpserver: udpserver.o
	gcc -o udpserver udpserver.o 
	
udpclient: udpclient.o generator.o
	gcc -o udpclient udpclient.o generator.o -lm -pthread
	
udpclient.o: udpclient.c
	gcc -c -g -w udpclient.c
	
udpserver.o: udpserver.c
	gcc -c -g -w udpserver.c

generator.o: generator.c
	gcc -c -g -w generator.c

clean:
	rm *.o udpclient udpserver
