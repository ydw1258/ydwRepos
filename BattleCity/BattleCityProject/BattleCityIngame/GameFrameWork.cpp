#include "GameFrameWork.h"
#include"GameManager.h"

GameFrameWork::GameFrameWork()
{
	m_LastTime = std::chrono::system_clock::now();
}
GameFrameWork::~GameFrameWork(){}

void GameFrameWork::Init(HWND hWnd, HINSTANCE g_hInst)
{
	m_hWnd = hWnd;
	hdc = GetDC(hWnd);
	memDC = CreateCompatibleDC(hdc);
	bitmap = CreateCompatibleBitmap(hdc, 550, 500);
	oldBitmap = (HBITMAP)SelectObject(memDC, bitmap);

	GameManager::GetInstance()->Init(hdc, g_hInst, hWnd);
}

void GameFrameWork::Release()
{
	ReleaseDC(m_hWnd, hdc);
	GameManager::GetInstance()->Release();
}

void GameFrameWork::Update()
{
	/*if (GetKeyState(VK_LBUTTON) & 0x8000)
	{
		POINT pt;
		GetCursorPos(&pt);
		ScreenToClient(m_hWnd, &pt);
	}*/
	std::chrono::duration<float> sec = std::chrono::system_clock::now() - m_LastTime;
	/*if (sec.count() < (1 / FPS))
		return;*/

	m_fElapseTime = sec.count();
	m_LastTime = std::chrono::system_clock::now();

	GameManager::GetInstance()->Update(m_fElapseTime);
	
	OperateInput();
	Render();
}

void GameFrameWork::OperateInput()
{
	if (GetKeyState(VK_SPACE) & 0x8000)
	{
		if (!isLButtonDown)
		{
			isLButtonDown = true;
			switch (GameManager::GetInstance()->scene)
			{
			case TITLE:
				GameManager::GetInstance()->SceneChange(INGAME, 1);
				break;
			case INGAME:
				//GameManager::GetInstance()->Shot();
				//GameManager::GetInstance()->SceneChange(INGAME, 1);

				break;
			}
		}
		else
		{
			isLButtonDown = false;
		}
	}
	if (GameManager::GetInstance()->CollisionCheck())
		return;
	if (GetKeyState(VK_LEFT) & 0x8000)
	{
		GameManager::GetInstance()->PlayerMove(LEFT, m_fElapseTime);
		return;
	}
	if (GetKeyState(VK_RIGHT) & 0x8000)
	{
		GameManager::GetInstance()->PlayerMove(RIGHT, m_fElapseTime);
		return;
	}
	if (GetKeyState(VK_UP) & 0x8000)
	{
		GameManager::GetInstance()->PlayerMove(UP, m_fElapseTime);
		return;
	}
	if (GetKeyState(VK_DOWN) & 0x8000)
	{
		GameManager::GetInstance()->PlayerMove(DOWN, m_fElapseTime);
		return;
	}
}

void GameFrameWork::Render()
{
	//스프라이트 변경은 일정시간마다만 호출
	GameManager::GetInstance()->Draw(memDC);
	BitBlt(hdc, 0, 0, 1000, 600, memDC, 0, 0, SRCCOPY);
}
