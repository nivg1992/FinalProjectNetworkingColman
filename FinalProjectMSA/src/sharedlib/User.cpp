/*
 * User.cpp
 *
 *  Created on: Jan 8, 2016
 *      Author: user
 */

#include "User.h"

User::User(string iUserName) {
	this->username = iUserName;
	this->password = "";
	this->socket = NULL;
	isLoggedIn = false;

}

User::User(string iUserName,string iPassword,TCPSocket* iSocket) {
	this->username = iUserName;
	this->password = iPassword;
	this->socket = iSocket;
	isLoggedIn = false;

}

User::~User() {
	// TODO Auto-generated destructor stub
}

