/*
 * MsaTcpDispatcher.h
 *
 *  Created on: Jan 8, 2016
 *      Author: user
 */

#include "MThread.h"
#include <map>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include "MultipleTCPSocketsListener.h"
#include "TCPSocket.h"
#include "MsaProtocol.h"
#include <string>
#include "sharedlib/User.h"
#include "sharedlib/Room.h"

#include "MultipleTCPSocketsListener.h"


#ifndef MSATCPDISPATCHER_H_
#define MSATCPDISPATCHER_H_

class MsaTcpDispatcher  : public MThread{
	typedef map<string, User*> UserMap;
	typedef map<string, Room*> RoomMap;

private:
	MultipleTCPSocketsListener* mtsl = new MultipleTCPSocketsListener();
	void processReadyPeer(TCPSocket* peer);

public:
	MsaTcpDispatcher();
	virtual ~MsaTcpDispatcher();
	void addPeer(TCPSocket* peer);
	TCPSocket* selectSocketReceive();
	void run();
	void login(TCPSocket* uUser);
	bool registerUser(TCPSocket* tmpPeer);
	User* GetUser(TCPSocket* peer);
	vector<string> getUsersFromFile();
	void connect(User* tmpPeer);

	static int readCommandFromPeer(TCPSocket* peer);
	static string readDataFromPeer(TCPSocket* peer);
	static void sendCommandToPeer(TCPSocket* peer, int command);
	static void sendDataToPeer(TCPSocket* peer, string msg);

	UserMap addressToUser;
	UserMap usernameToUser;
	RoomMap roomNameToRoom;
	vector<Session*> arrSessions;
};

#endif /* MSATCPDISPATCHER_H_ */
