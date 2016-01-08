//============================================================================
// Name        : TCPMessengerServer
// Author      : Eliav Menachi
// Version     :
// Copyright   :
// Description :
//============================================================================

#include <iostream>
#include "MsaManager.h"

using namespace std;

void printInstructions(){
	cout<<"-----------------------"<<endl;
	cout<<"lu - list all users"<<endl;
	cout<<"lcu - list all connected users"<<endl;
	cout<<"ls - list all sessions"<<endl;
	cout<<"lr - list all rooms"<<endl;
	cout<<"lru <room name> - list all users in this room"<<endl;
	cout<<"x - shutdown"<<endl;
	cout<<"-----------------------"<<endl;
}

int main(){
	cout<<"Welcome to TCP messenger Server"<<endl;
	printInstructions();

	MsaManager serverManagerInstance;


	while(true){
		string msg;
		string command;
		cin >> command;
		if (command == "lu")
		{
			serverManagerInstance.printAllUsers();
		}
		else if (command == "lcu")
		{
			serverManagerInstance.printAllConnetedUsers();
		}
		else if (command == "ls")
		{
			serverManagerInstance.printAllSessions();
		}
		else if (command == "lr")
		{
			serverManagerInstance.printAllRooms();
		}
		else if (command == "lru")
		{
			serverManagerInstance.printUsersByRoom(msg);
		}
		else if (command == "x")
		{
			break;
		}
		else
		{
			cout<<"wrong input"<<endl;
			printInstructions();
		}
	}
	serverManagerInstance.close();
	cout<<"messenger was closed"<<endl;
	return 0;
}
