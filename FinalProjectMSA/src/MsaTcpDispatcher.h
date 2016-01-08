/*
 * MsaTcpDispatcher.h
 *
 *  Created on: Jan 8, 2016
 *      Author: user
 */

#include "MThread.h"
#include <map>
#include "MultipleTCPSocketsListener.h"
#include "TCPSocket.h"
#include <string>
#include "sharedlib/User.h"
#include "sharedlib/Room.h"

#include "MultipleTCPSocketsListener.h"


#ifndef MSATCPDISPATCHER_H_
#define MSATCPDISPATCHER_H_

class MsaTcpDispatcher  : public MThread{
	typedef map<string, User*> UserMap;
	typedef map<string, Room*> RoomMap;

private:
	MultipleTCPSocketsListener* mtsl = new MultipleTCPSocketsListener();
	void processReadyPeer(TCPSocket* peer);

public:
	MsaTcpDispatcher();
	virtual ~MsaTcpDispatcher();
	void addPeer(TCPSocket* peer);
	TCPSocket* selectSocketReceive();
	void run();

	UserMap addressToUser;
	UserMap usernameToUser;
	RoomMap roomNameToRoom;
};

#endif /* MSATCPDISPATCHER_H_ */
