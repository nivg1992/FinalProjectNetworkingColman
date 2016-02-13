/*
 * UDPSocket.cpp
 *
 *  Created on: Feb 8, 2016
 *      Author: Michael Blitshtein
 */

#include "UDPSocket.h"
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>

UDPSocket::UDPSocket(int PORT)
{
	socket_fd = socket(AF_INET, SOCK_DGRAM, 0);

	// clear s_in struct
	bzero((char *) &s_in, sizeof(s_in));

	//sets sin address
	s_in.sin_family = (short)AF_INET;

	//WILDCARD
	s_in.sin_addr.s_addr = htonl(INADDR_ANY);
	s_in.sin_port = htons((u_short)PORT);

	fsize = sizeof(from);

	//bind socket on specified address
	if(bind(socket_fd, (struct sockaddr *)&s_in, sizeof(s_in))<0){
		perror ("Error naming channel");
	}
}
void UDPSocket::cclose()
{
	cout<<"closing socket"<<endl;
	shutdown(socket_fd,SHUT_RDWR);
	close(socket_fd);
}

int UDPSocket::sendTo(string msg, string ip, int PORT){
	struct sockaddr_in toAddr;
	bzero((char *) &toAddr, sizeof(toAddr));
	toAddr.sin_family = AF_INET;
	toAddr.sin_addr.s_addr = inet_addr(ip.data());
	toAddr.sin_port = htons(PORT);
	return sendto(socket_fd,msg.data(),msg.length(),0,(struct sockaddr *)&toAddr,sizeof(toAddr));
}
int UDPSocket::recv(char* buffer, int length){
	//ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags,
	//					struct sockaddr *src_addr, socklen_t *addrlen);
	return recvfrom(socket_fd,buffer,length,0,(struct sockaddr *)&from,&fsize);
}

string UDPSocket::fromAddr()
{
	return inet_ntoa(from.sin_addr);
}

int UDPSocket::reply(string msg)
{
	return sendto(socket_fd,msg.data(),msg.length(),0,(struct sockaddr *)&from,sizeof(from));
}
