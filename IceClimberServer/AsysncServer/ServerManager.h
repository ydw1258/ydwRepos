#pragma once
#include <Windows.h>
#include <iostream>
#include <string>
#include <map>
#include <fstream>
//#include <WinSock2.h>
//CreateThread ���� ���� ���� ���� �� ����.
#pragma comment(lib, "ws2_32")
#include<WinSock2.h>
#include<stdlib.h>
#include<stdio.h>
#include "..\..\Common\PACKET_HEADER.h"
#include "UserInfo.h"
#include <list>
using namespace std;

#define ROOMNUM 20
#define SERVERPORT 9000
#define BUFSIZE 512

struct SOCKETINFO
{
	OVERLAPPED overlapped;
	SOCKET sock;
	char buf[BUFSIZE + 1];
	int recvbytes;
	int sendbytes;
	WSABUF wsabuf;
};

class ServerManager
{
private:
	static ServerManager* mthis;
	map<string, string> mapAccounts; //������ ���� ����
	map<SOCKET, USER_INFO*> g_mapUser; //������ ���� ������
	//���� �ؾ� �� ���
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;
	
	DWORD recvbytes;
	DWORD flags;

	HWND hWnd;

	map<int, list<ROOM_INFO*>> g_RoomInfo; //0�� �κ�, �� ������
	int roomIndexes[ROOMNUM] = { 0 };

	HANDLE hThread;
public:
	SOCKET listen_sock;
	HANDLE hcp;
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
