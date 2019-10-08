#pragma once
#include<Winsock2.h>
#include<Windows.h>
#include<vector>
#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include "..\..\Common\PACKET_HEADER.h"
#include <list>
#pragma comment(lib, "winmm.lib")
using namespace std;

struct USER_INFO_STRING
{
	char szBuf[BUFSIZE];
	int len;
};
struct SOCKETINFO
{
	WSAOVERLAPPED overlapped;
	SOCKET sock;
	char buf[BUFSIZE + 1];
	int recvbytes;
	int sendbytes;
	WSABUF wsabuf;
};
#define TIME_LIMIT 50
class ServerManager
{
private:
	static ServerManager* mthis;
	map<SOCKET, USER_INFO*> g_mapUser; //������ ���� ������
	map<string, string> mapAccounts;
	HWND hWnd;
	int g_iIndex = 0;
	map<int, list<USER_INFO*>> g_RoomInfo; //0�� �κ�
	map<int, bool> g_isGameplaying; // 0���� 1����
	float Timer[ROOMNUM] = { TIME_LIMIT };

	int roomIndexes[ROOMNUM] = { 0 };
	//
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;
	DWORD recvbytes, flags;

	SOCKET listen_sock;
	SOCKADDR_IN serveraddr;

	vector<string> answer;
	int curTurn = 0;
	DWORD lastTime = timeGetTime();   //������ �ð�(temp����)

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
	void TimerCheck();
	USER_INFO* GetUserInfo(SOCKETINFO* sock);

	bool ProcessPacket(SOCKET sock, USER_INFO_STRING* pUser, char* szBuf, int& len);
	void ClientExit(SOCKET sock);
	void InitUser(SOCKET sock);

	float deltaTime; //���� ��
	void deltaTimeInit();
	// ��ŸŸ���� ���Ѵ�.
	
	ServerManager();
	~ServerManager();
};