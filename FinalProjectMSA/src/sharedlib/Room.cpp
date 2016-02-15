/*
 * Room.cpp
 *
 *  Created on: Jan 8, 2016
 *      Author: user
 */

#include "Room.h"

Room::Room(string roomName) {
	this->roomName=roomName;
}

Room::~Room() {

}

void Room::AddUserToRoom(User* uUser) {
	arrUsers.push_back(uUser);
}

void Room::RemoveUserFromRoom(User* uUser) {
	for(unsigned int i = 0 ; i<arrUsers.size();i++){
			if(arrUsers.at(i)->username == uUser->username)
			{
				arrUsers.erase(arrUsers.begin()+i);
			}
		}
}

int Room::countUsersInRoot(){
	return arrUsers.size();
}

void Room::UpdateRoomUsers(User* updateUser,int Command) {

}
