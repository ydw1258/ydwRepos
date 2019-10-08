#pragma once
#include <map>
#include<vector>
#include<list>
#include <string>
#include<iostream>
#include "SpriteRenderer.h"
#include "FontManager.h"
#include "UIButton.h"
#include "IMAGENUM.h"
#include "Physics.h"
#include "SHA1.h"
#include "PacketManager.h"
#include "..\..\Common\PACKET_HEADER.h"

using namespace std;


class GameManager
{
private:
	static GameManager* mthis;
	//map<int, int> mapRoomPlayers; //방번호, 플레이어 숫자

	//Lobby
	list<UIButton> roomButtons;
	UIButton gameExitButton;
	
	//Room
	UIButton startButton;
	UIButton roomExitButton;

	HWND chatInputBoxLobby;
	HWND chatInputBoxInRoom;
	HWND LOGINInput[2];
	SpriteRenderer blueboard;
public:
	//UI
	list<string> chatList;
	list<string> listPlayerID;
	list<DRAWPT> mousepointList;
	SERVERMANAGER_SCENE scene = LOGIN;
	SpriteRenderer memoImage;
	SpriteRenderer lobbybackground;
	SpriteRenderer ButtonImage;
	FontManager playerInfoFont;
	FontManager ChattingFont;
	HINSTANCE hInstance;
	HWND hwnd;
	RECT whiteBoard;

	static GameManager* GetInstance()
	{
		if (mthis == nullptr)
			mthis = new GameManager();
		return mthis;
	}
	void Init(HDC hdc, HINSTANCE hInstance, HWND _hwnd);
	void Draw(HDC hdc);
	void MouseButtonCheckInRoom(POINT pt);

	void DrawChatWindow(HDC hdc);
	void DrawCurUsers(HDC hdc);
	void DrawRooms(HDC hdc);
	void MouseDrawInGame(HDC hdc, POINT pt);
	void Login();
	//서버 관련
	
	void InputChatting(void);
	void SceneInitiator();
	void SceneChange(SERVERMANAGER_SCENE _scene);
	void EnterTheRoom(POINT pt);
	void ExitTheRoom(POINT pt);
	void GameStart(POINT pt);
	void GameExit(POINT pt);

	void RoomButtonUpdate();
};