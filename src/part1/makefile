# makefile for Darwin & Linux
UNAME := $(shell uname)
ifeq ($(UNAME), Darwin)
    c = clang++ -std=c++11
else ifeq ($(UNAME), Darwin)
    c = g++ -std=c++11
else
    c = g++-4.7 -std=c++11
endif

all: dataserver client

reqchannel.o: reqchannel.h reqchannel.cpp
	$(c) -c -g reqchannel.cpp

dataserver: dataserver.cpp reqchannel.o 
	$(c) -o dataserver dataserver.cpp reqchannel.o -lpthread

client: client.cpp reqchannel.o
	$(c) -o client client.cpp reqchannel.o
