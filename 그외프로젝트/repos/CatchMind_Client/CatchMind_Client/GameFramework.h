#pragma once
#include <Windows.h>
#include <chrono>
#include <vector>

#include "ENUMHEADER.h"
#include "../../CatchMind/CatchMind/PACKET.h"

using namespace std;

#define FPS	600.0f
#define PI 3.141592f

class Player;
class GameFrameWork
{
private:
	static GameFrameWork* pThis;

public:
	static GameFrameWork* GetInstance()
	{
		if (pThis == nullptr)
		{
			pThis = new GameFrameWork();
		}

		return pThis;
	}

private:
	HWND		hWnd;
	chrono::system_clock::time_point lastTime;
	float		elapseTime;

	HDC			memDC;
	HBITMAP		bitmap;
	HBITMAP		oldBitmap;

	bool isGameRunning = true;

	char packetBuf[1024];
	int myLen = 0;

	vector<Player*> userVec;

	SOCKET mySock;
	char myId[IDPWSIZE] = "DEBUG";
	PlayerType playerType = PT_NULL;
	int myRoom = -1;

public:
	bool GetIsGameRunning() { return isGameRunning; }
	void StopGame() { isGameRunning = false; }

	SOCKET GetMySocket() { return mySock; }
	char* GetMyId() { return myId; }
	void SetMyId(const char* value) { memcpy(&myId, value, IDPWSIZE); }
	PlayerType GetMyType() { return playerType; }
	void SetMyType(PlayerType type) { playerType = type; }
	int GetMyRoom() { return myRoom; }
	void SetMyRoom(int i) { myRoom = i; }

public:
	GameFrameWork();
	~GameFrameWork();

	void Init(HWND hWnd, HINSTANCE hInstance, SOCKET sock);
	void Release();
	void Update();
	void Render();

	void ProcessPacket(char * szBuf, int len);
	void ProcessPacketBuf();

	void PrintUserList(HDC hdc);

private:
	void LoadResource();
};