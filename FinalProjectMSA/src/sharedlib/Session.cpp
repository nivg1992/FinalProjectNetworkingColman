/*
 * Session.cpp
 *
 *  Created on: Jan 8, 2016
 *      Author: user
 */

#include "Session.h"

Session::Session(User* iFUser,User* iSUser) {
	this->firstUser = iFUser;
	this->secoundUser = iSUser;

}

Session::~Session() {
}

