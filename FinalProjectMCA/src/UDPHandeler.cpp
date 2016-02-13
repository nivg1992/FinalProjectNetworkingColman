/*
 * UDPHandeler.cpp
 *
 *  Created on: Feb 8, 2016
 *      Author: Michael Blitshtein
 */

#include "UDPHandeler.h"


void UDPHandeler::sendToRoom(string msg)
{
	for(unsigned int i=0;i<this->Roomusers.size();i++)
	{
		string tempdest = Roomusers.at(i);
		this->setDestinationMessage(tempdest);
		this->sendToPeer(msg);
	}
}

UDPHandeler::UDPHandeler(string myUserName,string myIpandSERVER_PORT) {

	this->myUserName = myUserName;
	char* SERVER_PORT = strdup(myIpandSERVER_PORT.c_str());
	string tempSERVER_PORT = strtok(SERVER_PORT,":");
	tempSERVER_PORT = strtok(NULL,":");
	clientUDPSock = new UDPSocket(atoi(tempSERVER_PORT.c_str()));
	UDPserverConnected=true;

}
void UDPHandeler::setDestinationMessage(string dest)
{
		char* SERVER_PORT = strdup(dest.c_str());
		destIp = strtok(SERVER_PORT,":");
		destSERVER_PORT = strtok(NULL,":");
}
void UDPHandeler::sendToPeer(string msg)
{
	 string finalmsg = "["+myUserName+"]"+" " + msg;
	 clientUDPSock->sendTo(finalmsg,destIp,atoi(destSERVER_PORT.c_str()));
}
UDPHandeler::~UDPHandeler() {
}

void UDPHandeler::run(){

	char buffer [100];

	while(UDPserverConnected)
		{
		  clientUDPSock->recv(buffer,sizeof(buffer));
	    //print the msg
		  cout<<buffer<<endl;
		  bzero((char *) &buffer, sizeof(buffer));
		}
	clientUDPSock->cclose();
}
