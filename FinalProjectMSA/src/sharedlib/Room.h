/*
 * Room.h
 *
 *  Created on: Jan 8, 2016
 *      Author: user
 */
#include <string.h>
#include <vector>
#include <string>

using namespace std;

#ifndef ROOM_H_
#define ROOM_H_

class Room {
	string roomName;

public:
	Room(string roomName);
	virtual ~Room();
	vector<User*> arrUsers;
	User* roomOwner;
	void AddUserToRoom(User* uUser);
	void RemoveUserFromRoom(User* uUser);
	int countUsersInRoot();
	void UpdateRoomUsers(User* updateUser,int Command);
};

#endif /* ROOM_H_ */
