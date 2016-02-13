/*
 * Session.h
 *
 *  Created on: Jan 8, 2016
 *      Author: user
 */

#ifndef SESSION_H_
#define SESSION_H_

#include "User.h"

class Session {
public:
	Session(User* iFUser,User* iSUser);
	virtual ~Session();
	User* firstUser;
	User* secoundUser;

};

#endif /* SESSION_H_ */
