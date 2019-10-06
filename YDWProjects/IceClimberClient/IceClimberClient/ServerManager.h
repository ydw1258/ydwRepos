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
	
	map<int, int> mapRoomPlayers; //방번호, 플레이어 숫자
	char playerID[10];
	int playerIndex = 99;
	int userIndexInRoom;
	bool isGameStart = false;
	int roomIndex;

	//서버관련
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

	//서버 통신 관련
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
	void GetPlayersInRoom(int roomNum); //로비포함 같은 방에 있는 사람 불러오기
	void EnterTheRoom(POINT pt);
	void ExitTheRoom(POINT pt);
	void GameStart(POINT pt);
	void ClientExit(POINT pt);
	void GetRooms();

	PacketManager();
	~PacketManager();
};