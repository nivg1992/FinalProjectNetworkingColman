/*
 * Room.h
 *
 *  Created on: Jan 8, 2016
 *      Author: user
 */
#ifndef ROOM_H_
#define ROOM_H_

#include <vector>
#include <string>
#include "../MsaUtility.h"
#include "../MsaProtocol.h"


#include "User.h"

class Room {
private:


public:
	Room(string roomName,User* iroomOwner);
	virtual ~Room();

	string roomName;
	vector<User*> arrUsers;
	User* roomOwner;
	void AddUserToRoom(User* uUser);
	void RemoveUserFromRoom(User* uUser);
	int countUsersInRoom();
	string userToString();
	void UpdateRoomUsers(User* updateUser,int Command);
};

#endif /* ROOM_H_ */
