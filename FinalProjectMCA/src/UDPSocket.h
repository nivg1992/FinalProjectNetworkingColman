/*
 * UDPSocket.h
 *
 *  Created on: Feb 8, 2016
 *      Author: Michael Blitshtein
 */

#ifndef UDPSOCKET_H_
#define UDPSOCKET_H_

#include <netinet/in.h>
#include <inttypes.h>
#include <strings.h>
#include <stdio.h>
#include <iostream>

using namespace std;

class UDPSocket{
private:
	struct sockaddr_in  s_in;
	struct sockaddr_in from;
	unsigned int fsize;
	int socket_fd;

public:
	UDPSocket(int PORT = 9999);
	int recv(char* buffer, int length);
	int sendTo(string message, string ip, int PORT);
	int reply(string message);
	void cclose();
	string fromAddr();
};

#endif /* UDPSOCKET_H_ */
