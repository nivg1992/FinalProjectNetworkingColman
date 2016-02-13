/*
 * MsaManager.cpp
 *
 *  Created on: Jan 8, 2016
 *      Author: user
 */

#include "MsaManager.h"

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

int MsaManager::readCommandFromPeer(TCPSocket* peer){

	int buffer;
	peer->recv((char*)&buffer,4);

	int Command = ntohl(buffer);
	return Command;
}

string MsaManager::readDataFromPeer(TCPSocket* peer){
	int buffer;
	//memset((void*)buffer,0,4);
	peer->recv((char*)&buffer,4);

	int DataLen = ntohl(buffer);

	char bufferData[DataLen];
	memset((void*)bufferData,0,DataLen);
	peer->recv(bufferData,DataLen);
	return string(bufferData);
}

void MsaManager::sendCommandToPeer(TCPSocket* peer, int command){
	char buffer[4];
    for (int i = 0; i < 4; i++)
    	buffer[3 - i] = (command >> (i * 8));

	peer->send(buffer,4);
}

void MsaManager::sendDataToPeer(TCPSocket* peer, string msg){
	char bufferMsgLen[4];
    for (int i = 0; i < 4; i++)
    	bufferMsgLen[3 - i] = (msg.length() >> (i * 8));

	peer->send(bufferMsgLen,4);
	peer->send(msg.data(), msg.length());
}
