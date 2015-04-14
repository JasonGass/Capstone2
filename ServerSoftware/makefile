all:
	g++ ./Packet/Packet.h ./Sender/Sender.o ./Connection/ConnectionListener.o ./Receiver/Receiver.o Main.cpp -std=c++0x -pthread -o server
	g++ ./Client/Client.cpp -pthread -std=c++0x -o client
