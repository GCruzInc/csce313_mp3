# makefile

all: dataserver simpleclient

reqchannel.o: reqchannel.H reqchannel.C
	g++ -c -g reqchannel.C

dataserver: dataserver.C reqchannel.o 
	g++ -o dataserver dataserver.C reqchannel.o -lpthread

simpleclient: client.c reqchannel.o
	g++ -o client client.c reqchannel.o
	
clean:
	rm *.o client dataserver
	
run:
	./client
