#pragma once
#include <Windows.h>
#include <chrono>
#include <math.h>
#include <stdio.h>
#include "ResourceManager.h"
#include "Player.h"

using namespace std;

#define FPS			60.0f
#define PI 3.141592f

class GameFrameWork
{
private:
	HWND		m_hWnd;
	HDC hdc;
	HDC         memDC;
	HBITMAP      bitmap;
	HBITMAP      oldBitmap;

	chrono::system_clock::time_point m_LastTime;
	float		m_fElapseTime;
	
	int OffsetX = 20;
	int OffsetY = 20;

	int mapTile[13 * 13];
	bool isLButtonDown = false;
public:
	GameFrameWork();
	~GameFrameWork();

	void Init(HWND hWnd, HINSTANCE g_hInst);
	void Release();
	void Update();
	void OperateInput();
	void Render();
};