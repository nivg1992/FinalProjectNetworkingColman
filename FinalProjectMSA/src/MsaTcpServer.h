/*
 * MsaTcpServer.h
 *
 *  Created on: Jan 8, 2016
 *      Author: user
 */
#include "MThread.h"
#include "TCPSocket.h"
#include "MsaTcpDispatcher.h"

#ifndef MSATCPSERVER_H_
#define MSATCPSERVER_H_

class MsaTcpServer  :  public MThread {
private:
	TCPSocket* _serverSock;
	MsaTcpDispatcher* _dispatcher;

public:
	MsaTcpServer(int port, MsaTcpDispatcher* dispatcher);
	virtual ~MsaTcpServer();
	void run();

};

#endif /* MSATCPSERVER_H_ */
