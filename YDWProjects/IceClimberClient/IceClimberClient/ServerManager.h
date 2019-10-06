#pragma once
#include<WinSock2.h>
#include <Windows.h>
#include <map>
#include<vector>
#include<list>
#include "SpriteRenderer.h"
#include "FontManager.h"

using namespace std;

#define WM_SOCKET (WM_USER+1)
#define BUFSIZE 1024

struct USER_INFO_STRING
{
	char szBuf[BUFSIZE];
	int len;
};
class PacketManager
{
private:
	static PacketManager* mthis;
	
	map<int, int> mapRoomPlayers; //���ȣ, �÷��̾� ����
	char playerID[10];
	int playerIndex = 99;
	int userIndexInRoom;
	bool isGameStart = false;
	int roomIndex;

	//��������
	SOCKET g_sock;
	WSADATA wsa;
	SOCKADDR_IN serveraddr;
public:
	SERVERMANAGER_SCENE scene = LOGIN;

	HINSTANCE hInstance;
	HWND hwnd;

	bool curTurn = 0;
	bool Mystone;

	static PacketManager* GetInstance()
	{
		if (mthis == nullptr)
			mthis = new PacketManager();
		return mthis;
	}
	
	void Login();

	//���� ��� ����
	void SendGameExit(int roomIndex);
	void SendEnterTheRoomPacket(int roomIndex);
	void SendExitTheRoom();
	void SendGameStart();
	void SendLogin(char ID[10], char password[10]);
	void SendPos(int x, int y);
	void ProcessSocketMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	bool ProcessPacket(char* szBuf, USER_INFO_STRING& userinfo, int len, WPARAM wParam);
	void InitConnection();
	void InputChatting(void);
	void GetPlayersInRoom(int roomNum); //�κ����� ���� �濡 �ִ� ��� �ҷ�����
	void EnterTheRoom(POINT pt);
	void ExitTheRoom(POINT pt);
	void GameStart(POINT pt);
	void ClientExit(POINT pt);
	void GetRooms();

	PacketManager();
	~PacketManager();
};