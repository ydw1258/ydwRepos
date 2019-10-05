#pragma once
#include <map>
#include<vector>
#include<list>
#include "SpriteRenderer.h"
#include "FontManager.h"
#include "..\Common\PACKET_HEADER.h"

using namespace std;

#define WM_SOCKET (WM_USER+1)
#define BUFSIZE 1024

struct USER_INFO_STRING
{
	char szBuf[BUFSIZE];
	int len;
};
class ServerManager
{
private:
	static ServerManager* mthis;
	//map<int, POINT> ptData; //플레이어, 전송데이터
	map<int, int> mapRoomPlayers; //방번호, 플레이어 숫자
	char playerID[10];
	int playerIndex = 3;
	int userIndexInRoom;
	bool isGameStart = false;
	int roomIndex;

	//UI
	list<string> chatList;
	list<string> listPlayerID;
	list<RECT> roomButtons;
	RECT roomExitButton;
	RECT startButton;
	RECT RoomExitButton;
	RECT gameExitButton;
	HWND chatInputBoxLobby;
	HWND chatInputBoxIngame;
	HWND LOGINInput[2];
	SpriteRenderer board;
	SpriteRenderer UIbuttons[10];
	FontManager normalFont;

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

	static ServerManager* GetInstance()
	{
		if (mthis == nullptr)
			mthis = new ServerManager();
		return mthis;
	}
	void Init(HDC hdc, HINSTANCE hInstance, HWND _hwnd);
	void Draw(HDC hdc);
	void DrawChatWindow(HDC hdc);
	void DrawCurUsers(HDC hdc);
	void DrawRooms(HDC hdc);
	void Login();

	//서버 관련
	void SendPos(int x, int y);
	void ProcessSocketMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	bool ProcessPacket(char* szBuf, USER_INFO_STRING& userinfo, int len, WPARAM wParam);
	void InitConnection();
	void InputChatting(void);
	void SceneInitiator();
	void SceneChange(Scene _scene);
	void GetPlayersInRoom(int roomNum); //로비포함 같은 방에 있는 사람 불러오기
	void EnterTheRoom(POINT pt);
	void ExitTheRoom(POINT pt);
	void GameStart(POINT pt);
	void ClientExit(POINT pt);
	void GetRooms();

	ServerManager();
	~ServerManager();
};