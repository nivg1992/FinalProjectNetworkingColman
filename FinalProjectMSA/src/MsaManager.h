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
public:
	MsaManager();
	virtual ~MsaManager();

	void printAllConnetedUsers();
	void printAllUsers();
	void printAllSessions();
	void printAllRooms();
	void printUsersByRoom(string roomName);
	void close();
};

#endif /* MSAMANAGER_H_ */
