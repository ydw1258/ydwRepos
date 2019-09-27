#pragma once
#include <Windows.h>
#include <iostream>
#include <string>
#include <map>
#include <fstream>
//#include <WinSock2.h>
#include "..\..\Common\PACKET_HEADER.h"
#include "UserInfo.h"
#include <list>

using namespace std;

class ServerManager
{
private:
	static ServerManager* mthis;
	map<SOCKET, USER_INFO*> g_mapUser; //접속한 유저 정보들
	map<string, string> mapAccounts;
	bool turn = 0;
	HWND hWnd;
	int g_iIndex = 0;
	map<int, list<USER_INFO*>> g_RoomInfo; //0번 로비
	int roomIndexes[7] = { 0 };
public:
	static ServerManager* GetInstance()
	{
		if (mthis == nullptr)
			mthis = new ServerManager();
		return mthis;
	}
	ServerManager();
	~ServerManager();
	void Init(HWND _hWnd);
	
	void ProcessSocketMessage(HWND, UINT, WPARAM, LPARAM);
	bool ProcessPacket(SOCKET sock, USER_INFO* pUser, char* szBuf, int& len);
	void err_display(int errcode);
	void err_display(char* szMsg);
};

