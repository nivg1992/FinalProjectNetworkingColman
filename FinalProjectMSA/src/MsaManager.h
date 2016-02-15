/*
 * MsaManager.h
 *
 *  Created on: Jan 8, 2016
 *      Author: user
 */
#include <string>
#include "sharedlib/Room.h"
#include "sharedlib/User.h"
#include "sharedlib/Session.h"

using namespace std;

#ifndef MSAMANAGER_H_
#define MSAMANAGER_H_


class MsaManager {
	typedef map<string, User*> UserMap;
	typedef map<string, Room*> RoomMap;

public:
	MsaManager();
	virtual ~MsaManager();

	void printAllConnetedUsers();
	void printAllUsers();
	void printAllSessions();
	void printAllRooms();
	void printUsersByRoom(string roomName);
	void close();

	// read and write socket
	static int readCommandFromPeer(TCPSocket* peer);
	static string readDataFromPeer(TCPSocket* peer);
	static void sendCommandToPeer(TCPSocket* peer, int command);
	static void sendDataToPeer(TCPSocket* peer, string msg);

	UserMap addressToUser;
	UserMap usernameToUser;
	RoomMap roomNameToRoom;
	vector<Session*> arrSessions;
};

#endif /* MSAMANAGER_H_ */
