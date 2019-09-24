#pragma once
#include <map>
#include "SpriteRenderer.h"
#include "FontManager.h"

#define WIDTH 19
#define HEIGHT 19
#define WM_SOCKET (WM_USER+1)
#define BUFSIZE 512

class GameManager
{
private:
	static GameManager* mthis;
	map<int, POINT> ptData; //플레이어, 전송데이터

	//서버관련
	int playerIndex= 3;
	SOCKET g_sock;
	WSADATA wsa;
	SOCKADDR_IN serveraddr;
public:
	SpriteRenderer board;
	SpriteRenderer blackStone;
	SpriteRenderer whiteStone;
	FontManager font;
	HWND hwnd;
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
	void DrawRect(HDC hdc);
	void MouseButtonCheck(POINT pt);
	void GameOverCheck();
	//서버 관련
	void SendPos(int x, int y);
	void ProcessSocketMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	void ProcessPacket(char* szBuf, int len);

	GameManager();
	~GameManager();
};