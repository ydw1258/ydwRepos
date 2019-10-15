#pragma once
#include <Windows.h>
#include <iostream>
using namespace std;

class Scene
{
protected:
	HWND hWnd;
	HINSTANCE hInstance;
	bool isInit = false;

public:
	Scene();
	virtual ~Scene();

	virtual int Init(HWND hWnd, HINSTANCE hInstance);
	virtual void Input();
	virtual void Input(UINT iMessage, WPARAM wParam, LPARAM lParam);
	virtual void Render(HDC hdc);
	virtual void Update(float deltatime);
	virtual void ProcessPacket(char * szBuf, int len);
	virtual void Release();
};

