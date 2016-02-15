/*
 * MsaManager.cpp
 *
 *  Created on: Jan 8, 2016
 *      Author: user
 */

#include "MsaManager.h"

MsaManager::MsaManager() {

}

MsaManager::~MsaManager() {

}

void MsaManager::printAllConnetedUsers() {

}
void MsaManager::printAllUsers() {

}
void MsaManager::printAllSessions() {

}
void MsaManager::printAllRooms() {

}
void MsaManager::printUsersByRoom(string roomName) {

}
void MsaManager::close() {

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
