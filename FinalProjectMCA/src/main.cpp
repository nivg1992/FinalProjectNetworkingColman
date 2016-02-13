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
	TCPMessengerClient* MC = new TCPMessengerClient();
	while(true){
		string msg;
		string command;
		cin >> command;
		if(command == "c")
				{
					string ip;
					cin >> ip;
					MC->connect(ip); //connect to server
				}
				else if(command == "login")
				{
					string user, password;
					cin >> user;
					cin >> password;
					MC->login(user,password);//login to server
				}
				else if(command == "lu")
				{
					MC->listRegisteredUsers();//prints registered users
				}
				else if(command == "lr")
				{
					MC->listRooms();//prints rooms
				}
				else if(command == "i")
				{
					printInstructions();//display printInstructions
				}
				else if(command == "o")
				{
					string name;
					cin >>name;
					MC->openSession(name); //opens a session with user
				}
				else if(command == "or")
				{
					string name;
					cin >>name;
					MC->joinRoom(name); // opens a room
				}
				else if(command == "rg"){
					string userName;
					string password;
					cin>>userName;
					cin>>password;
					MC->registration(userName,password); //register and login a new user
				}
				else if(command == "s") // sends a message
				{
						getline(std::cin,msg);

						if (MC->clientStatus!=IN_SESSION && MC->clientStatus!=IN_ROOM){
							cout<<"No opened rooms or session - open one first"<<endl;
							printInstructions();
						}
						else
						{
							MC->sendMessage(msg);
						}
				}
				else if(command == "cs") // closes session or leave the room your in
				{
					if(MC->clientStatus==IN_SESSION)
						MC->closeSession();

					else if(MC->clientStatus==IN_ROOM)
						MC->exitRoom();

					else
						cout<<"No open rooms or session."<<endl;


				}
				else if(command == "cr") //creates room
				{
					string inputRoomName;
					cin>>inputRoomName;

					if (MC->clientStatus == IN_SESSION || MC->clientStatus == IN_ROOM)
					{
						cout<<"Please close all Sessions/Rooms before creating a new one."<<endl;
					}
					else if(MC->clientStatus == NOT_CONNECTED)
						cout<<"The Client is offline."<<endl;
					else
					{
						MC->createRoom(inputRoomName);
					}
				}
				else if(command == "lru"){//prints users in a room
					string roomName;
					cin>>roomName;
					MC->printUsersInRoom(roomName);
				}
				else if(command == "lcu"){ //list connected users
					MC->listConnectedUsers();
				}else if(command == "d"){//disconnect from server
					MC->disconnect();
					offline=true;
				}else if(command == "l"){//display current state
					MC->printStatus();
				}else if(command == "x"){//exit the client
					MC->disconnect();
					break;
				}
				else{
					cout<<"wrong choice - Please choose again"<<endl;
					printInstructions();
				}
	}
//	msngrServer.close();
	cout<<"messenger was closed"<<endl;
	return 0;
}
