#pragma once
#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include "..\..\Common\PACKET_HEADER.h"
#include <list>

using namespace std;

class ServerManager
{
private:
	static ServerManager* mthis;
	map<SOCKET, USER_INFO*> g_mapUser; //접속한 유저 정보들
	map<string, string> mapAccounts;
	HWND hWnd;
	int g_iIndex = 0;
	map<int, list<USER_INFO*>> g_RoomInfo; //0번 로비
	map<int, bool> g_isGameplaying;
	int roomIndexes[ROOMNUM] = { 0 };
	//
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;
	DWORD recvbytes, flags;

	SOCKET listen_sock;
	SOCKADDR_IN serveraddr;
public:
	static ServerManager* GetInstance()
	{
		if (mthis == nullptr)
			mthis = new ServerManager();
		return mthis;
	}
	int retval;
	HANDLE hcp;
	void Init();
	
	
	bool Update();
	USER_INFO* GetUserInfo(SOCKETINFO* sock);

	bool ProcessPacket(SOCKET sock, USER_INFO* pUser, char* szBuf, int& len);
	void ClientExit(SOCKET sock);
	ServerManager();
	~ServerManager();
};

