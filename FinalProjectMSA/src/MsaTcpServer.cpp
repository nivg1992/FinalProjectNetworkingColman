/*
 * MsaTcpServer.cpp
 *
 *  Created on: Jan 8, 2016
 *      Author: user
 */

#include "MsaTcpServer.h"

MsaTcpServer::MsaTcpServer(int port, MsaTcpDispatcher* dispatcher) {
	_serverSock = new TCPSocket(port);
	int i  = _serverSock->getSocketFid();
	_dispatcher = dispatcher;


}

MsaTcpServer::~MsaTcpServer() {
	_serverSock->cclose();
}

void MsaTcpServer::run()
{
	_dispatcher->start();

	while(true)
	{
		TCPSocket* newPeer = _serverSock->listenAndAccept();
		if (newPeer!= NULL)
		{
			cout<<"Add Peer"<<newPeer->destIpAndPort()<<endl;
			_dispatcher->addPeer(newPeer);
		}

	}
}
