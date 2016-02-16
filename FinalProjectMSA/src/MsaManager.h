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
#include "MsaProtocol.h"

using namespace std;


class MsaManager {
	typedef map<string, User*> UserMap;
	typedef map<string, Room*> RoomMap;

public:
	MsaTcpServer server;
	MsaTcpDispatcher* dispatcher;

	MsaManager();
	virtual ~MsaManager();

	void printAllConnetedUsers();
	void printAllUsers();
	void printAllSessions();
	void printAllRooms();
	void printUsersByRoom(string roomName);
	void close();

	UserMap addressToUser;
	UserMap usernameToUser;
	RoomMap roomNameToRoom;
	vector<Session*> arrSessions;
};

#endif /* MSAMANAGER_H_ */
