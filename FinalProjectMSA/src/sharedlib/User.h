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
	User();
	virtual ~User();
	string username;
	string password;
	TCPSocket* socket;
	bool isLoggedIn;
	bool logIn(string username,string password);
};

#endif /* USER_H_ */
