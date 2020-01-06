CFLAGS:=-c -Wall -Weffc++ -g -std=c++11 -Iinclude
LDFLAGS:=-lboost_system

all: StompBookClubClient
	g++ -pthread -o bin/StompBookClubClient bin/connectionHandler.o bin/client.o bin/STOMPMessage.o bin/StompBookClubClient.o $(LDFLAGS) 

StompBookClubClient: bin/connectionHandler.o bin/client.o bin/STOMPMessage.o bin/StompBookClubClient.o
	
bin/connectionHandler.o: src/connectionHandler.cpp
	g++ -pthread $(CFLAGS) -o bin/connectionHandler.o src/connectionHandler.cpp

bin/client.o: src/client.cpp
	g++ -pthread $(CFLAGS) -o bin/client.o src/client.cpp

bin/STOMPMessage.o: src/STOMPMessage.cpp
	g++ -pthread $(CFLAGS) -o bin/STOMPMessage.o src/STOMPMessage.cpp

bin/StompBookClubClient.o: src/StompBookClubClient.cpp
	g++ -pthread $(CFLAGS) -o bin/StompBookClubClient.o src/StompBookClubClient.cpp
	
.PHONY: clean
clean:
	rm -f bin/*
