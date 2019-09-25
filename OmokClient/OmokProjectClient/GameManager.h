#pragma once
#include <map>
#include<vector>
#include<list>
#include "SpriteRenderer.h"
#include "FontManager.h"
#include "..\Common\PACKET_HEADER.h"

using namespace std;

#define WIDTH 19
#define HEIGHT 19
#define WM_SOCKET (WM_USER+1)
#define BUFSIZE 1024

class GameManager
{
private:
	static GameManager* mthis;
	map<int, POINT> ptData; //�÷��̾�, ���۵�����
	
	//int g_iIndex = 3;
	//��������
	char playerID[128];
	int playerIndex= 3;
	SOCKET g_sock;
	WSADATA wsa;
	SOCKADDR_IN serveraddr;
	HWND chatInputBox;
	HWND LOGINInput[2];
	list<string> chatList;
	list<string> listPlayerID;
public:
	Scene scene = LOGIN;
	SpriteRenderer board;
	SpriteRenderer blackStone;
	SpriteRenderer whiteStone;
	SpriteRenderer UIbutton;
	SpriteRenderer memoImage;
	SpriteRenderer lobbybackground;
	SpriteRenderer blueBoard;
	FontManager font;
	HINSTANCE hInstance;
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

	void DrawChatWindow(HDC hdc);
	void DrawCurUsers(HDC hdc);
	void DrawRooms(HDC hdc);

	void Login();
	//���� ����
	void SendPos(int x, int y);
	void ProcessSocketMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	void ProcessPacket(char* szBuf, int len);
	void InitConnection();
	void InputChatting(void);
	void SceneInitiator();
	void SceneChange(Scene _scene);
	void GetPeopleInRoom(int roomNum); //�κ����� ���� �濡 �ִ� ��� �ҷ�����

	GameManager();
	~GameManager();
};