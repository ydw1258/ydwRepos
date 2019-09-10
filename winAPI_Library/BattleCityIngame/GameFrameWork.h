#pragma once
#include <Windows.h>
#include <chrono>
#include"ResourceManager.h"
#include"SpriteRenderer.h"
#include "Player.h"
using namespace std;

#define FPS			60.0f

enum STATE
{
	IDLE,
	MOVE,
	JUMP
};

class GameFrameWork
{
private:
	HWND		m_hWnd;
	chrono::system_clock::time_point m_LastTime;
	float		m_fElapseTime;
	SpriteRenderer blockSprites[11];

	Player player;
	
	//수학적으로
	STATE		m_eState;
	//물리적
	float		m_fVectorX;
	float		m_fVectorY;

	int mapTile[13 * 13];
public:
	GameFrameWork();
	~GameFrameWork();

	void Init(HWND hWnd, HINSTANCE g_hInst);
	void Release();
	void Update();
	void OperateInput();
	void Render();
};

