/*
 * TCPMessengerProtocol.h
 *
 *  Created on: Feb 13, 2013
 *      Author: efi
 */

#ifndef TCPMESSENGERPROTOCOL_H_
#define TCPMESSENGERPROTOCOL_H_

/**
 * TCP Messenger protocol:
 * all messages are of the format [Command 4 byte int]
 * and optionally data follows in the format [Data length 4 byte int][ Data ]
 */
// GENERAL
#define SERVER_PORT 				3346
#define NONE						1
#define PRINT_SERVER_DATA	     	2
#define DISCONNECT                  3
#define SERVER_CLOSED			    4

// P2P
#define OPEN_SESSION 				5
#define CLOSE_SESSION 				6
#define SEND_MESSAGE				7
#define SESSION_DENIED				8
#define SESSION_ACCEPTED			9

// LOGIN & REGISTRARION
#define REGISTER_USER  				10
#define REGISTRATION_SUCCESSFUL		11
#define REGISTRATION_FAILED			12
#define LOGIN_REQUEST 		     	13
#define LOGIN_ERROR    			    14
#define LOGIN_ACCEPTED    		    15

// USER STATUS
#define CONNECTED					16
#define NOT_CONNECTED               17
#define FREE                        18
#define IN_SESSION                  19
#define IN_ROOM                     20

// REQUEST DATA FROM SERVER
#define GET_ALL_USERS 				21
#define GET_CONNECTED_USERS			22
#define GET_ROOM_USERS				23
#define GET_ROOMS			     	24

// ROOM ACTIONS
#define NO_ROOMS					25
#define EXIT_ROOM					26
#define JOIN_ROOM                   27
#define CREATE_ROOM 				28
#define ROOM_EXISTS   	            29
#define ROOM_CREATED				30
#define NO_SUCH_ROOM				31
#define JOIN_ROOM_SUCCESSFUL        32
#define ROOM_UPDATED  		        33
#define LEAVE_ROOM                  34


#define SESSION_DENIED_MSG "Connection to peer refused, peer might be busy or disconnected, try again later"



#endif /* TCPMESSENGERPROTOCOL_H_ */
