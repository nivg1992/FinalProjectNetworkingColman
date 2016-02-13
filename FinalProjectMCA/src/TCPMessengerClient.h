
#include <strings.h>
#include <map>
#include <vector>
#include "MThread.h"
#include "TCPSocket.h"
#include "UDPHandeler.h"
#include "TCPMessengerProtocol.h"

using namespace std;

/**
 * The TCP Messenger client class
 */
class TCPMessengerClient: public MThread{
	TCPSocket* clientSocket;

public:
	bool connected;
	bool socketConnected;
	UDPHandeler* UDPhandeler;
	string roomName;
	string userSession;
	string username;
	int clientStatus;

	void run();//handle server response
	TCPMessengerClient(); //constractor
	bool connect(string ip);//connect to server
	bool login(string user,string password);//login to server
	void sendString2Sock(string msg,int protocol);//send string to server
	bool registertion(string user,string password);//register to server
	bool isConnected();//check if connected
	bool disconnect();//disconnect from server
	bool openSession(string username);//open a session
	bool joinRoom(string roomName);//open a room
	bool closeSession();//close session
	bool sendMessage(string msg);//send message
	void sentInt2Sock(int command);//send int to server
	void createRoom(string roomName);//creates a new room
	void exitRoom();//exit the current room
	void printStatus();//prints client status
	void print(string data, int NI);//print data
	void printUsersInRoom(string roomName);//prints users in room
	void listRooms();//print all rooms
	void listConnectedUsers();//prints connected users
	void listRegisteredUsers();//prints registered users

};

