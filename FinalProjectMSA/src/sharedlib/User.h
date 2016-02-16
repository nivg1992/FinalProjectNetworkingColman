/*
 * User.h
 *
 *  Created on: Jan 8, 2016
 *      Author: user
 */
#ifndef USER_H_
#define USER_H_

#include <string>
#include "TCPSocket.h"

class User {
public:
	User(string iUserName);
	User(string iUserName,string iPassword,TCPSocket* iSocket);
	virtual ~User();
	string username;
	string password;
	TCPSocket* socket;
	bool isLoggedIn;
	string roomName;
	string sessionWithUserName;
};

#endif /* USER_H_ */
