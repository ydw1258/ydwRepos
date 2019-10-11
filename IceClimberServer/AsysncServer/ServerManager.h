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
#include <chrono>
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
#define TIME_LIMIT 10
#define MAX_ROTATION 2 //전체유저가 출제할 최대 정답

class ServerManager
{
private:
	chrono::system_clock::time_point m_LastTime;
	float		m_fElapseTime;

	static ServerManager* mthis;
	map<SOCKET, USER_INFO*> g_mapUser; //접속한 유저 정보들
	map<string, string> mapAccounts;
	HWND hWnd;
	
	map<int, list<USER_INFO*>> g_RoomInfo; //0번 로비
	map<int, bool> g_isGameplaying; // 0번이 1번방
	float Timer[ROOMNUM] = { TIME_LIMIT };

	int roomIndexes[ROOMNUM] = { 0 };
	//
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;
	DWORD recvbytes, flags;

	SOCKET listen_sock;
	SOCKADDR_IN serveraddr;
	int ROTATION = 0;
	vector<string> answer;
	char curAnswer[128];
	int curTurn[ROOMNUM] = { 0 };
	DWORD lastTime = timeGetTime();   //마지막 시간(temp변수)
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

	// 델타타임을 구한다.
	
	ServerManager();
	~ServerManager();
};