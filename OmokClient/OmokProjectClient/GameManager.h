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

struct USER_INFO_STRING
{
	char szBuf[BUFSIZE];
	int len;
};
class GameManager
{
private:
	static GameManager* mthis;
	//map<int, POINT> ptData; //플레이어, 전송데이터
	map<int, int> mapRoomPlayers; //방번호, 플레이어 숫자
	//int g_iIndex = 3;
	//서버관련
	char playerID[128];
	int playerIndex= 3;
	int userIndexInRoom;
	bool isGameStart = false;
	int roomIndex;
	SOCKET g_sock;
	WSADATA wsa;
	SOCKADDR_IN serveraddr;
	HWND chatInputBoxLobby;
	HWND chatInputBoxIngame;
	HWND LOGINInput[2];
	list<string> chatList;
	list<string> listPlayerID;
	list<RECT> roomButtons;
	RECT roomExitButton;
	RECT startButton;
	RECT gameExitButton;
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
	void MouseButtonCheckIngame(POINT pt);
	void GameOverCheck();

	void DrawChatWindow(HDC hdc);
	void DrawCurUsers(HDC hdc);
	void DrawRooms(HDC hdc);

	void Login();
	//서버 관련
	void SendPos(int x, int y);
	void ProcessSocketMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	bool ProcessPacket(char* szBuf, USER_INFO_STRING& userinfo, int len);
	void InitConnection();
	void BoardDraw(HDC hdc);
	void InputChatting(void);
	void SceneInitiator();
	void SceneChange(Scene _scene);
	void GetPlayersInRoom(int roomNum); //로비포함 같은 방에 있는 사람 불러오기
	void EnterTheRoom(POINT pt);
	void ExitTheRoom(POINT pt);
	void GameStart(POINT pt);
	void GameExit(POINT pt);
	void GetRooms();
	void BoardInputCheck(POINT pt);

	GameManager();
	~GameManager();
};