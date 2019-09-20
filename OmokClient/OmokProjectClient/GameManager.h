#pragma once
#include <map>
#include "SpriteRenderer.h"

#define WIDTH 19
#define HEIGHT 19
#define WM_SOCKET (WM_USER+1)

class GameManager
{
private:
	static GameManager* mthis;
	map<int, POINT> ptData; //�÷��̾�, ���۵�����

	//��������
	int g_iIndex = 0;
public:
	SpriteRenderer board;
	SpriteRenderer blackStone;
	SpriteRenderer whiteStone;
	HWND hwnd;
	bool isMyTurn = false;
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
	//���� ����
	void SendPos(int x, int y);
	void ProcessSocketMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	void ProcessPacket(char* szBuf, int len);

	GameManager();
	~GameManager();
};