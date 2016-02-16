#ifndef MSAUTILITY_H_
#define MSAUTILITY_H_

#include <string>
#include <cstring>
#include "TCPSocket.h"

struct MsaUtility {
    static inline int readCommandFromPeer(TCPSocket* peer){

		int buffer;
		peer->recv((char*)&buffer,4);

		int Command = ntohl(buffer);
		return Command;
	}

    static inline char* readDataFromPeer(TCPSocket* peer){
		int buffer;
		//memset((void*)buffer,0,4);
		peer->recv((char*)&buffer,4);

		int DataLen = ntohl(buffer);

		char bufferData[DataLen];
		memset((void*)bufferData,0,DataLen);
		peer->recv(bufferData,DataLen);
		return bufferData;
	}

    static inline void sendCommandToPeer(TCPSocket* peer, int command){
		char buffer[4];
	    for (int i = 0; i < 4; i++)
	    	buffer[3 - i] = (command >> (i * 8));

		peer->send(buffer,4);
	}

    static inline void sendDataToPeer(TCPSocket* peer, string msg){
		char bufferMsgLen[4];
	    for (int i = 0; i < 4; i++)
	    	bufferMsgLen[3 - i] = (msg.length() >> (i * 8));

		peer->send(bufferMsgLen,4);
		peer->send(msg.data(), msg.length());
	}
};

#endif
