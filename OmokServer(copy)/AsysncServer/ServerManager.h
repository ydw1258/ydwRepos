#pragma once
#include<Windows.h>
#include<iostream>
#include "..\..\Common\PACKET_HEADER.h"
#include <map>
using namespace std;
#define BUFSIZE 512
#define WM_SOCKET (WM_USER+1)

class USER_INFO
{
public:
	int index;
	char szBuf[BUFSIZE];
	int len;
	int x;
	int y;
};

class ServerManager
{
private:
	int retval = 0;
	WSADATA wsa;
	static ServerManager* mthis;
	char chatMessage[128];

public:
	static ServerManager* GetInstance()
	{
		if (mthis == nullptr)
			mthis = new ServerManager();
		return mthis;
	}
	void Init(HWND hwnd);
	int g_iIndex = 0;
	map<SOCKET, USER_INFO*> g_mapUser;
	void ProcessSocketMessage(HWND, UINT, WPARAM, LPARAM);
	bool ProcessPacket(SOCKET sock, USER_INFO* pUser, char* szBuf, int& len);
	void err_display(int errcode);
	void err_display(char* szMsg);
	bool turn = 0;

	ServerManager();
	~ServerManager();
};
