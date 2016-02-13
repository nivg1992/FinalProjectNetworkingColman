/*
 * RecieveMsgUDP.h
 *
 *  Created on: Jun 5, 2013
 *      Author: user
 */

#ifndef UDPHANDELER_H_
#define UDPHANDELER_H_
#include "UDPSocket.h"
#include "MThread.h"
#include <string.h>
#include <vector>
#include <stdlib.h>

class UDPHandeler:public MThread {

public:
	UDPSocket* clientUDPSock;
	string myUserName;
	string destIp;
	string destSERVER_PORT;
	string roomName;
	vector<string> Roomusers;
	bool UDPserverConnected;

	void sendToPeer(string msg);
	UDPHandeler(string myUserName,string myIpandSERVER_PORT);
	void setDestmessage(string dest);
	void sendToRoom(string msg);
	void PrintUsersInRoom();
	virtual ~UDPHandeler();
	void run();

};

#endif /* UDPHANDELER_H */
