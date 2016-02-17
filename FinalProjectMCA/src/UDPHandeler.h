/*
 * UDPHandeler.h
 *
 *  Created on: Feb 8, 2016
 *      Author: Michael Blitshtein
 */

#ifndef UDPHANDELER_H_
#define UDPHANDELER_H_
#include "UDPSocket.h"
#include "MThread.h"
#include <string.h>
#include <vector>
#include <stdlib.h>

class UDPSocket;

class UDPHandeler:public MThread {

public:
	UDPSocket* clientUDPSock;
	string userName;
	string destinationIp;
	string destinationServerPort;
	string roomName;
	vector<string> RoomUsers;
	bool IsUdpServerConnected;

	void sendToPeer(string msg);
	UDPHandeler(string myUserName,string myIpandSERVER_PORT);
	void setDestinationMessage(string dest);
	void sendToRoom(string msg);
	void PrintUsersInRoom();
	virtual ~UDPHandeler();
	void run();

};

#endif /* UDPHANDELER_H */
