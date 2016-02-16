/*
 * Room.cpp
 *
 *  Created on: Jan 8, 2016
 *      Author: user
 */

#include "Room.h"

Room::Room(string roomName,User* iroomOwner) {
	this->roomName = roomName;
	this->roomOwner = iroomOwner;
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

int Room::countUsersInRoom(){
	return arrUsers.size();
}

string Room::userToString() {
	string userinString;

	for(unsigned int i=0;i<this->arrUsers.size();i++)
	{
		userinString.append(this->arrUsers.at(i)->socket->destIpAndPort());

		if(i!=this->arrUsers.size()-1)
			userinString.append(" ");
	}

	return userinString;
}

void Room::UpdateRoomUsers(User* updateUser,int Command) {
	for(unsigned int i=0;i < arrUsers.size();i++) {
		MsaUtility::sendCommandToPeer(arrUsers.at(i)->socket,ROOM_UPDATED);

		string tempMsg = updateUser->username;
		tempMsg.append(" Has Joined the room");

		switch(Command)
		{
			case JOIN_ROOM:
				{

					string tempMsg = updateUser->username;
					tempMsg.append(" Has Joined the room");

					break;
				}
			case LEAVE_ROOM:
				{
					string tempMsg = updateUser->username;
					tempMsg.append(" Has Left the room");
					break;
				}
		}

		MsaUtility::sendDataToPeer(arrUsers.at(i)->socket, tempMsg);

		int numOfUsers = countUsersInRoom();

		MsaUtility::sendCommandToPeer(arrUsers.at(i)->socket, numOfUsers);

		string usersVectorString = this->userToString();
		MsaUtility::sendDataToPeer(arrUsers.at(i)->socket, usersVectorString);
	}
}
