/*
 * MsaTcpDispatcher.cpp
 *
 *  Created on: Jan 8, 2016
 *      Author: user
 */

#include "MsaTcpDispatcher.h"

MsaTcpDispatcher::MsaTcpDispatcher() {
}

MsaTcpDispatcher::~MsaTcpDispatcher() {
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
	int command = readCommandFromPeer(peer);
	User* currUser = GetUser(peer);

	switch (command) {
		case LOGIN_REQUEST:
			login(peer);
			break;
		case REGISTER_USER:
			registerUser(peer);
			break;
		case OPEN_SESSION:
			if(currUser != NULL){

			}
			break;
		case CLOSE_SESSION:

			break;
		case JOIN_ROOM:
			if(currUser != NULL){

			}
			break;
		case CREATE_ROOM:
			if(currUser != NULL){

			}
			break;
		case LEAVE_ROOM:

			break;
		case GET_CONNECTED_USERS:
			if(currUser != NULL){

			}
			break;
		case GET_ROOM_USERS:
			if(currUser != NULL){

			}
			break;
		case GET_ROOMS:
			if(currUser != NULL){

			}
			break;
		case GET_ALL_USERS:
			if(currUser != NULL){

			}
			break;
		case DISCONNECT:

			break;
		default:
			break;
	}
}

vector<string> MsaTcpDispatcher::getUsersFromFile(){

		ifstream uin;
		uin.open("users.txt");
		vector<string> userNameBuffer;
		string temp;

		if (uin.is_open())
		{
			while (uin >> temp)
			{
				userNameBuffer.push_back(temp);

			}
		}

		uin.close();
		return userNameBuffer;

	}

bool MsaTcpDispatcher::registerUser(TCPSocket* tmpPeer)
{

	char* buffer = readDataFromPeer(tmpPeer);

	// get user and password
	string userName = strtok(buffer," ");
	string password = strtok(NULL, " ");
	User* newUser = new User(userName,password,tmpPeer);
	vector<string> usersVec = getUsersFromFile();

	// check is user already exists
	for(unsigned int i=0;i<usersVec.size();i++)
	{
			string currUser=usersVec.at(i);
			// parse user name from file
			string currPass=currUser.substr(currUser.find("#")+1, currUser.length());
			currUser=currUser.substr(0, currUser.find("#"));

			// validate user and password
			if (userName==currUser && password==currPass)
			{
				sendCommandToPeer(newUser->socket,REGISTRATION_FAILED);
				return false;
			}
	}

	// open file for writing
	ofstream pin2;
	pin2.open("users.txt",ofstream::app);

	// add user to users file
	pin2<<userName<<"#"<<password<<endl;
	pin2.close();


	//free(tmpUserAndPass);
	connect(newUser);
	sendCommandToPeer(newUser->socket,REGISTRATION_SUCCESSFUL);
	cout<<userName<<" has been registerd"<<endl;

	return true;

}

void MsaTcpDispatcher::login(TCPSocket* tmpPeer)
{
	//get user name and password
	cout<<"in login"<<endl;
	char* buffer = readDataFromPeer(tmpPeer);
	string userName = strtok(buffer," ");
	string password = strtok(NULL," ");

	bool auth=false;

	vector<string> usersVec = getUsersFromFile();

	for(unsigned int i=0;i<usersVec.size();i++)
	{

		string currUser=usersVec.at(i);

		// parse user name from file
		string currPass=currUser.substr(currUser.find("#")+1, currUser.length());
		currUser=currUser.substr(0, currUser.find("#"));

		// validate user and password
		if (userName==currUser && password==currPass)
		{
			User* LoginUser = new User(userName,password,tmpPeer);

			connect(LoginUser);

			return;
		}

	}

	sendCommandToPeer(tmpPeer, LOGIN_ERROR);
}

void MsaTcpDispatcher::connect(User* tmpUser)
{
	if(GetUser(tmpUser->socket) == NULL)
	{
		cout<<"user "<< tmpUser->username <<" connected successfully to server"<<endl;
		tmpUser->isLoggedIn = true;
		addressToUser.insert(pair<string,User*>(tmpUser->socket->destIpAndPort(),tmpUser));
		usernameToUser.insert(pair<string,User*>(tmpUser->username,tmpUser));
		sendCommandToPeer(tmpUser->socket,LOGIN_ACCEPTED);
		sendDataToPeer(tmpUser->socket,tmpUser->socket->destIpAndPort());
	}
	else
	{
		sendCommandToPeer(tmpUser->socket,LOGIN_ERROR);
	}
}

User* MsaTcpDispatcher::GetUser(TCPSocket* peer) {
	if(this->addressToUser.find(peer->destIpAndPort()) != this->addressToUser.end()) {
		User* currUser = this->addressToUser[peer->destIpAndPort()];
		if(currUser->isLoggedIn)
		{
			return currUser;
		} else {
			return NULL;
		}
	} else {
		return NULL;
	}
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
}

int MsaTcpDispatcher::readCommandFromPeer(TCPSocket* peer){

	int buffer;
	peer->recv((char*)&buffer,4);

	int Command = ntohl(buffer);
	return Command;
}

string MsaTcpDispatcher::readDataFromPeer(TCPSocket* peer){
	int buffer;
	//memset((void*)buffer,0,4);
	peer->recv((char*)&buffer,4);

	int DataLen = ntohl(buffer);

	char bufferData[DataLen];
	memset((void*)bufferData,0,DataLen);
	peer->recv(bufferData,DataLen);
	return string(bufferData);
}

void MsaTcpDispatcher::sendCommandToPeer(TCPSocket* peer, int command){
	char buffer[4];
    for (int i = 0; i < 4; i++)
    	buffer[3 - i] = (command >> (i * 8));

	peer->send(buffer,4);
}

void MsaTcpDispatcher::sendDataToPeer(TCPSocket* peer, string msg){
	char bufferMsgLen[4];
    for (int i = 0; i < 4; i++)
    	bufferMsgLen[3 - i] = (msg.length() >> (i * 8));

	peer->send(bufferMsgLen,4);
	peer->send(msg.data(), msg.length());
}
