/*
 * MsaTcpDispatcher.cpp
 *
 *  Created on: Jan 8, 2016
 *      Author: user
 */

#include "MsaTcpDispatcher.h"
#include <cstring>

MsaTcpDispatcher::MsaTcpDispatcher(MsaManager* Manager) {
	this->_Manager = Manager;
}

MsaTcpDispatcher::~MsaTcpDispatcher() {
	delete mtsl;
}

void MsaTcpDispatcher::processReadyPeer(TCPSocket* peer)
{
	int command = MsaManager::readCommandFromPeer(peer);
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
				MsaManager::sendCommandToPeer(peer,PRINT_SERVER_DATA);
				MsaManager::sendDataToPeer(peer,prepareToSendUsers(getConnectedUsers()));
			}
			break;
		case GET_ROOM_USERS:
			if(currUser != NULL){
				string roomNameFromClient = MsaManager::readDataFromPeer(peer);

				vector<Room*> rooms = getAllUsersInRoom(roomNameFromClient);

				if(rooms == NULL) {
					MsaManager::sendCommandToPeer(peer,NO_ROOMS);
				} else {
					MsaManager::sendCommandToPeer(peer,PRINT_SERVER_DATA);
					MsaManager::sendDataToPeer(peer,prepareToSendRooms(rooms));
				}
			}
			break;
		case GET_ROOMS:
			if(currUser != NULL){
				MsaManager::sendCommandToPeer(peer,PRINT_SERVER_DATA);
				MsaManager::sendDataToPeer(peer,prepareToSendRooms(getAllRooms()));
			}
			break;
		case GET_ALL_USERS:
			if(currUser != NULL){
				MsaManager::sendCommandToPeer(peer,PRINT_SERVER_DATA);
				MsaManager::sendDataToPeer(peer,prepareToSendUsers(getAllUsers()));
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

	for(int i=0;i < rRooms.size();i++) {
		strRooms.append(rRooms.at(i)->roomName);
		if(i!=rRooms.size()-1){
			strRooms.append(" ");
		}
	}

	return strRooms;
}
string MsaTcpDispatcher::prepareToSendUsers(vector<User*> uUsers) {
	string strUsers;

	for(int i=0;i < uUsers.size();i++) {
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
	for(int i = 0; i <numUsers;i++)
	{
		string currUser=usersVec.at(i);
		// parse user name from file
		currUser=currUser.substr(0, currUser.find("#"));
		User* user = new User(currUser);
		Users.push_back(user);
	}

	return Users;
}

vector<Session*> MsaTcpDispatcher::getAllSessions(){
	return _Manager->arrSessions;
}

vector<Room*> MsaTcpDispatcher::getAllRooms(){
	vector<Room*> Rooms;

	for (std::map<string,Room*>::iterator it = _Manager->roomNameToRoom.begin();it != _Manager->roomNameToRoom.end();it++) {
		Rooms.push_back(it->second);
	}

	return Rooms;
}

vector<User*> MsaTcpDispatcher::getAllUsersInRoom(string name){
	Room* room = _Manager->roomNameToRoom.find(name);
	if(room != _Manager->roomNameToRoom.end()) {
		return room->arrUsers;
	}
	else {
		return NULL;
	}
}

void MsaTcpDispatcher::openSession(User* tmpPeer) {
	string secendUserName =  MsaManager::readDataFromPeer(tmpPeer->socket);

	User* secendUser = _Manager->usernameToUser.find(secendUserName);

	if(secendUser != _Manager->usernameToUser.end()) {
		if(secendUser->sessionWithUserName == "" && secendUser->roomName == "") {
			MsaManager::sendCommandToPeer(secendUser->socket,OPEN_SESSION);
			int state = MsaManager::readCommandFromPeer(secendUser->socket);

			if(state == FREE) {
				MsaManager::sendCommandToPeer(tmpPeer->socket,SESSION_ACCEPTED);
				MsaManager::sendDataToPeer(tmpPeer->socket,secendUser->username + " " + secendUser->socket->destIpAndPort());

				MsaManager::sendCommandToPeer(secendUser->socket,SESSION_ACCEPTED);
				MsaManager::sendDataToPeer(secendUser->socket,tmpPeer->username + " " + tmpPeer->socket->destIpAndPort());

				Session* newSession = new Session(tmpPeer,secendUser);
				_Manager->arrSessions.push_back(newSession);
			} else {
				MsaManager::sendCommandToPeer(tmpPeer->socket,SESSION_DENIED);
			}
		} else {
			MsaManager::sendCommandToPeer(tmpPeer->socket,SESSION_DENIED);
		}
	}
	else {
		MsaManager::sendCommandToPeer(tmpPeer->socket,SESSION_DENIED);
	}

}

void MsaTcpDispatcher::closeSession(User* tmpPeer) {
	for(int i=0;i<_Manager->arrSessions.size();i++)
	{
		if(_Manager->arrSessions.at(i) == tmpPeer->socket->destIpAndPort())
		{
			User* firstUser = _Manager->arrSessions.at(i)->firstUser;
			User* secoundUser = _Manager->arrSessions.at(i)->secoundUser;
			MsaManager::sendCommandToPeer(firstUser->socket,CLOSE_SESSION);
			MsaManager::sendCommandToPeer(secoundUser->socket,CLOSE_SESSION);
			_Manager->arrSessions.erase(_Manager->arrSessions.begin()+i);
			return;
		}
	}
}

void MsaTcpDispatcher::createRoom(User* tmpPeer) {
	if(tmpPeer->roomName == "") {
		string RequestRoomName =  MsaManager::readDataFromPeer(tmpPeer->socket);
		Room* room = _Manager->roomNameToRoom.find(RequestRoomName);

		if(room == _Manager->roomNameToRoom.end()) {
			Room* newRoom = new Room(RequestRoomName);
			newRoom->arrUsers.push_back(tmpPeer);
			newRoom->roomOwner = tmpPeer;
			_Manager->roomNameToRoom.insert(pair<string,Room*>(RequestRoomName,newRoom));

			MsaManager::sendCommandToPeer(tmpPeer->socket,ROOM_CREATED);
		} else {
			MsaManager::sendCommandToPeer(tmpPeer->socket,ROOM_EXISTS);
		}
	}
}

void MsaTcpDispatcher::enterRoom(User* tmpPeer) {
	if(tmpPeer->roomName == "") {
			string RequestRoomName =  MsaManager::readDataFromPeer(tmpPeer->socket);
			Room* room = _Manager->roomNameToRoom.find(RequestRoomName);
			if(room != _Manager->roomNameToRoom.end()) {
				MsaManager::sendCommandToPeer(tmpPeer->socket,JOIN_ROOM_SUCCESSFUL);
				MsaManager::sendDataToPeer(tmpPeer->socket,room->roomName);

				room->arrUsers.push_back(tmpPeer);
				room->UpdateRoomUsers(tmpPeer,JOIN_ROOM);
			} else {
				MsaManager::sendCommandToPeer(tmpPeer->socket,NO_SUCH_ROOM);
			}
	}
}

void MsaTcpDispatcher::exitRoom(User* tmpPeer) {
	if(tmpPeer->roomName != "") {
		Room* room = _Manager->roomNameToRoom.find(tmpPeer->roomName);
		if(room->roomOwner->username == tmpPeer->username) {
			_Manager->roomNameToRoom.erase(tmpPeer->roomName);
		} else {
			for(int i=0;i<room->arrUsers.size();i++)
			{
				if(room->arrUsers.at(i) == tmpPeer->socket->destIpAndPort())
				{
					room->arrUsers.erase(room->arrUsers.begin() + i);
					room->UpdateRoomUsers(tmpPeer,LEAVE_ROOM);
					MsaManager::sendCommandToPeer(tmpPeer->socket,EXIT_ROOM);
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

bool MsaTcpDispatcher::registerUser(TCPSocket* tmpPeer)
{

	char* buffer = MsaManager::readDataFromPeer(tmpPeer);

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
				MsaManager::sendCommandToPeer(newUser->socket,REGISTRATION_FAILED);
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
	MsaManager::sendCommandToPeer(newUser->socket,REGISTRATION_SUCCESSFUL);
	cout<<userName<<" has been registerd"<<endl;

	return true;

}

void MsaTcpDispatcher::login(TCPSocket* tmpPeer)
{
	//get user name and password
	cout<<"in login"<<endl;
	char* buffer = MsaManager::readDataFromPeer(tmpPeer);
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

	MsaManager::sendCommandToPeer(tmpPeer, LOGIN_ERROR);
}

void MsaTcpDispatcher::connect(User* tmpUser)
{
	if(GetUser(tmpUser->socket) == NULL)
	{
		cout<<"user "<< tmpUser->username <<" connected successfully to server"<<endl;
		tmpUser->isLoggedIn = true;
		this->_Manager->addressToUser.insert(pair<string,User*>(tmpUser->socket->destIpAndPort(),tmpUser));
		this->_Manager->usernameToUser.insert(pair<string,User*>(tmpUser->username,tmpUser));
		MsaManager::sendCommandToPeer(tmpUser->socket,LOGIN_ACCEPTED);
		MsaManager::sendDataToPeer(tmpUser->socket,tmpUser->socket->destIpAndPort());
	}
	else
	{
		MsaManager::sendCommandToPeer(tmpUser->socket,LOGIN_ERROR);
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
	if (this->_Manager->addressToUser.size() > 0)
	{
		return mtsl->listenToSocket(1);
	}
	return NULL;
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
	mtsl->addSocket(peer);
}


