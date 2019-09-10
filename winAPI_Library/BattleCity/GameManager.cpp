#include "GameManager.h"
#include<iostream>
using namespace std;

GameManager* GameManager::mthis = nullptr;

enum TIMERID
{
	TIMER_TITLESPRITECHAGE,
	TIMER_MAKEFIRE,
	TIMER_SPRITECHANGE,
	TIMER_GAMEOVERTIMER
};

void GameManager::Init(HDC hdc, HINSTANCE _g_hInst)
{
	for (int i = 0; i < 10; i++)
		Timer[i] = 0;
	//player.Init(3); // playerSpeed
	TitleFont.Init();
	ResourceManager::GetInstance()->Init(hdc, _g_hInst);
	SetTimers();
	for (int i = 0; i < 14; i++)
		blocks[i].Init(16, 16, (IMAGENUM)(IMAGENUM__BLOCK + i));
}
void GameManager::Input(WPARAM wParam, LPARAM lParam, int radioButtonCheckNum)
{
	CheckTileInput(LOWORD(lParam), HIWORD(lParam), radioButtonCheckNum);
}
void GameManager::CheckTileInput(int x, int y, int radioButtonCheckNum)
{
	RECT rect;
	POINT pt;
	pt.x = x;
	pt.y = y;


	for (int i = 0; i < TILE_HEIGHT_NUM; i++)
	{
		for (int j = 0; j < TILE_WIDTH_NUM; j++)
		{
			rect.top = i * TileImageSizeY + GameOffsetY;
			rect.bottom = (i + 1) * TileImageSizeY + GameOffsetY;
			rect.left = j * TileImageSizeX + GameOffsetX;
			rect.right = (j + 1) * TileImageSizeX  + GameOffsetX;

			//cout << rect.top << " " << rect.bottom << " " << rect.left << " " << rect.right << endl;
			if (Physics::GetInstance()->RECTbyPointCollisionCheck(rect, pt))
			{
				//라디오 버튼 클릭된 것
				mapTile[i * TILE_WIDTH_NUM + j] = radioButtonCheckNum;
					//SendMessage(r1, BM_GETCHECK, 0, 0) == BST_CHECKED) GRAPH = 0;
			}
		}
	}
	
}
void GameManager::SetTimers()
{
	TimerReset[TIMER_MAKEFIRE] = 5.0f;
	TimerReset[TIMER_SPRITECHANGE] = 0.4f;
	TimerReset[TIMER_TITLESPRITECHAGE] = 0.5f;
	TimerReset[TIMER_GAMEOVERTIMER] = 2.0f;
}
//Update 매프레임 호출
void GameManager::Update()
{
	Physics::GetInstance()->deltaTimeInit();
	EditModeUpdate();
}
void GameManager::InTitleUpdate()
{
	Timer[TIMER_TITLESPRITECHAGE] += Physics::GetInstance()->deltaTime;

	if (Timer[TIMER_TITLESPRITECHAGE] >= TimerReset[TIMER_TITLESPRITECHAGE])
	{
		Timer[TIMER_TITLESPRITECHAGE] = 0;
	}
}
void GameManager::EditModeUpdate()
{}

//Draw 매프레임 호출
void GameManager::Draw(HDC hdc)
{
	BitBlt(hdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, ResourceManager::backBuffer->GetmemDC(), 0, 0, SRCCOPY);
	ResourceManager::GetInstance()->Draw(hdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, IMAGENUM_BLACKBACKGROUND);
	
	for (int i = 0; i < 14; i++)
	{
		blocks[i].Draw(hdc, GameOffsetX, i * TileImageSizeY+ GameOffsetY);
	}

	for (int i = 0; i < TILE_HEIGHT_NUM; i++)
	{
		for (int j = 0; j < TILE_WIDTH_NUM; j++)
		{
			blocks[mapTile[i * TILE_HEIGHT_NUM + j]].Draw(hdc, j * TileImageSizeX + GameOffsetX, i * TileImageSizeY + GameOffsetY);
			
		}
	}
}

void GameManager::DrawBlack()
{
	HBRUSH myBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
	myBrush = CreateSolidBrush(RGB(0, 0, 0));
	HBRUSH oldBrush = (HBRUSH)SelectObject(ResourceManager::GetInstance()->backBuffer->GetmemDC(), myBrush);
	Rectangle(ResourceManager::GetInstance()->backBuffer->GetmemDC(), 0, 0, 1024, 740);
	SelectObject(ResourceManager::GetInstance()->backBuffer->GetmemDC(), oldBrush);
	DeleteObject(myBrush);
}
GameManager::GameManager() {}
GameManager::~GameManager() {}