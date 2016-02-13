
#include "TCPMessengerClient.h"
#include "TCPMessengerProtocol.h"
#include "UDPSocket.h"
#include <string.h>




TCPMessengerClient::TCPMessengerClient(){//constractor
	this->roomName="none";
	this->username="none";
	this->clientStatus=NOT_CONNECTED;
}
bool TCPMessengerClient::connect(string ip)//connect to server
{

	if(this->clientStatus==NOT_CONNECTED)
		{
		    clientSocket = new TCPSocket(ip,SERVER_PORT);
			this->start();
			connected=true;
			clientStatus = CONNECTED;
			return true;
		}
	else
		{
			puts("Connection was open");
			return false;
		}
}
bool TCPMessengerClient::login(string user,string password)//login to server
{

			string msg=user+" "+password;
			if(connected==true)
			{
			this->sendString2Sock(msg,LOGIN_REQUEST);
			username=user;
			return true;
			}
			else {
				puts("Failed - please connect to server first");
				return false;
			}
}

bool TCPMessengerClient::registertion(string user,string password)//register new user
{

	if(this->clientStatus==NOT_CONNECTED)
		{
		puts("Failed - please connect to server first");
		return false;
		}
	else
		{
		string commandToServer=user+" "+password;
		connected=true;
		username=user;
		this->sendString2Sock(commandToServer,REGISTER_USER);
		return true;
		}
}

bool TCPMessengerClient::isConnected(){//checks if connected
		return connected;
}

bool TCPMessengerClient::disconnect(){//disconnect from server
if(clientStatus!=NOT_CONNECTED)
{
	if(clientStatus==IN_ROOM)
	{
		this->exitRoom();
	}
	if(clientStatus==IN_SESSION)
	{
		this->closeSession();
	}
	this->sentInt2Sock(DISCONNECT);
	system("sleep 1");
	connected=false;
	this->UDPhandeler->UDPserverConnected=false;
	clientSocket->cclose();
	this->clientStatus=NOT_CONNECTED;
}
else
{
	puts("Failed - please connect to server first");
}
}

bool TCPMessengerClient::openSession(string username){

	if(isConnected())
	{
	  if(clientStatus == FREE)
	  {
		  this->sendString2Sock(username,OPEN_SESSION);
		  return true;
	  }
	  else
	  {
		  puts("You have an open session - close it and try again");
		  return false;
	  }

	}
	else
	{
		puts("Failed - please connect to server first");
		return false;
	}
}
bool TCPMessengerClient::joinRoom(string roomName){

	if(isConnected())
	{
	  if(clientStatus == FREE)
	  {
		  this->sendString2Sock(roomName,JOIN_ROOM);
		  return true;
	  }
	  else
	  {
		  puts("You are in a room - close it and try again");
		  return false;
	  }

	}
	else
	{
		puts("Failed - please connect to server first");
		return false;
	}
}

void TCPMessengerClient::run()//handles server commands
{
	char* buffer =new char[2048];
	while(connected)
	{
		int serverCommand;
		clientSocket->recv((char*)&serverCommand,4);
		bzero(buffer,1024);
		serverCommand = ntohl(serverCommand);

		switch(serverCommand)
		{
			case SESSION_ACCEPTED://new session accepted
								{
									int msgSize;
									clientSocket->recv((char*)&msgSize,4);
									msgSize = ntohl(msgSize);
									clientSocket->recv(buffer, msgSize);
									cout<<"session with "<<buffer<<endl;
									string socketWith = strtok(buffer, " ");
									string socketAddr = strtok(NULL, " ");
									UDPhandeler->setDestmessage(socketAddr);
									userSession =socketWith;
									socketConnected= true;
									clientStatus=IN_SESSION;
									break;
								}
			case SESSION_DENIED://session request denied
								{
									puts("failed to open session");
									break;
								}
			case OPEN_SESSION://new session
								{

									this->sentInt2Sock(clientStatus);
									break;
								}
			case LOGIN_ERROR://login denied
									{
										puts("Login error");
										username="NoUser";
										break;
									}
			case LOGIN_ACCEPTED://login accepted
									{
										int msgSize;
										clientSocket->recv((char*)&msgSize,4);
										msgSize = ntohl(msgSize);
										clientSocket->recv(buffer, msgSize);
										UDPhandeler = new UDPHandeler(username,buffer);
										UDPhandeler->start();
										clientStatus = FREE;
										cout<<"You are connected as "<<username<<endl;
										break;
									}

		case CLOSE_SESSION://close session
									{
										puts("Session was disconnected");
										clientStatus = FREE;
										socketConnected= false;
										userSession="none";
										break;
									}
			case ROOM_EXISTS://roomName already in use
									{
										this->roomName="none";
										puts("Room name is in use - please choose a new name");
										break;
									}
			case ROOM_CREATED://create room accepted
									{
										this->clientStatus=IN_ROOM;
										puts("Room is created");
										break;
									}
			case JOIN_ROOM_SUCCESSFUL://entering a room accepted
									{
										int msgSize;
										clientSocket->recv((char*)&msgSize,4);
										msgSize = ntohl(msgSize);
										clientSocket->recv(buffer, msgSize);
										cout<<"Welcome to room: "<<buffer<<endl;
										this->clientStatus=IN_ROOM;
										this->roomName = buffer;
										break;
									}
			case NO_SUCH_ROOM://roomName not found
									{
										puts("No room with that name was found");
										break;
									}
			case ROOM_UPDATED://refreshing room users list
									{
										int msgSize;
										clientSocket->recv((char*)&msgSize,4);
										msgSize = ntohl(msgSize);
										clientSocket->recv(buffer, msgSize);
										cout<<buffer<<endl;
										int usersSize;
										clientSocket->recv((char*)&usersSize,4);
										usersSize = ntohl(usersSize);
										bzero(buffer,1024);
										int msgLen;
										clientSocket->recv((char*)&msgLen,4);
										msgLen = ntohl(msgLen);
									    clientSocket->recv(buffer, msgLen);
									    this->UDPhandeler->Roomusers.clear();
									    string tmpUser;
									    tmpUser = strtok(buffer," ");
									    for(int i =0; i<usersSize-1;i++){
									    	this->UDPhandeler->Roomusers.push_back(tmpUser);
									    	tmpUser=strtok(NULL," ");
									    }
									    this->UDPhandeler->Roomusers.push_back(tmpUser);
										break;
									}
			case NO_ROOMS://no open rooms
									{
										cout<<"No open rooms yet"<<endl;
										break;
									}
			case EXIT_ROOM://exit a room
									{
										puts("You have left the room");
										break;
									}
			case PRINT_SERVER_DATA://print
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
										puts("New user created");
										break;
									}
			case REGISTRATION_FAILED://registration denied
									{
										puts(" failed - User name already exist");
										break;
									}
			case SERVER_CLOSED://server down
									{
										puts("server DOWN!!!! you are now offline!!");
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

void TCPMessengerClient::print(string data, int NI){//print

	char* tmp = strdup(data.c_str());
	int i;
    string text = strtok(tmp," ");
	for(i =0; i<NI-1;i++){
		cout<<text<<endl;
		text=strtok(NULL," ");
	}
	cout<<text<<endl;
	free(tmp);
}


void TCPMessengerClient::listRooms(){

		this->sentInt2Sock(GET_ROOMS);
}

void TCPMessengerClient::printUsersInRoom(string roomName){
	if(this->clientStatus == NOT_CONNECTED or this->clientStatus == CONNECTED)
			cout<<"You must be logged in"<<endl;
	else
		this->sendString2Sock(roomName,GET_ROOM_USERS);


}

bool TCPMessengerClient::closeSession(){
	if(isConnected())
		{
		if(this->clientStatus==IN_SESSION)
		{
		this->sentInt2Sock(CLOSE_SESSION);
		clientStatus= FREE;
		socketConnected= false;
		userSession="none";
		puts("you have been disconnected from your current session");
		return true;
		}
		else
		{
			puts("you're not in a session");
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

void TCPMessengerClient::sentInt2Sock(int protocol)
{
	int command = htonl(protocol);
	clientSocket->send((char*)&command,4);

}
void TCPMessengerClient::sendString2Sock(string msg,int protocol){
	if(protocol!=DONT_SEND_COMMAND)
	   this->sentInt2Sock(protocol);

	int msglen=htonl(msg.length());
	clientSocket->send((char*)&msglen,4);
	clientSocket->send(msg.c_str(),(msg.length()));
}

void TCPMessengerClient::createRoom(string roomName)
{

	this->sendString2Sock(roomName,CREATE_ROOM);
	this->roomName= roomName;

}
void TCPMessengerClient::exitRoom()
{
	if(clientStatus==IN_ROOM)
	{
		this->sendString2Sock(this->roomName,LEAVE_ROOM);
		this->clientStatus=FREE;
		this->roomName="none";
	}
	else
		puts("your not in a room");
}

void  TCPMessengerClient::printStatus(){
	if(this->clientStatus==FREE){
		cout<<"connected to server as: "<<this->username<<endl;
	}
	else if(this->clientStatus==IN_ROOM){
		cout<<"you are in room "<<this->roomName<<endl;
	}
	else if(this->clientStatus==IN_SESSION){
		cout<<"you are in session with: "<<userSession<<endl;
	}
	else
		cout<<"you are not logged in"<<endl;
}


void TCPMessengerClient::listConnectedUsers()
{
	if(this->clientStatus == NOT_CONNECTED or this->clientStatus == CONNECTED) //print all connected users ONLY when connected
		cout<<"You are not logged in"<<endl;
	else
		this->sentInt2Sock(GET_CONNECTED_USERS);

}

void TCPMessengerClient::listRegisteredUsers(){
	if(this->clientStatus == NOT_CONNECTED or this->clientStatus == CONNECTED) //print all users ONLY when connected
		cout<<"You are not logged in"<<endl;
	else
	{
		this->sentInt2Sock(GET_ALL_USERS);
	}
}
