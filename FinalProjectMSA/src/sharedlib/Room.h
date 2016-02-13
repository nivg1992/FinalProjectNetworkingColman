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
	void AddUserToRoom(User* uUser);
	void RemoveUserFromRoom(User* uUser);
	int countUsersInRoot();
};

#endif /* ROOM_H_ */
