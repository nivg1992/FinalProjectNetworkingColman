/*
 * User.h
 *
 *  Created on: Jan 8, 2016
 *      Author: user
 */

#include <string>
#include "TCPSocket.h"


#ifndef USER_H_
#define USER_H_

class User {
public:
	User(string iUserName,string iPassword,TCPSocket* iSocket);
	virtual ~User();
	string username;
	string password;
	TCPSocket* socket;
	bool isLoggedIn;
};

#endif /* USER_H_ */
