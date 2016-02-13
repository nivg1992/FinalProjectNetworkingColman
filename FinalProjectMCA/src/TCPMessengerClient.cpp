
#include "TCPMessengerClient.h"
#include "TCPMessengerProtocol.h"
#include "UDPSocket.h"
#include <string.h>
#include <iostream>


TCPMessengerClient::TCPMessengerClient()
{
	this->roomName="";
	this->username="";
	this->clientStatus=NOT_CONNECTED;
}
bool TCPMessengerClient::connect(string ip)
{
	// Connect to server
	if(this->clientStatus==NOT_CONNECTED)
		{
		clientSocket = new TCPSocket(ip, SERVER_PORT);
		this->start();
		connected = true;
		clientStatus = CONNECTED;
		return true;
		}
	else
		{
		cout<<"Connection to the server already exists"<<endl;
		return false;
		}
}
bool TCPMessengerClient::login(string user,string password)
{
	string msg = user + " " + password;
	if(connected == true)
	{
	this->sendStringToSocket(msg, LOGIN_REQUEST);
	username = user;
	return true;
	}
	else
	{
	cout<<"Failed, need to connect to server first"<<endl;
	return false;
	}
}

bool TCPMessengerClient::registration(string user,string password)
{

	if(this->clientStatus == NOT_CONNECTED)
		{
		cout<<"Failed, need to connect to server first"<<endl;
		return false;
		}
	else
		{
			string commandToServer=user+" "+password;
			connected = true;
			username = user;
			this->sendStringToSocket(commandToServer, REGISTER_USER);
			return true;
		}
}

bool TCPMessengerClient::isConnected(){
		return connected;
}

bool TCPMessengerClient::disconnect(){
	if(clientStatus != NOT_CONNECTED)
	{
		if(clientStatus == IN_ROOM)
		{
			this->exitRoom();
		}
		if(clientStatus == IN_SESSION)
		{
			this->closeSession();
		}
		this->sentIntToSocket(DISCONNECT);
		system("sleep 1");
		connected = false;
		this->UDPhandeler->UDPserverConnected = false;
		clientSocket->cclose();
		this->clientStatus = NOT_CONNECTED;
	}
	else
	{
		cout<<"Failed, need to connect to server first"<<endl;
	}
}

bool TCPMessengerClient::openSession(string username){

	if(isConnected())
	{
	  if(clientStatus == FREE)
	  {
		  this->sendStringToSocket(username, OPEN_SESSION);
		  return true;
	  }
	  else
	  {
		  cout<<"There is an open session, close it and retry"<<endl;
		  return false;
	  }

	}
	else
	{
		cout<<"Failed, need to connect to server first"<<endl;
		return false;
	}
}
bool TCPMessengerClient::joinRoom(string roomName){

	if(isConnected())
	{
		if(clientStatus == FREE)
		{
		  this->sendStringToSocket(roomName, JOIN_ROOM);
		  return true;
		}
		else
		{
		  cout<<"You are in a room, close it and retry"<<endl;
		  return false;
		}
	}
	else
	{
	cout<<"Failed, need to connect to server first"<<endl;
	return false;
	}
}

void TCPMessengerClient::run()
{
	char* buffer =new char[2048];
	while(connected)
	{
		int serverCommand;
		clientSocket->recv((char*)&serverCommand,4);
		bzero(buffer,1024);
		// getting the command
		serverCommand = ntohl(serverCommand);

		switch(serverCommand)
		{
			case SESSION_ACCEPTED:
								{
									int msgSize;
									string socketWith, socketAddress;
									clientSocket->recv((char*)&msgSize,4);
									msgSize = ntohl(msgSize);
									clientSocket->recv(buffer, msgSize);
									cout<<"session with "<<buffer<<endl;
									socketWith = strtok(buffer, " ");
									socketAddress = strtok(NULL, " ");
									UDPhandeler->setDestmessage(socketAddress);
									userSession = socketWith;
									socketConnected = true;
									clientStatus = IN_SESSION;
									break;
								}
			case SESSION_DENIED:
								{
									cout<<"failed to open session"<<endl;
									break;
								}
			case OPEN_SESSION:
								{

									this->sentIntToSocket(clientStatus);
									break;
								}
			case LOGIN_ERROR:
									{
										cout<<"Login error"<<endl;
										username = "NoUser";
										break;
									}
			case LOGIN_ACCEPTED:
									{
										int msgSize;
										clientSocket->recv((char*)&msgSize,4);
										msgSize = ntohl(msgSize);
										clientSocket->recv(buffer, msgSize);
										UDPhandeler = new UDPHandeler(username, buffer);
										UDPhandeler->start();
										clientStatus = FREE;
										cout<<"You are connected as "<<username<<endl;
										break;
									}

		case CLOSE_SESSION:
									{
										cout<<"Session was disconnected"<<endl;
										clientStatus = FREE;
										socketConnected = false;
										userSession = "";
										break;
									}
			case ROOM_EXISTS:
									{
										this->roomName = "";
										cout<<"Room name exists - choose a new name"<<endl;
										break;
									}
			case ROOM_CREATED:
									{
										this->clientStatus = IN_ROOM;
										cout<<"Room is created"<<endl;
										break;
									}
			case JOIN_ROOM_SUCCESSFUL:
									{
										int msgSize;
										clientSocket->recv((char*)&msgSize,4);
										msgSize = ntohl(msgSize);
										clientSocket->recv(buffer, msgSize);
										cout<<"Welcome to room: "<<buffer<<endl;
										this->clientStatus = IN_ROOM;
										this->roomName = buffer;
										break;
									}
			case NO_SUCH_ROOM:
									{
										cout<<"No room found"<<endl;
										break;
									}
			case ROOM_UPDATED:
									{
										int msgSize, usersSize, msgLength;
										string tmpUser;

										clientSocket->recv((char*)&msgSize,4);
										msgSize = ntohl(msgSize);
										clientSocket->recv(buffer, msgSize);
										cout<<buffer<<endl;

										clientSocket->recv((char*)&usersSize, 4);
										usersSize = ntohl(usersSize);
										bzero(buffer, 1024);

										clientSocket->recv((char*)&msgLength, 4);
										msgLength = ntohl(msgLength);
									    clientSocket->recv(buffer, msgLength);
									    this->UDPhandeler->Roomusers.clear();

									    tmpUser = strtok(buffer," ");
									    for(int i = 0; i < usersSize - 1; i++){
									    	this->UDPhandeler->Roomusers.push_back(tmpUser);
									    	tmpUser = strtok(NULL," ");
									    }
									    this->UDPhandeler->Roomusers.push_back(tmpUser);
										break;
									}
			case NO_ROOMS:
									{
										cout<<"No open rooms yet"<<endl;
										break;
									}
			case EXIT_ROOM:
									{
										cout<<"You have left the room"<<endl;
										break;
									}
			case PRINT_SERVER_DATA:
									{
										int NI;
									    clientSocket->recv((char*)&NI,4);
									    NI = ntohl(NI);
										int msgSize;
										clientSocket->recv((char*)&msgSize,4);
										msgSize = ntohl(msgSize);
										clientSocket->recv(buffer, msgSize);
										this->print(buffer,NI);
										break;
									}
			case REGISTRATION_SUCCESSFUL://registration approved
									{
										cout<<"New user created"<<endl;
										break;
									}
			case REGISTRATION_FAILED://registration denied
									{
										cout<<" failed - User name already exist"<<endl;
										break;
									}
			case SERVER_CLOSED://server down
									{
										cout<<"server DOWN!!!! you are now offline!!"<<endl;
											if(clientStatus==IN_ROOM)
											{
												this->exitRoom();
											}
											if(clientStatus==IN_SESSION)
											{
												this->closeSession();
											}
										system("sleep 1");
										connected=false;
										this->UDPhandeler->UDPserverConnected=false;
										clientSocket->cclose();
										this->clientStatus=NOT_CONNECTED;
										break;
									}
		}
	}
}

void TCPMessengerClient::print(string data, int NI){

	char* tmp = strdup(data.c_str());
	int i;
    string text = strtok(tmp," ");
	for(i =0; i<NI-1; i++){
		cout<<text<<endl;
		text=strtok(NULL," ");
	}
	cout<<text<<endl;
	free(tmp);
}


void TCPMessengerClient::listRooms(){

		this->sentIntToSocket(GET_ROOMS);
}

void TCPMessengerClient::printUsersInRoom(string roomName){
	if(this->clientStatus == NOT_CONNECTED or this->clientStatus == CONNECTED)
			cout<<"You must be logged in"<<endl;
	else
		this->sendStringToSocket(roomName, GET_ROOM_USERS);
}

bool TCPMessengerClient::closeSession(){
	if(isConnected())
	{
		if(this->clientStatus == IN_SESSION)
		{
			this->sentIntToSocket(CLOSE_SESSION);
			cout<<"Disconnected from session"<<endl;
			clientStatus = FREE;
			socketConnected = false;
			userSession = "";
			return true;
		}
		else
		{
			cout<<"you're not in a session"<<endl;
		}
	}

	return false;
}

/**
 * send the given message to the connected peer
 */
bool TCPMessengerClient::sendMessage(string msg){

	if(this->clientStatus==IN_SESSION)
	{
		UDPhandeler->sendToPeer(msg);
		return true;
	}
	if(this->clientStatus==IN_ROOM)
	{
		UDPhandeler->sendToRoom(msg);
		return true;
	}
	return false;
}

void TCPMessengerClient::sentIntToSocket(int protocol)
{
	int command = htonl(protocol);
	clientSocket->send((char*)&command, 4);
}
void TCPMessengerClient::sendStringToSocket(string msg,int protocol){
	if(protocol != NONE)
	   this->sentIntToSocket(protocol);

	int msglen = htonl(msg.length());
	clientSocket->send((char*)&msglen, 4);
	clientSocket->send(msg.c_str(), (msg.length()));
}

void TCPMessengerClient::createRoom(string roomName)
{

	this->sendStringToSocket(roomName, CREATE_ROOM);
	this->roomName = roomName;

}
void TCPMessengerClient::exitRoom()
{
	if(clientStatus == IN_ROOM)
	{
		this->sendStringToSocket(this->roomName, LEAVE_ROOM);
		this->clientStatus = FREE;
		this->roomName = "";
	}
	else
		cout<<"you are not in a room"<<endl;
}

void  TCPMessengerClient::printStatus(){
	if(this->clientStatus == FREE){
		cout<<"Connected as - "<<this->username<<endl;
	}
	else if(this->clientStatus == IN_ROOM){
		cout<<"Entered room - "<<this->roomName<<endl;
	}
	else if(this->clientStatus == IN_SESSION){
		cout<<"In session with: "<<userSession<<endl;
	}
	else
		cout<<"You are not logged in"<<endl;
}


void TCPMessengerClient::listConnectedUsers()
{
	if(this->clientStatus == NOT_CONNECTED || this->clientStatus == CONNECTED) //print all connected users ONLY when connected
		cout<<"You are not logged in"<<endl;
	else
		this->sentIntToSocket(GET_CONNECTED_USERS);

}

void TCPMessengerClient::listRegisteredUsers(){
	if(this->clientStatus == NOT_CONNECTED || this->clientStatus == CONNECTED) //print all users ONLY when connected
		cout<<"You are not logged in"<<endl;
	else
	{
		this->sentIntToSocket(GET_ALL_USERS);
	}
}
