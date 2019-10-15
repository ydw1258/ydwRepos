#pragma once
#include <Windows.h>

class SceneMgr;
class MainGame
{
public:
	MainGame();
	~MainGame();

	void Init(HWND hWnd, HDC hdc, HINSTANCE hInstance);
	void Update();
	void Input(WPARAM wParam, LPARAM lParam, int btnIdx, bool up);
	void Render();
	void Release();
};

