#include "GameFrameWork.h"
#include"GameManager.h"
#include <math.h>
#include <stdio.h>

GameFrameWork::GameFrameWork()
{
	player.x = 200.0f;
	player.y = 500.0f;

	m_fVectorX = 1.0f;
	m_fVectorY = 0.0f;

	m_LastTime = std::chrono::system_clock::now();
}

GameFrameWork::~GameFrameWork(){}

void GameFrameWork::Init(HWND hWnd, HINSTANCE g_hInst)
{
	m_hWnd = hWnd;

	HDC hdc = GetDC(hWnd);
	ResourceManager::GetInstance()->Init(hdc, g_hInst);

	for (int i = 0; i < 11; i++)
		blockSprites[i].Init((IMAGENUM)(IMAGENUM__BLOCK + i), 1, 16, 16);
	ReleaseDC(hWnd, hdc);

	player.Init(64,64, IMAGENUM_ROW_1_DOWN_00);
}


void GameFrameWork::Release()
{
	ResourceManager::GetInstance()->Release();
}

#define PI 3.141592f

void GameFrameWork::Update()
{
	if (GetKeyState(VK_LBUTTON) & 0x8000)
	{
		POINT pt;
		GetCursorPos(&pt);

		ScreenToClient(m_hWnd, &pt);
	}

	std::chrono::duration<float> sec = std::chrono::system_clock::now() - m_LastTime;
	/*if (sec.count() < (1 / FPS))
		return;*/

	m_fElapseTime = sec.count();
	m_LastTime = std::chrono::system_clock::now();

	OperateInput();

	Render();
}

void GameFrameWork::OperateInput()
{
	if (GetKeyState(VK_LEFT) & 0x8000)
	{

	}
	if (GetKeyState(VK_RIGHT) & 0x8000)
	{

	}
	if (GetKeyState(VK_UP) & 0x8000)
	{

	}
	if (GetKeyState(VK_DOWN) & 0x8000)
	{

	}
	if (GetKeyState(VK_SPACE) & 0x8000)
	{
		if (m_bJump == false)
		{
			// 키 다운
			m_bJump = true;

		}
		else
		{
			//누르고 있을때
		}
	}
	else
	{
		if (m_bJump)
		{
			//키 업
		}
		m_bJump = false;

	}

}

void GameFrameWork::Render()
{
	HDC hdc = GetDC(m_hWnd);

	ResourceManager::GetInstance()->Draw(ResourceManager::backBuffer->GetmemDC(), 0, 0, 600, 600, IMAGENUM_BLACKBACKGROUND);
	
	for (int i = 0; i < 11; i++)
	{
		for (int j = 0; j < 4; j++);
		{
			blockSprites[i].DrawObject(ResourceManager::backBuffer->GetmemDC(), i * 16, 10);
		}
		
	}
	BitBlt(hdc, 0, 0, 600, 600, ResourceManager::backBuffer->GetmemDC(), 0, 0, SRCCOPY);
	player
	ReleaseDC(m_hWnd, hdc);
}
