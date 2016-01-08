/*
 * MsaTcpDispatcher.cpp
 *
 *  Created on: Jan 8, 2016
 *      Author: user
 */

#include "MsaTcpDispatcher.h"

MsaTcpDispatcher::MsaTcpDispatcher() {
	// TODO Auto-generated constructor stub

}

MsaTcpDispatcher::~MsaTcpDispatcher() {
	// TODO Auto-generated destructor stub
}

void MsaTcpDispatcher::run()
{
	//loop
	while(true)
	{
		//listen to all peers
		TCPSocket* peer = this->selectSocketReceive();

		if (peer != NULL)
		{
			//process ready peer
			this->processReadyPeer(peer);
		}
	}
}

void MsaTcpDispatcher::processReadyPeer(TCPSocket* peer)
{
}

TCPSocket* MsaTcpDispatcher::selectSocketReceive()
{
	if (this->addressToUser.size() > 0)
	{
		return mtsl->listenToSocket(1);
	}
	return NULL;
}


void MsaTcpDispatcher::addPeer(TCPSocket* peer)
{
	User currentUser;
	currentUser.socket = peer;
	mtsl->addSocket(peer);

//	this->peers.insert(pair<string,TCPSocket*>(peer->destIpAndPort(),peer));
//	mtsl->addSocket(peer);
}
