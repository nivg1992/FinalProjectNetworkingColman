/*
 * MsaTcpDispatcher.cpp
 *
 *  Created on: Jan 8, 2016
 *      Author: user
 */

#include "MsaTcpDispatcher.h"
#include <cstring>
#include <string.h>
#include <stdio.h>

MsaTcpDispatcher::MsaTcpDispatcher(MsaManager* Manager) {
	this->_Manager = Manager;

	mtsl = new MultipleTCPSocketsListener();
}

MsaTcpDispatcher::~MsaTcpDispatcher() {
	delete mtsl;
}

void MsaTcpDispatcher::processReadyPeer(TCPSocket* peer)
{
	int command = MsaUtility::readCommandFromPeer(peer);
	User* currUser = GetUser(peer);

	switch (command) {
		case LOGIN_REQUEST:
			if(this->_Manager->addressToUser.find(peer->destIpAndPort()) != this->_Manager->addressToUser.end()) {
					User* currUserLogin = this->_Manager->addressToUser[peer->destIpAndPort()];
					login(currUserLogin);
			}

			break;
		case REGISTER_USER:
			if(this->_Manager->addressToUser.find(peer->destIpAndPort()) != this->_Manager->addressToUser.end()) {
					User* currUserRegister = this->_Manager->addressToUser[peer->destIpAndPort()];
					registerUser(currUserRegister);
			}

			break;
		case OPEN_SESSION:
			if(currUser != NULL){
				openSession(currUser);
			}
			break;
		case CLOSE_SESSION:
			if(currUser != NULL){
				closeSession(currUser);
			}
			break;
		case JOIN_ROOM:
			if(currUser != NULL){
				enterRoom(currUser);
			}
			break;
		case CREATE_ROOM:
			if(currUser != NULL){
				createRoom(currUser);
			}
			break;
		case LEAVE_ROOM:
			if(currUser != NULL){
				exitRoom(currUser);
			}
			break;
		case GET_CONNECTED_USERS:
			if(currUser != NULL){
				MsaUtility::sendCommandToPeer(peer,PRINT_SERVER_DATA);
				MsaUtility::sendDataToPeer(peer,prepareToSendUsers(getConnectedUsers()));
			}
			break;
		case GET_ROOM_USERS:
			if(currUser != NULL){
				string roomNameFromClient = string(MsaUtility::readDataFromPeer(peer));

				vector<User*> Users = getAllUsersInRoom(roomNameFromClient);

				if(Users.size() == 0) {
					MsaUtility::sendCommandToPeer(peer,NO_ROOMS);
				} else {
					MsaUtility::sendCommandToPeer(peer,PRINT_SERVER_DATA);
					MsaUtility::sendDataToPeer(peer,prepareToSendUsers(Users));
				}
			}
			break;
		case GET_ROOMS:
			if(currUser != NULL){
				MsaUtility::sendCommandToPeer(peer,PRINT_SERVER_DATA);
				MsaUtility::sendDataToPeer(peer,prepareToSendRooms(getAllRooms()));
			}
			break;
		case GET_ALL_USERS:
			if(currUser != NULL){
				MsaUtility::sendCommandToPeer(peer,PRINT_SERVER_DATA);
				MsaUtility::sendDataToPeer(peer,prepareToSendUsers(getAllUsers()));
			}
			break;
		case DISCONNECT:
			if(currUser->roomName != "") {
				_Manager->roomNameToRoom.erase(currUser->roomName);
			}
			break;
		default:
			break;
	}
}
string MsaTcpDispatcher::prepareToSendRooms(vector<Room*> rRooms) {
	string strRooms;

	for(unsigned int i=0;i < rRooms.size();i++) {
		strRooms.append(rRooms.at(i)->roomName);
		if(i!=rRooms.size()-1){
			strRooms.append(" ");
		}
	}

	return strRooms;
}
string MsaTcpDispatcher::prepareToSendUsers(vector<User*> uUsers) {
	string strUsers;

	for(unsigned int i=0;i < uUsers.size();i++) {
		strUsers.append(uUsers.at(i)->username);
		if(i!=uUsers.size()-1){
			strUsers.append(" ");
		}
	}

	return strUsers;
}

vector<User*> MsaTcpDispatcher::getConnectedUsers(){
	vector<User*> Users;

	for (std::map<string,User*>::iterator it = _Manager->addressToUser.begin();it != _Manager->addressToUser.end();it++) {
		Users.push_back(it->second);
	}

	return Users;
}

vector<User*> MsaTcpDispatcher::getAllUsers(){
	vector<string> usersVec = getUsersFromFile();
	int numUsers = usersVec.size();
	vector<User*> Users;
	User* userFromFile;

	for(int i = 0; i <numUsers;i++)
	{
		string currUser=usersVec.at(i);
		// parse user name from file
		currUser=currUser.substr(0, currUser.find("#"));
		userFromFile = new User(currUser);
		Users.push_back(userFromFile);
	}

	return Users;
}

vector<Session*> MsaTcpDispatcher::getAllSessions(){
	return _Manager->arrSessions;
}

vector<Room*> MsaTcpDispatcher::getAllRooms(){
	vector<Room*> Rooms;
	if(_Manager->roomNameToRoom.size() > 0) {
		for (std::map<string,Room*>::iterator it = _Manager->roomNameToRoom.begin();it != _Manager->roomNameToRoom.end();it++) {
			Rooms.push_back(it->second);
		}
	}

	return Rooms;
}

vector<User*> MsaTcpDispatcher::getAllUsersInRoom(string name){
	if(_Manager->roomNameToRoom.find(name) != _Manager->roomNameToRoom.end()) {
		return _Manager->roomNameToRoom[name]->arrUsers;
	}
	else {
		vector<User*> emptyUsers;
		return emptyUsers;
	}
}

void MsaTcpDispatcher::openSession(User* tmpPeer) {
	string secendUserName =  string(MsaUtility::readDataFromPeer(tmpPeer->socket));

	if(_Manager->usernameToUser.find(secendUserName) != _Manager->usernameToUser.end()) {
		User* secendUser = _Manager->usernameToUser[secendUserName];
		if(secendUser->sessionWithUserName == "" && secendUser->roomName == "") {
			MsaUtility::sendCommandToPeer(secendUser->socket,OPEN_SESSION);
			int state = MsaUtility::readCommandFromPeer(secendUser->socket);

			if(state == FREE) {
				MsaUtility::sendCommandToPeer(tmpPeer->socket,SESSION_ACCEPTED);
				MsaUtility::sendDataToPeer(tmpPeer->socket,secendUser->username + " " + secendUser->socket->destIpAndPort());

				MsaUtility::sendCommandToPeer(secendUser->socket,SESSION_ACCEPTED);
				MsaUtility::sendDataToPeer(secendUser->socket,tmpPeer->username + " " + tmpPeer->socket->destIpAndPort());

				Session* newSession = new Session(tmpPeer,secendUser);
				_Manager->arrSessions.push_back(newSession);
			} else {
				MsaUtility::sendCommandToPeer(tmpPeer->socket,SESSION_DENIED);
			}
		} else {
			MsaUtility::sendCommandToPeer(tmpPeer->socket,SESSION_DENIED);
		}
	}
	else {
		MsaUtility::sendCommandToPeer(tmpPeer->socket,SESSION_DENIED);
	}

}

void MsaTcpDispatcher::closeSession(User* tmpPeer) {
	for(unsigned int i=0;i<_Manager->arrSessions.size();i++)
	{
		if((_Manager->arrSessions.at(i)->firstUser->socket->destIpAndPort() == tmpPeer->socket->destIpAndPort()) ||
		   _Manager->arrSessions.at(i)->secoundUser->socket->destIpAndPort() == tmpPeer->socket->destIpAndPort())
		{
			User* firstUser = _Manager->arrSessions.at(i)->firstUser;
			User* secoundUser = _Manager->arrSessions.at(i)->secoundUser;
			MsaUtility::sendCommandToPeer(firstUser->socket,CLOSE_SESSION);
			MsaUtility::sendCommandToPeer(secoundUser->socket,CLOSE_SESSION);
			_Manager->arrSessions.erase(_Manager->arrSessions.begin()+i);
			return;
		}
	}
}

void MsaTcpDispatcher::createRoom(User* tmpPeer) {
	if(tmpPeer->roomName == "") {
		string RequestRoomName =  string(MsaUtility::readDataFromPeer(tmpPeer->socket));

		if(_Manager->roomNameToRoom.find(RequestRoomName) == _Manager->roomNameToRoom.end()) {
			Room* newRoom = new Room(RequestRoomName, tmpPeer);
			newRoom->arrUsers.push_back(tmpPeer);
			_Manager->roomNameToRoom.insert(pair<string,Room*>(RequestRoomName,newRoom));

			MsaUtility::sendCommandToPeer(tmpPeer->socket,ROOM_CREATED);
		} else {
			MsaUtility::sendCommandToPeer(tmpPeer->socket,ROOM_EXISTS);
		}
	}
}

void MsaTcpDispatcher::enterRoom(User* tmpPeer) {
	if(tmpPeer->roomName == "") {
			string RequestRoomName =  string(MsaUtility::readDataFromPeer(tmpPeer->socket));
			if(_Manager->roomNameToRoom.find(RequestRoomName) != _Manager->roomNameToRoom.end()) {
				Room* room = _Manager->roomNameToRoom[RequestRoomName];
				MsaUtility::sendCommandToPeer(tmpPeer->socket,JOIN_ROOM_SUCCESSFUL);
				MsaUtility::sendDataToPeer(tmpPeer->socket,room->roomName);

				room->arrUsers.push_back(tmpPeer);
				room->UpdateRoomUsers(tmpPeer,JOIN_ROOM);
			} else {
				MsaUtility::sendCommandToPeer(tmpPeer->socket,NO_SUCH_ROOM);
			}
	}
}

void MsaTcpDispatcher::exitRoom(User* tmpPeer) {
	if(tmpPeer->roomName != "") {
		Room* room = _Manager->roomNameToRoom[tmpPeer->roomName];
		if(room->roomOwner->username == tmpPeer->username) {
			_Manager->roomNameToRoom.erase(tmpPeer->roomName);
		} else {
			for(unsigned int i=0;i<room->arrUsers.size();i++)
			{
				if(room->arrUsers.at(i)->socket->destIpAndPort() == tmpPeer->socket->destIpAndPort())
				{
					room->arrUsers.erase(room->arrUsers.begin() + i);
					room->UpdateRoomUsers(tmpPeer,LEAVE_ROOM);
					MsaUtility::sendCommandToPeer(tmpPeer->socket,EXIT_ROOM);
					return;
				}
			}
		}
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

bool MsaTcpDispatcher::registerUser(User* tmpPeer)
{

	char* buffer = MsaUtility::readDataFromPeer(tmpPeer->socket);

	// get user and password
	char* userName = strtok(buffer," ");
	char* password = strtok(NULL, " ");

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
				MsaUtility::sendCommandToPeer(tmpPeer->socket,REGISTRATION_FAILED);
				return false;
			}


	}

	tmpPeer->username = userName;
	tmpPeer->password = password;

	// open file for writing
	ofstream pin2;
	pin2.open("users.txt",ofstream::app);

	// add user to users file
	pin2<<userName<<"#"<<password<<endl;
	pin2.close();


	//free(tmpUserAndPass);
	connect(tmpPeer);
	MsaUtility::sendCommandToPeer(tmpPeer->socket,REGISTRATION_SUCCESSFUL);
	cout<<userName<<" has been registerd"<<endl;

	return true;

}

void MsaTcpDispatcher::login(User* tmpPeer)
{
	//get user name and password
	cout<<"in login"<<endl;
	char* buffer = MsaUtility::readDataFromPeer(tmpPeer->socket);
	string userName = strtok(buffer," ");
	string password = strtok(NULL," ");

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
			tmpPeer->username = userName;
			tmpPeer->password = password;
			connect(tmpPeer);

			return;
		}

	}

	MsaUtility::sendCommandToPeer(tmpPeer->socket, LOGIN_ERROR);
}

void MsaTcpDispatcher::connect(User* tmpUser)
{
	if(GetUser(tmpUser->socket) == NULL)
	{
		cout<<"user "<< tmpUser->username <<" connected successfully to server"<<endl;
		tmpUser->isLoggedIn = true;
		//this->_Manager->addressToUser.insert(pair<string,User*>(tmpUser->socket->destIpAndPort(),tmpUser));
		this->_Manager->usernameToUser.insert(pair<string,User*>(tmpUser->username,tmpUser));
		MsaUtility::sendCommandToPeer(tmpUser->socket,LOGIN_ACCEPTED);
		MsaUtility::sendDataToPeer(tmpUser->socket,tmpUser->socket->destIpAndPort());
	}
	else
	{
		MsaUtility::sendCommandToPeer(tmpUser->socket,LOGIN_ERROR);
	}
}

User* MsaTcpDispatcher::GetUser(TCPSocket* peer) {
	if(this->_Manager->addressToUser.find(peer->destIpAndPort()) != this->_Manager->addressToUser.end()) {
		User* currUser = this->_Manager->addressToUser[peer->destIpAndPort()];
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
	mtsl = new MultipleTCPSocketsListener();
	mtsl->addSockets(this->_Manager->getPeersVector());

	return mtsl->listenToSocket(2);
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

void MsaTcpDispatcher::addPeer(TCPSocket* peer)
{
	User* newUser = new User(peer);
	this->_Manager->addressToUser.insert(pair<string,User*>(newUser->socket->destIpAndPort(),newUser));
}


