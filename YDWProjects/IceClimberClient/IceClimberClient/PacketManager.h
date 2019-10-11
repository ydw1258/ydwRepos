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

	//����, ��Ŷó��
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

	//���� ����
	bool isGameStart = false;
	char answer[10] = "?";
	int score = 0;
	int curTurn;
	
	//���� ����
	int userIndexInRoom;
	int roomIndex;
	//map<int, int> mapRoomPlayers; //���ȣ, �÷��̾� ����
	
	
	//��Ŷ ����
	void SendGameExit(int roomIndex);
	void SendEnterTheRoomPacket(int roomIndex);
	void SendExitTheRoom();
	void SendGameStart();
	void SendLogin(char* ID, char* password);
	void SendPos(DRAWPT pt);
	void SendChattingData(char* str);

	void InitConnection(HWND hwnd);
	//������ ���� ���� ��Ŷ ó��
	void ProcessSocketMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	bool ProcessPacket(char* szBuf, int len, WPARAM wParam);
	void GetPlayersInRoom(int roomNum); //�κ����� ���� �濡 �ִ� ��� �ҷ�����
	void GetRooms();
	bool isMyTurn();
	PacketManager();
	~PacketManager();

	//void ClientExit(POINT pt); //���� �����ư Ŭ��
	void Release();
};