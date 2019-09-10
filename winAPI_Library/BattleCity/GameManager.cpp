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
	player.Init(33, 26, IMAGE_NUM_PLAYERTANK_UP_00);

	for (int i = 0; i < 10; i++)
		Timer[i] = 0;
	//player.Init(3); // playerSpeed
	TitleFont.Init();
	ResourceManager::GetInstance()->Init(hdc, _g_hInst);
	SetTimers();
	for (int i = 0; i < 14; i++)
		blocks[i].Init(33, 25, (IMAGENUM)(IMAGENUM_BLOCK00 + i));
}
void GameManager::Input(WPARAM wParam)
{
	switch (scene)
	{
	case TITLE:
		break;
	case EDIT_MODE:
		switch (wParam)
		{
		case VK_LEFT:
			player.PlayerMove(LEFT);
			return;
		case VK_RIGHT:
			player.PlayerMove(RIGHT);
			return;
		case VK_UP:
			player.PlayerMove(UP);
			return;
		case VK_DOWN:
			player.PlayerMove(DOWN);
			return;
		case VK_SPACE:
			
			return;
		}
		break;
	default:
		break;
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

	switch (scene)
	{
	case TITLE:
		InTitleUpdate();
		break;
	case EDIT_MODE:
		EditModeUpdate();
		break;
	default:
		break;
	}
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
	EditModeDraw(hdc);
}
void GameManager::DrawTitle(HDC hdc)
{
	BitBlt(hdc, 0, 0, 1024, 720, ResourceManager::backBuffer->GetmemDC(), 0, 0, SRCCOPY);

	char titleName[7] = "Circus";
	TitleFont.Draw(titleName, 250, 200);

	BitBlt(hdc, 0, 0, 1024, 720, hdc, 0, 0, SRCCOPY);
}
void GameManager::EditModeDraw(HDC hdc)
{
	BitBlt(hdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, ResourceManager::backBuffer->GetmemDC(), 0, 0, SRCCOPY);
	ResourceManager::GetInstance()->Draw(hdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, IMAGENUM_BLACKBACKGROUND);
	for (int i = 0; i < 14; i++)
	{
		blocks[i].Draw(hdc, 650, i * 40);
	}
	
	for (int i = 0; i < TILE_HEIGHT_NUM; i++)
	{
		for (int j = 0; j < TILE_WIDTH_NUM; j++)
		{
			switch (mapTile[i * TILE_HEIGHT_NUM + j])
			{
			case 0:
				blocks[1].Draw(hdc, j * TileImageSizeX + GameOffsetX, i *TileImageSizeY + GameOffsetY);
				break;
			case 1:
				
				break;
			case 2:
				break;
			case 3:
				break;
			case 4:
				break;
			case 5:
				break;
			case 6:
				break;
			case 7:
				break;
			case 8:
				break;
			case 9:
				break;
			case 10:
				break;
			case 11:
				break;
			case 12:
				break;
			case 13:
				break;
			case 14:
				break;
			case 15:
				break;
			}
		}
	}
	player.EditDraw(ResourceManager::backBuffer->GetmemDC(), GameOffsetX, GameOffsetY);
}
void GameManager::DrawStagelogo(HDC hdc)
{
}

void GameManager::GameOver()
{
	isGameOver = true;
	//player.SetDie();
}
void GameManager::GameStart()
{
	DrawBlack();
	scene = EDIT_MODE;
}
void GameManager::Restart()
{
	isGameOver = false;
	GameOverflag = false;
	//player.Reset();
}
bool GameManager::CollisionCheck()
{
	RECT rcTemp;

	/*
	for (auto it = lEnemyFires.begin(); it != lEnemyFires.end(); it++)
	{
		if (Physics::GetInstance()->RECTbyRECTCollisionCheck(player.GetPlayerRect(), it->GetFireRect()))
		{
			return true;
		}
	}*/
	return false;
}
void GameManager::CollisionView()
{
	/*
	Rectangle(ResourceManager::backBuffer->GetmemDC(), player.GetPlayerRect().left, player.GetPlayerRect().top,
		player.GetPlayerRect().right, player.GetPlayerRect().bottom);
	for (auto it = lEnemyFires.begin(); it != lEnemyFires.end(); it++)
	{
		Rectangle(ResourceManager::backBuffer->GetmemDC(), it->GetFireRect().left, it->GetFireRect().top, it->GetFireRect().right, it->GetFireRect().bottom);
	}
	*/
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