#include "GameManager.h"
#include<iostream>
using namespace std;

GameManager* GameManager::mthis = nullptr;
enum TIMERID
{
	TIMER_TITLESPRITECHAGE,
	TIMER_MAKEFIRE,
	TIMER_SPRITECHANGE
};
void GameManager::Init(HDC hdc, HINSTANCE _g_hInst)
{
	for (int i = 0; i < 10; i++)
		Timer[i] = 0;
	player.Init();
	TitleFont.Init();
	ResourceManager::GetInstance()->Init(hdc, _g_hInst);
	MakeEnemyFire(NORMAL);
	SetTimers();
	backGround[0].Init(IMAGENUM_BACK, 1, 67, 183);
	backGround[1].Init(IMAGENUM_BACKNORMAL, 1, 65, 64);
	backElephant.Init(IMAGENUM_BACKDECO, 1, 66, 67);
	star.Init(IMAGENUM_STAR, 3, 14, 12);
	miter.Init();
}
void GameManager::SetTimers()
{
	TimerReset[TIMER_MAKEFIRE] = 5.0f;
	TimerReset[TIMER_SPRITECHANGE] = 0.4f;
	TimerReset[TIMER_TITLESPRITECHAGE] = 0.5f;
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
	case STAGE1:
		InGameUpdate();
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
		star.SpriteChange();
		
	}
}
void GameManager::InGameUpdate()
{
	static bool flag = false;

	if (CollisionCheck() && !flag)
	{
		flag = true;
		player.SetDie();
	}
	player.Jump();

	for (int i = 0; i < 10; i++)
	{
		Timer[i] += Physics::GetInstance()->deltaTime;

		if (Timer[i] >= TimerReset[i])
		{
			Timer[i] = 0;
			switch (i)
			{
			case TIMER_MAKEFIRE:
				GameManager::GetInstance()->MakeEnemyFire((FIRETYPE)(NORMAL + rand() % 3));
				break;
			case TIMER_SPRITECHANGE:
				player.SpriteChange();
				break;
			}
		}
	}
}

//Draw 매프레임 호출
void GameManager::Draw(HDC hdc)
{
	switch (scene)
	{
	case TITLE:
		DrawTitle(hdc);
		return;
	case STAGE1:
		InGameDraw(hdc);
		break;
	default:
		break;
	}
}
void GameManager::DrawTitle(HDC hdc)
{
	BitBlt(hdc, 0, 0, 1024, 720, ResourceManager::backBuffer->GetmemDC(), 0, 0, SRCCOPY);
	for(int i = 0; i < 30; i++)
		star.DrawObject(hdc, 200 + i*20, 170);
	char titleName[7] = "Circus"; 
	TitleFont.Draw(titleName, 250, 200);

	BitBlt(hdc, 0, 0, 1024, 720, hdc, 0, 0, SRCCOPY);
}
void GameManager::InGameDraw(HDC hdc)
{
	switch (player.playerState)
	{
	case STOP:
		MoveEnemyFire(0);
		// 안움직일때
		backGround[0].DrawBackground(ResourceManager::backBuffer->GetmemDC(), 0, 535, 20, 1);
		backGround[1].DrawBackground(ResourceManager::backBuffer->GetmemDC(), 0, 470, 20, 1);
		break;
	case LEFT:
		backGround[0].DrawBackground(ResourceManager::backBuffer->GetmemDC(), 0, 535, 20, 1);
		backGround[1].DrawBackground(ResourceManager::backBuffer->GetmemDC(), 0, 470, 20, 1);
		backElephant.DrawMoveBackground(ResourceManager::backBuffer->GetmemDC(), 970, 470, 0);
		miter.Draw(distance, 970, 670, 0);
		CameraX--;
		MoveEnemyFire(0);
		break;
	case RIGHT:
		backGround[0].DrawBackground(ResourceManager::backBuffer->GetmemDC(), 0, 535, 20, 1);
		backGround[1].DrawBackground(ResourceManager::backBuffer->GetmemDC(), 0, 470, 20, 1);
		backElephant.DrawMoveBackground(ResourceManager::backBuffer->GetmemDC(), 970, 470, 0);
		CameraX++;
		miter.Draw(distance, 970, 670, 0);
		MoveEnemyFire(0);
		
		break;
	}
	backElephant.DrawMoveBackground(ResourceManager::backBuffer->GetmemDC(), 970, 470, 0);
	miter.Draw(distance, 970, 670);
		
	DrawFire(ResourceManager::backBuffer->GetmemDC());
	player.Draw(ResourceManager::backBuffer->GetmemDC());

	//AddFontResourceA("godoMaum.ttf");
	CollisionView();
	BitBlt(hdc, 0, 0, 1024, 720, ResourceManager::backBuffer->GetmemDC(), 0, 0, SRCCOPY);
	//Render로 교체
	if (player.playerState == PLAYER_DIE)
		isGameOver = true;
}
void GameManager::DrawStagelogo(HDC hdc)
{
}

void GameManager::DrawFire(HDC hdc)
{
	for (auto it = lEnemyFires.begin(); it != lEnemyFires.end(); it++)
	{
		it->Draw(hdc, player.playerState);
	}
}

void GameManager::GameOver()
{
	isGameOver = true;
	player.SetDie();
}
void GameManager::GameStart()
{
	DrawBlack();
	scene = STAGE1;
}
void GameManager::MakeEnemyFire(FIRETYPE EnemyFireType)
{
	EnemyFire enemyFire;
	switch (EnemyFireType)
	{
	case NORMAL:
	case SMALL:
		enemyFire.Init(EnemyFireType, 1000, 565, 1);
		break;
	case FUTON:
		enemyFire.Init(EnemyFireType, 1000, 660, 1);
		break;
	default:
		break;
	}
	
	lEnemyFires.push_back(enemyFire);
}

void GameManager::MoveEnemyFire(int moveSpeed) //default == 0
{
	for (auto it = lEnemyFires.begin(); it != lEnemyFires.end();)
	{
		it->Move(moveSpeed);
		if (it->OutOfScreen())
		{
			it = lEnemyFires.erase(it);
		}
		else
			it++;
	}
}

bool GameManager::CollisionCheck()
{
	RECT rcTemp;
		
	for (auto it = lEnemyFires.begin(); it != lEnemyFires.end(); it++)
	{
		if (Physics::GetInstance()->RECTbyRECTCollisionCheck(player.GetPlayerRect(), it->GetFireRect()))
		{
			return true;
		}
	}
	return false;
}
void GameManager::CollisionView()
{
	Rectangle(ResourceManager::backBuffer->GetmemDC(), player.GetPlayerRect().left, player.GetPlayerRect().top, 
													player.GetPlayerRect().right, player.GetPlayerRect().bottom);
	for (auto it = lEnemyFires.begin(); it != lEnemyFires.end(); it++)
	{
		Rectangle(ResourceManager::backBuffer->GetmemDC(), it->GetFireRect().left, it->GetFireRect().top, it->GetFireRect().right, it->GetFireRect().bottom);
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
GameManager::GameManager(){}
GameManager::~GameManager(){}