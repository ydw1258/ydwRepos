#pragma once
#include <map>
#include<vector>
#include<list>
#include <string>
#include<iostream>
#include "SpriteRenderer.h"
#include "FontManager.h"
#include "..\..\Common\PACKET_HEADER.h"
#include "IMAGENUM.h"
#include "Physics.h"
#include "SHA1.h"

using namespace std;


class PacketManager
{
private:
	static PacketManager* mthis;
	map<int, int> mapRoomPlayers; //방번호, 플레이어 숫자

	//서버관련
	char packetBuf[BUFSIZE];
	int myLen = 0;



	int playerIndex = 99;
	char playerID[10];
	WSADATA wsa;
	SOCKADDR_IN serveraddr;
	SOCKET g_sock;
public:
	bool isGameStart = false;
	
	int userIndexInRoom;
	int roomIndex;
	SERVERMANAGER_SCENE scene = LOGIN;

	HINSTANCE hInstance;
	HWND hwnd;

	static PacketManager* GetInstance()
	{
		if (mthis == nullptr)
			mthis = new PacketManager();
		return mthis;
	}

	//서버 통신 관련
	void SendGameExit(int roomIndex);
	void SendEnterTheRoomPacket(int roomIndex);
	void SendExitTheRoom();
	void SendGameStart();
	void SendLogin(char* ID, char* password);
	void SendPos(int x, int y);
	void SendChattingData(char* str);

	void ProcessSocketMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	bool ProcessPacket(char* szBuf, int len, WPARAM wParam);
	void InitConnection(HWND hwnd);
	void GetPlayersInRoom(int roomNum); //로비포함 같은 방에 있는 사람 불러오기
	//void ClientExit(POINT pt);
	void GetRooms();

	PacketManager();
	~PacketManager();
};