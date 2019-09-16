#include "GameFrameWork.h"
#include"GameManager.h"


GameFrameWork::GameFrameWork()
{
	player.x = 200.0f;
	player.y = 500.0f;

	m_LastTime = std::chrono::system_clock::now();
}

GameFrameWork::~GameFrameWork(){}

void GameFrameWork::Init(HWND hWnd, HINSTANCE g_hInst)
{
	m_hWnd = hWnd;

	HDC hdc = GetDC(hWnd);
	ResourceManager::GetInstance()->Init(hdc, g_hInst);

	for (int i = 0; i < 11; i++)
		blockSprites[i].Init((IMAGENUM)(IMAGENUM_BLOCK + i), 1, 16, 16);
	ReleaseDC(hWnd, hdc);

	player.Init(16,16, IMAGENUM_ROW_1_DOWN_00);
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
}

void GameFrameWork::Render()
{
	HDC hdc = GetDC(m_hWnd);

	ResourceManager::GetInstance()->Draw(ResourceManager::backBuffer->GetmemDC(), 0, 0, 600, 600, IMAGENUM_BLACKBACKGROUND);
	
	BitBlt(hdc, 0, 0, 600, 600, ResourceManager::backBuffer->GetmemDC(), 0, 0, SRCCOPY);
	player.Draw(hdc, 20, 20);

	/* */
	for(int i = 0 ; i < 15;i++)
		ResourceManager::GetInstance()->Draw(hdc,10, 10 + i*20, 32, 32, (IMAGENUM)(IMAGENUM_EMPTY + i));

	ReleaseDC(m_hWnd, hdc);
}
