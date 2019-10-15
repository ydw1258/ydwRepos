#pragma once
#include <WinSock2.h>
#include "PACKET.h"

class UserInfo
{
public:
	SOCKET socket;
	char recvBuf[BUFSIZE];
	int recvLen;

	char id[IDPWSIZE];
	PLAYER_STATE state;
	int roomNum;

public:
	UserInfo();
	~UserInfo();
};

