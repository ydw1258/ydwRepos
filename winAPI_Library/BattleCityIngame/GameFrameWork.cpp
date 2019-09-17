#include "GameFrameWork.h"
#include"GameManager.h"

GameFrameWork::GameFrameWork()
{
	player.x = 4 * GameManager::GetInstance()->TileImageSizeX + OffsetX;
	player.y = 12 * GameManager::GetInstance()->TileImageSizeY + OffsetY;

	m_LastTime = std::chrono::system_clock::now();
}
GameFrameWork::~GameFrameWork(){}

void GameFrameWork::Init(HWND hWnd, HINSTANCE g_hInst)
{
	m_hWnd = hWnd;

	HDC hdc = GetDC(hWnd);
	ResourceManager::GetInstance()->Init(hdc, g_hInst);
	GameManager::GetInstance()->Init(hdc, g_hInst);

	for (int i = 0; i < 11; i++)
		blockSprites[i].Init((IMAGENUM)(IMAGENUM_BLOCK + i), 1, 16, 16);
	ReleaseDC(hWnd, hdc);

	player.Init(32,32, IMAGENUM_ROW_1_DOWN_00);
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

	GameManager::GetInstance()->MoveBullets();
	OperateInput();
	Render();
}

void GameFrameWork::OperateInput()
{
	if (GetKeyState(VK_SPACE) & 0x8000)
	{
		GameManager::GetInstance()->Shot(player);
	}
	if (GameManager::GetInstance()->CollisionCheck(player))
		return;
	if (GetKeyState(VK_LEFT) & 0x8000)
	{
		player.PlayerMove(LEFT);
		return;
	}
	if (GetKeyState(VK_RIGHT) & 0x8000)
	{
		player.PlayerMove(RIGHT);
		return;
	}
	if (GetKeyState(VK_UP) & 0x8000)
	{
		player.PlayerMove(UP);
		return;
	}
	if (GetKeyState(VK_DOWN) & 0x8000)
	{
		player.PlayerMove(DOWN);
		return;
	}
	
}

void GameFrameWork::Render()
{
	HDC hdc = GetDC(m_hWnd);

	ResourceManager::GetInstance()->Draw(ResourceManager::backBuffer->GetmemDC(), 0, 0, 600, 600, IMAGENUM_BLACKBACKGROUND);
	GameManager::GetInstance()->Draw(ResourceManager::backBuffer->GetmemDC());

	player.Draw(ResourceManager::backBuffer->GetmemDC(), 20, 20);
	//GameManager::GetInstance()->CollisionDraw(player, ResourceManager::backBuffer->GetmemDC());
	GameManager::GetInstance()->DrawBullets(ResourceManager::backBuffer->GetmemDC());
	BitBlt(hdc, 0, 0, 600, 600, ResourceManager::backBuffer->GetmemDC(), 0, 0, SRCCOPY);

	ReleaseDC(m_hWnd, hdc);
}
