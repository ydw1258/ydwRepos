#pragma once
#include <Scene.h>
#include <string>

#include "../../CatchMind/CatchMind/PACKET.h"

using namespace std;

class BitMap;
class LoginScene : public Scene
{
private:
	BitMap* bg;

	HWND idBox = nullptr;
	HWND pwBox = nullptr;

	bool returnDown = false;
	bool isTabDown = false;

	string idString;

	LOGIN_RESULT loginResult = LR_SUCCESS;

public:
	LoginScene();
	~LoginScene();

	virtual int Init(HWND hWnd, HINSTANCE hInstance);
	virtual void Update(float deltatime);
	virtual void Input();
	virtual void ProcessPacket(char * szBuf, int len);
	virtual void Render(HDC hdc);
	virtual void Release();

private:
	void SendTryLogInPacket(string id, string pw);
};

