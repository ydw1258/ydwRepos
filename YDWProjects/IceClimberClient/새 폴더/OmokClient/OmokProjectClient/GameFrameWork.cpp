#include "GameFrameWork.h"
#include"GameManager.h"

GameFrameWork::GameFrameWork()
{
	m_LastTime = std::chrono::system_clock::now();
}
GameFrameWork::~GameFrameWork() {}

void GameFrameWork::Init(HWND hWnd, HINSTANCE g_hInst)
{
	m_hWnd = hWnd;
	HDC hdc = GetDC(hWnd);
	GameManager::GetInstance()->Init(hdc, g_hInst, hWnd);
	
	ReleaseDC(hWnd, hdc);
}

void GameFrameWork::Release(){}

#define PI 3.141592f

void GameFrameWork::Update()
{
	if (GetKeyState(VK_LBUTTON) & 0x8000)
	{
		if (!isLButtonDown)
		{
			isLButtonDown = true;
			POINT pt;
			GetCursorPos(&pt);
			ScreenToClient(m_hWnd, &pt);

			HDC hdc = GetDC(m_hWnd);

			//GameManager::GetInstance()->blackStone.DrawObject(hdc, pt.x, pt.y);
			switch (GameManager::GetInstance()->scene)
			{
			case LOBBY:
				GameManager::GetInstance()->EnterTheRoom(pt);
				GameManager::GetInstance()->GameExit(pt);
				break;
			case INGAME:
				GameManager::GetInstance()->MouseButtonCheckIngame(pt);
				break;
			}
						
			//GameManager::GetInstance()->
			ReleaseDC(m_hWnd, hdc);
		}
	}
	else
	{
		isLButtonDown = false;
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
	if (GetKeyState(VK_RETURN) & 0x8000)
	{
		if (!isKeydown)
		{
			switch (GameManager::GetInstance()->scene)
			{
			case LOGIN:
				GameManager::GetInstance()->Login();
				break;
			case LOBBY:
			case INGAME:
				GameManager::GetInstance()->InputChatting();
				break;
			}
			
			isKeydown = true;
		}
	}
	else
	{
		if (isKeydown)
			isKeydown = false;
	}
}

void GameFrameWork::Render()
{
	HDC hdc = GetDC(m_hWnd);

	GameManager::GetInstance()->Draw(ResourceManager::backBuffer->GetmemDC());
	//GameManager::GetInstance()->DrawRect(ResourceManager::backBuffer->GetmemDC());
	BitBlt(hdc, 0, 0, 1000, 800, ResourceManager::backBuffer->GetmemDC(), 0, 0, SRCCOPY);

	ReleaseDC(m_hWnd, hdc);
}