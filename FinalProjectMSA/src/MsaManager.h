/*
 * MsaManager.h
 *
 *  Created on: Jan 8, 2016
 *      Author: user
 */

#ifndef MSAMANAGER_H_
#define MSAMANAGER_H_

#include <string>
#include <map>
#include <vector>

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sharedlib/User.h"
#include "sharedlib/Room.h"
#include "sharedlib/Session.h"

#include "MsaTcpServer.h"
#include "MsaProtocol.h"
#include "MsaTcpDispatcher.h"

class MsaTcpDispatcher;
class MsaTcpServer;

using namespace std;

class MsaManager {
	typedef map<string, User*> UserMap;
	typedef map<string, Room*> RoomMap;

public:
	MsaTcpServer* server;
	MsaTcpDispatcher* dispatcher;

	MsaManager();
	virtual ~MsaManager();

	void printAllConnetedUsers();
	void printAllUsers();
	void printAllSessions();
	void printAllRooms();
	void printUsersByRoom(string roomName);
	void close();
	vector<TCPSocket*> getPeersVector();

	UserMap addressToUser;
	UserMap usernameToUser;
	RoomMap roomNameToRoom;
	vector<Session*> arrSessions;
};

#endif /* MSAMANAGER_H_ */
