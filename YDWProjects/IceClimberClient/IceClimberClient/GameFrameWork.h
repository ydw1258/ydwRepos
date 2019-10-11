#pragma once
#include <chrono>
#include <math.h>
#include <stdio.h>
#include "IMAGENUM.h"
#include <Windows.h>
#include "GameManager.h"

using namespace std;

#define FPS			60.0f

class GameFrameWork
{
private:
	HDC memDC;
	HWND		m_hWnd;
	chrono::system_clock::time_point m_LastTime;
	float		m_fElapseTime;

	int OffsetX = 20;
	int OffsetY = 20;

	int mapTile[13 * 13];
	bool isLButtonDown = false;
	bool isKeydown = false;
public:
	GameFrameWork();
	~GameFrameWork();

	void Init(HWND hWnd, HINSTANCE g_hInst);
	void Release();
	void Update();
	void OperateInput();
	void Render();
};

