/*
 * main.cpp
 *
 *  Created on: Feb 8, 2016
 *      Author: Michael Blitshtein
 */

#include <iostream>
#include <string.h>
#include "TCPMessengerProtocol.h"
#include "TCPMessengerClient.h"

using namespace std;

void printInstructions(){
	cout<<"-----------------------"<<endl;
	cout<<"c <IP> - connect to the server in the given ip"<<endl;
	cout<<"lu - print the user list from the server"<<endl;
	cout<<"lcu - print the connected users list"<<endl;
	cout<<"lr - print all rooms"<<endl;
	cout<<"lru <room name> - print all users in this room"<<endl;
	cout<<"login <user> <password> - login with the user and password"<<endl;
	cout<<"register <user> <password> - register the new user with the given password and login the user."<<endl;
	cout<<"o <username> - open a session with the user"<<endl;
	cout<<"or <room name> - enter a chat room"<<endl;
	cout<<"s <message> - send a message"<<endl;
	cout<<"l - print the current status of the client "<<endl;
	cout<<"cs - disconnect the open session / exit from a room"<<endl;
	cout<<"d - disconnect from server"<<endl;
	cout<<"x - close the app"<<endl;
	cout<<"-----------------------"<<endl;
}

int main(){
	cout<<"Welcome to TCP messenger Client"<<endl;
	printInstructions();
	bool offline=false;
	TCPMessengerClient* Client = new TCPMessengerClient();
	while(true){
		string message;
		string command;
		cin >> command;
		//connect to server
		if(command == "c")
		{
			string ip;
			cin >> ip;
			Client->connect(ip);
		}
		else if(command == "login")
		{
			string user, password;
			cin >> user;
			cin >> password;
			Client->login(user,password);
		}
		//prints registered users
		else if(command == "lu")
		{
			Client->listRegisteredUsers();
		}
		//prints rooms
		else if(command == "lr")
		{
			Client->listRooms();
		}
		else if(command == "i")
		{
			printInstructions();
		}
		//opens a session with user
		else if(command == "o")
		{
			string name;
			cin >>name;
			Client->openSession(name);
		}
		// opens a room
		else if(command == "or")
		{
			string name;
			cin >>name;
			Client->joinRoom(name);
		}
		//register and login a new user
		else if(command == "rg"){
			string userName;
			string password;
			cin>>userName;
			cin>>password;
			Client->registration(userName,password);
		}
		// send a message
		else if(command == "s")
		{
			cin >> message;

			if (Client->clientStatus != IN_SESSION && Client->clientStatus != IN_ROOM){
				cout<<"There are no rooms\session"<<endl;
				printInstructions();
			}
			else
			{
				Client->sendMessage(message);
			}
		}
		// close session or leave the room your in
		else if(command == "cs")
		{
			if(Client->clientStatus==IN_SESSION)
				Client->closeSession();
			else if(Client->clientStatus==IN_ROOM)
				Client->exitRoom();
			else
				cout<<"No open rooms or session."<<endl;
		}
		//create a room
		else if(command == "cr")
		{
			string roomName;
			cin>>roomName;

			if (Client->clientStatus == IN_SESSION || Client->clientStatus == IN_ROOM)
			{
				cout<<"Close all Sessions/Rooms before creating a new one."<<endl;
			}
			else if(Client->clientStatus == NOT_CONNECTED)
				cout<<"The Client is offline."<<endl;
			else
			{
				Client->createRoom(roomName);
			}
		}
		//prints users in a room
		else if(command == "lru"){
			string roomName;
			cin>>roomName;
			Client->printUsersInRoom(roomName);
		}
		//print list of connected users
		else if(command == "lcu"){
			Client->listConnectedUsers();
		}
		//disconnect from server
		else if(command == "d")
		{
			Client->disconnect();
			offline=true;
		}
		//print current state
		else if(command == "l"){
			Client->printStatus();
		}
		else if(command == "x")
		{
			Client->disconnect();
			break;
		}
		else
		{
			cout<<"wrong choice - Please choose again"<<endl;
			printInstructions();
		}
	}
//	msngrServer.close();
	cout<<"messenger was closed"<<endl;
	return 0;
}
