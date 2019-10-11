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

	//소켓, 패킷처리
	char packetBuf[BUFSIZE];
	int myLen = 0;
	WSADATA wsa;
	SOCKADDR_IN serveraddr;
	SOCKET g_sock;

	char playerID[10];
public:
	static PacketManager* GetInstance()
	{
		if (mthis == nullptr)
			mthis = new PacketManager();
		return mthis;
	}

	//게임 진행
	bool isGameStart = false;
	char answer[10] = "?";
	int score = 0;
	int curTurn;
	
	//유저 정보
	int userIndexInRoom;
	int roomIndex;
	//map<int, int> mapRoomPlayers; //방번호, 플레이어 숫자
	
	
	//패킷 전송
	void SendGameExit(int roomIndex);
	void SendEnterTheRoomPacket(int roomIndex);
	void SendExitTheRoom();
	void SendGameStart();
	void SendLogin(char* ID, char* password);
	void SendPos(DRAWPT pt);
	void SendChattingData(char* str);

	void InitConnection(HWND hwnd);
	//서버로 부터 받은 패킷 처리
	void ProcessSocketMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	bool ProcessPacket(char* szBuf, int len, WPARAM wParam);
	void GetPlayersInRoom(int roomNum); //로비포함 같은 방에 있는 사람 불러오기
	void GetRooms();
	bool isMyTurn();
	PacketManager();
	~PacketManager();

	//void ClientExit(POINT pt); //게임 종료버튼 클릭
	void Release();
};