#include "MainGame.h"
#include "macro.h"
#include "ResManager.h"

#include "MineButton.h"

MainGame* MainGame::m_sThis = nullptr;

MainGame::MainGame()
{
	m_eState = GAME_PLAY;
}

MainGame::~MainGame()
{
}

void MainGame::Init(HWND hWnd, HDC hdc, HINSTANCE hInst)
{
	m_hWnd = hWnd;
	srand(GetTickCount());

	ResManager::inst()->Init(hdc, hInst, TEXT("block.bmp"));
	ResManager::inst()->Init(hdc, hInst, TEXT("block_0.bmp"));
	ResManager::inst()->Init(hdc, hInst, TEXT("block_1.bmp"));
	ResManager::inst()->Init(hdc, hInst, TEXT("block_2.bmp"));
	ResManager::inst()->Init(hdc, hInst, TEXT("block_3.bmp"));
	ResManager::inst()->Init(hdc, hInst, TEXT("block_4.bmp"));
	ResManager::inst()->Init(hdc, hInst, TEXT("block_5.bmp"));
	ResManager::inst()->Init(hdc, hInst, TEXT("block_6.bmp"));
	ResManager::inst()->Init(hdc, hInst, TEXT("block_7.bmp"));
	ResManager::inst()->Init(hdc, hInst, TEXT("block_8.bmp"));
	ResManager::inst()->Init(hdc, hInst, TEXT("flag.bmp"));
	ResManager::inst()->Init(hdc, hInst, TEXT("mine.bmp"));
	ResManager::inst()->Init(hdc, hInst, TEXT("back.bmp"));

	for (int y = 0; y < 10; y++)
	{
		for (int x = 0; x < 10; x++)
		{
			MineButton* NBTn = new MineButton;
			NBTn->Init(x, y);
			vMineButton.push_back(NBTn);
		}
	}
}

void MainGame::Update()
{
	if (m_eState != GAME_PLAY)
		return;

}

void MainGame::Input(POINT pt)
{
	for (size_t i = 0; i < vMineButton.size(); i++)
	{
		vMineButton[i]->Input(pt);
	}
}

void MainGame::Draw(HDC hdc)
{
	for (auto iter = vMineButton.begin(); iter != vMineButton.end(); iter++)
	{
		(*iter)->Draw(hdc);
	}
}

void MainGame::Release()
{
	/*
	
	for (auto iter = m_vecCard.begin(); iter != m_vecCard.end(); iter++)
	{
		SAFE_DELETE(*iter);
	}
	m_vecCard.clear();
	*/
	
	SAFE_DELETE(m_sThis);
}
