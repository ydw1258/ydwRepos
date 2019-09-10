#include "GameFrameWork.h"
#include <math.h>
#include <stdio.h>

GameFrameWork::GameFrameWork()
{
	Player_x = 200.0f;
	Player_y = 500.0f;

	m_bJump = false;

	m_fVectorX = 1.0f;
	m_fVectorY = 0.0f;

	m_LastTime = std::chrono::system_clock::now();
}

GameFrameWork::~GameFrameWork()
{
}

void GameFrameWork::Init(HWND hWnd)
{
	m_hWnd = hWnd;

	HDC hdc = GetDC(hWnd);
	m_hMemDC[0] = CreateCompatibleDC(hdc);
	m_hBitmap[0] = CreateCompatibleBitmap(hdc, 1024, 768);
	m_hOld[0] = (HBITMAP)SelectObject(m_hMemDC[0], m_hBitmap[0]);

	m_hMemDC[1] = CreateCompatibleDC(m_hMemDC[0]);
	m_hBitmap[1] = (HBITMAP)LoadImage(NULL, "back.bmp", IMAGE_BITMAP, 0, 0
		, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
	m_hOld[1] = (HBITMAP)SelectObject(m_hMemDC[1], m_hBitmap[1]);

	m_hMemDC[2] = CreateCompatibleDC(m_hMemDC[0]);
	m_hBitmap[2] = (HBITMAP)LoadImage(NULL, "char.bmp", IMAGE_BITMAP, 0, 0
		, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
	m_hOld[2] = (HBITMAP)SelectObject(m_hMemDC[2], m_hBitmap[2]);
	ReleaseDC(hWnd, hdc);
}


void GameFrameWork::Release()
{
	for (int i = 0; i < 3; i++)
	{
		SelectObject(m_hMemDC[i], m_hOld[i]);
		DeleteObject(m_hBitmap[i]);
		DeleteDC(m_hMemDC[i]);
	}

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
		Player_x -= 500 * m_fElapseTime;
	if (GetKeyState(VK_RIGHT) & 0x8000)
		Player_x += 500 * m_fElapseTime;
	if (GetKeyState(VK_UP) & 0x8000)
		Player_y -= 500 * m_fElapseTime;
	if (GetKeyState(VK_DOWN) & 0x8000)
		Player_y += 500 * m_fElapseTime;

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

	BitBlt(m_hMemDC[0], 0, 0, 1024, 768, m_hMemDC[1], 0, 0, SRCCOPY);
	TransparentBlt(m_hMemDC[0], (int)Player_x, (int)Player_y, 240, 192, m_hMemDC[2], 0, 0, 240, 192, RGB(255, 0, 255));

	BitBlt(hdc, 0, 0, 1024, 768, m_hMemDC[0], 0, 0, SRCCOPY);

	ReleaseDC(m_hWnd, hdc);
}
