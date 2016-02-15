
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
	UDPHandeler* Udphandeler;
	string roomName;
	string userSession;
	string username;
	int clientStatus;

	TCPMessengerClient();
	//connect to server
	bool connect(string ip);
	//login to server
	bool login(string user,string password);
	//register to server
	bool registration(string user,string password);
	//check if connected
	bool isConnected();
	//disconnect from server
	bool disconnect();
	//open a session
	bool openSession(string username);
	//open a room
	bool joinRoom(string roomName);
	//handle server response
	void run();
	//print data
	void print(string data, int NI);
	//print all rooms
	void listRooms();
	//prints users in room
	void printUsersInRoom(string roomName);
	//close session
	bool closeSession();
	//send message
	bool sendMessage(string msg);
	//send int to server
	void sentIntToSocket(int command);
	//send string to server
	void sendStringToSocket(string msg,int protocol);
	//creates a new room
	void createRoom(string roomName);
	//exit the current room
	void exitRoom();
	//prints client status
	void printStatus();
	//prints connected users
	void listConnectedUsers();
	//prints registered users
	void listRegisteredUsers();
};

