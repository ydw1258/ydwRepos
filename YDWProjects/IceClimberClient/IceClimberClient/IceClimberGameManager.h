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
#define TIME_LIMIT 30
#define PENCOLORNUM 17
class GameManager
{
private:
	static GameManager* mthis;
	//map<int, int> mapRoomPlayers; //방번호, 플레이어 숫자
	float remainTime = TIME_LIMIT;
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
	
	HPEN pen, oPen;
	PenInfo penInfo[PENCOLORNUM]; //rgb만 사용
	vector<RECT> lcolorPen;
	//17개
	/*blackPen;
	whitePen;
	redPen;
	bluePen;
	greenPen;
	yellowPen;
	NavyPen;
	darkCyanPen;
	maroonPen;
	purplePen;
	olivePen;
	lightGreyPen;
	darkGreyPen;
	cyanPen;
	darkGreenPen;
	orangePen;
	greenYellowPen;
	*/
public:
	PenInfo curPenInfo;
	BitMap whiteBoardImage;
	//UI
	list<string> chatList;
	list<string> listPlayerID;
	//list<DRAWPT> mousepointList;
	SERVERMANAGER_SCENE scene = LOGIN;
	SpriteRenderer memoImage;
	SpriteRenderer lobbybackground;
	SpriteRenderer ButtonImage;
	FontManager playerInfoFont;
	FontManager ChattingFont;
	HINSTANCE hInstance;
	HWND hwnd;
	RECT whiteBoard;

	
	UIButton DumpAll; //전체를 펜 색상으로 칠하기
	
	void whiteBoardDraw(DRAWPT drawPt, PenInfo penInfo);

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
	void Login();
	//서버 관련
	
	void InputChatting(void);
	void SceneInitiator();
	void SceneChange(SERVERMANAGER_SCENE _scene);
	void EnterTheRoom(POINT pt);
	void ExitTheRoom(POINT pt);
	void GameStart(POINT pt);
	void GameExit(POINT pt);
	void GameOver();
	void DrawPenColorsButton(HDC hdc);
	void RoomButtonUpdate();
	void PenInit();
	void PenColorButtonClickedCheck(POINT pt);
	void TimeCheck(float deltaTime);
	void TimeOver();
	
};