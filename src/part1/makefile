# makefile

all: dataserver client

reqchannel.o: reqchannel.h reqchannel.cpp
	g++-4.7 -c -g reqchannel.cpp

dataserver: dataserver.cpp reqchannel.o 
	g++-4.7 -o dataserver dataserver.cpp reqchannel.o -lpthread

client: client.cpp reqchannel.o
	g++-4.7 -o client client.cpp reqchannel.o
