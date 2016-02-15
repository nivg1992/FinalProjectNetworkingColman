/*
 * UDPHandeler.cpp
 *
 *  Created on: Feb 8, 2016
 *      Author: Michael Blitshtein
 */

#include "UDPHandeler.h"


UDPHandeler::UDPHandeler(string userName,string myIpandSERVER_PORT) {

	this->userName = userName;
	char* S_PORT = strdup(myIpandSERVER_PORT.c_str());
	string tempSERVER_PORT = strtok(S_PORT,":");
	tempSERVER_PORT = strtok(NULL,":");
	clientUDPSock = new UDPSocket(atoi(tempSERVER_PORT.c_str()));
	IsUdpServerConnected=true;

}
void UDPHandeler::run(){

	char buffer [100];

	while(IsUdpServerConnected)
		{
		  clientUDPSock->recv(buffer,sizeof(buffer));
		  //print the message
		  cout<<buffer<<endl;
		  bzero((char *) &buffer, sizeof(buffer));
		}
	clientUDPSock->cclose();
}

void UDPHandeler::sendToRoom(string msg)
{
	for(unsigned int i=0;i<this->RoomUsers.size();i++)
	{
		string tempdest = RoomUsers.at(i);
		this->setDestinationMessage(tempdest);
		this->sendToPeer(msg);
	}
}
void UDPHandeler::setDestinationMessage(string dest)
{
		char* SERVER_PORT = strdup(dest.c_str());
		destinationIp = strtok(SERVER_PORT,":");
		destinationServerPort = strtok(NULL,":");
}

UDPHandeler::~UDPHandeler() {
}


void UDPHandeler::sendToPeer(string message)
{
	 string finalmessage = "["+userName+"]"+" " + message;
	 clientUDPSock->sendTo(finalmessage, destinationIp, atoi(destinationServerPort.c_str()));
}
