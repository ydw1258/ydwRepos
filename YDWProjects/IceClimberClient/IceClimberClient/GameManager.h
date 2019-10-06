#pragma once
#include <map>
#include<vector>
#include<list>
#include "SpriteRenderer.h"
#include "FontManager.h"
#include "PacketManager.h"
#include "UIButton.h"

using namespace std;

struct USER_INFO_STRING
{
	char szBuf[BUFSIZE];
	int len;
};
class GameManager
{
private:
	static GameManager* mthis;
	map<int, int> mapRoomPlayers; //���ȣ, �÷��̾� ����

	//UI
	list<string> chatList;
	list<string> listPlayerID;
	
	//Lobby
	list<UIButton> roomButtons;
	UIButton gameExitButton;

	//Room
	UIButton startButton;
	UIButton roomExitButton;

	HWND chatInputBoxLobby;
	HWND chatInputBoxIngame;
	HWND LOGINInput[2];
	SpriteRenderer blueboard;
public:
	SERVERMANAGER_SCENE scene = LOGIN;
	SpriteRenderer memoImage;
	SpriteRenderer lobbybackground;
	SpriteRenderer ButtonImage;
	FontManager playerInfoFont;
	FontManager ChattingFont;
	HINSTANCE hInstance;
	HWND hwnd;

	static GameManager* GetInstance()
	{
		if (mthis == nullptr)
			mthis = new GameManager();
		return mthis;
	}
	void Init(HDC hdc, HINSTANCE hInstance, HWND _hwnd);
	void Draw(HDC hdc);
	void DrawRect(HDC hdc);
	void MouseButtonCheckIngame(POINT pt);

	void DrawChatWindow(HDC hdc);
	void DrawCurUsers(HDC hdc);
	void DrawRooms(HDC hdc);

	void Login();
	//���� ����
	void SendPos(int x, int y);
	
	
	void InitConnection();
	void BoardDraw(HDC hdc);
	void InputChatting(void);
	void SceneInitiator();
	void SceneChange(SERVERMANAGER_SCENE _scene);
	void GetPlayersInRoom(int roomNum); //�κ����� ���� �濡 �ִ� ��� �ҷ�����
	void EnterTheRoom(POINT pt);
	void ExitTheRoom(POINT pt);
	void GameStart(POINT pt);
	void GameExit(POINT pt);
	void GetRooms();

	GameManager();
	~GameManager();
};