#pragma once
#include <map>
#include<vector>
#include "SpriteRenderer.h"
#include "FontManager.h"
#include "PACKET_HEADER.h"

#define WIDTH 19
#define HEIGHT 19
#define WM_SOCKET (WM_USER+1)
#define BUFSIZE 512

#define	BLACK RGB(0, 0, 0)
#define	WHITE = RGB(255, 255, 255)
#define	RED = RGB(255, 0, 0)
#define	GREEN = RGB(0, 255, 0)
#define	BLUE = RGB(0, 0, 255)
#define	LIGHTGREEN RGB(255, 255, 0)
#define	YELLOW = RGB(0, 255, 255)
#define	MAGENTA = RGB(255, 0, 255)

class GameManager
{
private:
	static GameManager* mthis;
	map<int, POINT> ptData; //플레이어, 전송데이터

	//서버관련
	int playerIndex = 3;
	SOCKET g_sock;
	WSADATA wsa;
	SOCKADDR_IN serveraddr;
	HWND chatInputBox;
	HWND hWndList;
public:
	Scene scene = INGAME;
	SpriteRenderer board;
	SpriteRenderer blackStone;
	SpriteRenderer whiteStone;
	FontManager font;
	HINSTANCE hInstance;
	HWND hwnd;
	vector<string> chatList;
	bool curTurn = 0;
	bool Mystone;
	int stoneSizeXY = 37;
	int BoardInfo[HEIGHT * WIDTH] = { 0 };
	
	int offsetX = 24;
	int offsetY = 26;

	static GameManager* GetInstance()
	{
		if (mthis == nullptr)
			mthis = new GameManager();
		return mthis;
	}
	void Init(HDC hdc, HINSTANCE hInstance, HWND _hwnd);
	void Draw(HDC hdc);
	void DrawChatWindow(HDC hdc);
	void DrawRect(HDC hdc);
	void MouseButtonCheck(POINT pt);
	void GameOverCheck();
	//서버 관련
	void SendPos(int x, int y);
	void ProcessSocketMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	void ProcessPacket(char* szBuf, int len);
	void InputChatting(void);
	GameManager();
	~GameManager();
};