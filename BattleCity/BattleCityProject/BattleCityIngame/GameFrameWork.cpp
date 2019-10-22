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
	bitmap = CreateCompatibleBitmap(hdc, 800, 700);
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
	if (sec.count() < (1 / FPS))
		return;
	
	m_fElapseTime = sec.count();
	m_LastTime = std::chrono::system_clock::now();

	GameManager::GetInstance()->Update(m_fElapseTime);
	
	OperateInput();
	Render();
}

void GameFrameWork::OperateInput()
{
	//1 / m_fElapseTime // => FPS
	if (GetKeyState(VK_SPACE) & 0x8000)
	{
		if (!isButtonDown)
		{
			isButtonDown = true;
			switch (GameManager::GetInstance()->scene)
			{
			case TITLE:
				GameManager::GetInstance()->SceneChange(INGAME, 1);
				break;
			case INGAME:
				GameManager::GetInstance()->Shot();
				//GameManager::GetInstance()->SceneChange(INGAME, 1);

				break;
			}
		}
		else if(isButtonDown)
		{
			isButtonDown = false;
		}
	}
	if (GameManager::GetInstance()->CollisionCheck(GameManager::GetInstance()->player.x, GameManager::GetInstance()->player.y, GameManager::GetInstance()->player.direction))
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
	if (GetKeyState(VK_SHIFT) & 0x8000)
	{
		if (GameManager::GetInstance()->isCollisionView = true)
		{
			GameManager::GetInstance()->isCollisionView = false;
		}
		else
		{
			GameManager::GetInstance()->isCollisionView = true;
		}
	}
}

void GameFrameWork::Render()
{
	//��������Ʈ ������ �����ð����ٸ� ȣ��
	GameManager::GetInstance()->Draw(memDC);
	
	BitBlt(hdc, 0, 0, 1000, 1000, memDC, 0, 0, SRCCOPY);
}