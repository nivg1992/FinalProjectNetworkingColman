/*
 * MsaTcpDispatcher.h
 *
 *  Created on: Jan 8, 2016
 *      Author: user
 */

#ifndef MSATCPDISPATCHER_H_
#define MSATCPDISPATCHER_H_

#include "MThread.h"
#include <map>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include "MultipleTCPSocketsListener.h"
#include "TCPSocket.h"

#include <stdio.h>
#include <string.h>

#include "sharedlib/User.h"
#include "sharedlib/Room.h"
#include "sharedlib/Session.h"

#include "MsaUtility.h"
#include "MsaProtocol.h"
#include "MsaManager.h"

using namespace std;

class MsaManager;

class MsaTcpDispatcher  : public MThread{
private:
	MultipleTCPSocketsListener* mtsl;
	void processReadyPeer(TCPSocket* peer);
	MsaManager* _Manager;

public:
	MsaTcpDispatcher(MsaManager* Manager);
	virtual ~MsaTcpDispatcher();

	// MThread
	void addPeer(TCPSocket* peer);
	TCPSocket* selectSocketReceive();
	void run();

	// Login and register
	void login(TCPSocket* uUser);
	bool registerUser(TCPSocket* tmpPeer);
	void connect(User* tmpPeer);

	// Session
	void openSession(User* tmpPeer);
	void closeSession(User* tmpPeer);

	// Room
	void createRoom(User* tmpPeer);
	void enterRoom(User* tmpPeer);
	void exitRoom(User* tmpPeer);

	vector<User*> getConnectedUsers();
	vector<User*> getAllUsers();
	vector<Room*> getAllRooms();
	vector<Session*> getAllSessions();
	vector<User*> getAllUsersInRoom(string name);
	string prepareToSendUsers(vector<User*> uUsers);
	string prepareToSendRooms(vector<Room*> rRooms);

	// Get User
	User* GetUser(TCPSocket* peer);
	vector<string> getUsersFromFile();
};

#endif /* MSATCPDISPATCHER_H_ */
