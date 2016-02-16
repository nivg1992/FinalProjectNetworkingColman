/*
 * MsaManager.cpp
 *
 *  Created on: Jan 8, 2016
 *      Author: user
 */

#include "MsaManager.h"
#include <cstring>

MsaManager::MsaManager() {
	dispatcher = new MsaTcpDispatcher(this);
	server = new MsaTcpServer(MSNGR_PORT, dispatcher);
}

MsaManager::~MsaManager() {

}

void MsaManager::printAllConnetedUsers() {
	vector <User>connectedUsers = dispatcher->getConnectedUsers();
	if(connectedUsers.size() == 0)
		cout<<"no peers connected"<<endl;
	else
	{
		if(connectedUsers.size() != 0)
		{
			cout<<"***********Connected users***********"<<endl;
			for(unsigned int i = 0;i<connectedUsers.size();i++)
			{
				cout<<connectedUsers[i].username<<endl;
			}
		}

	}
}
void MsaManager::printAllUsers() {
	vector <User>allUsers = dispatcher->getAllUsers();
	if(allUsers.size() == 0)
		cout<<"no users"<<endl;
	else
	{
		if(allUsers.size() != 0)
		{
			cout<<"***********Users List***********"<<endl;
			for(unsigned int i = 0;i<allUsers.size();i++)
			{
				cout<<allUsers[i].username<<endl;
			}
		}

	}
}
void MsaManager::printAllSessions() {
	vector <User>allUsers = dispatcher->getAllUsers();
	if(allUsers.size() == 0)
		cout<<"no peers connected"<<endl;
	else
	{
		if(allUsers.size() != 0)
		{
			cout<<"***********Connected users***********"<<endl;
			for(unsigned int i = 0;i<allUsers.size();i++)
			{
				cout<<allUsers[i].username<<endl;
			}
		}

	}
}
void MsaManager::printAllRooms() {
	vector <Room> rooms = dispatcher->getAllRooms();
	if(rooms.size() == 0)
	cout<<"no rooms "<<endl;
	else
	{
		if(rooms.size() != 0)
		{
			cout<<"***********Rooms***********"<<endl;
			for(unsigned int i = 0;i<rooms.size();i++)
			{
				cout<<rooms[i].roomName<<endl;
			}
		}
	}
}
void MsaManager::printUsersByRoom(string roomName) {
	vector <User> users = dispatcher->getAllUsersInRoom(roomName);
	if(users.size() == 0)
	cout<<"no users in room "<<endl;
	else
	{
		if(users.size() != 0)
		{
			cout<<"***********Users list***********"<<endl;
			for(unsigned int i = 0;i<users.size();i++)
			{
				cout<<users[i].username<<endl;
			}
		}
	}
}
void MsaManager::close() {
	delete server;
	delete dispatcher;
}


