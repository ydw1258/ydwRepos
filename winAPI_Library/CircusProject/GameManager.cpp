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
	player.Init(3); // playerSpeed
	TitleFont.Init();
	ResourceManager::GetInstance()->Init(hdc, _g_hInst);
	MakeEnemyFire(NORMAL);
	SetTimers();
	backGround[0].Init(IMAGENUM_BACK, 1, 67, 183);
	backGround[1].Init(IMAGENUM_BACKNORMAL, 1, 65, 64);
	backElephant.Init(IMAGENUM_BACKDECO, 1, 66, 67, 970);
	star.Init(IMAGENUM_STAR, 3, 14, 12);
	MiterInit();
	goal.Init(5000, 650);
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
	if (isGameOver)
	{
		Timer[TIMER_GAMEOVERTIMER] += Physics::GetInstance()->deltaTime;
		if (Timer[TIMER_GAMEOVERTIMER] >= TimerReset[TIMER_GAMEOVERTIMER])
		{
			Timer[TIMER_GAMEOVERTIMER] = 0;
			Restart();
			
		}
		return;
	}

	if (CollisionCheck() && !GameOverflag)
	{
		player.SetDie();
		GameOverflag = true;
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
	if (isGameOver)
		return;
	switch (player.playerState)
	{
	case STOP:
		MoveEnemyFire(0);
		// 안움직일때
		backGround[0].DrawSrolledBackground(ResourceManager::backBuffer->GetmemDC(), 0, 535, 20, 1);
		backGround[1].DrawSrolledBackground(ResourceManager::backBuffer->GetmemDC(), 0, 470, 20, 1);
		break;
	case LEFT:
		backGround[0].DrawSrolledBackground(ResourceManager::backBuffer->GetmemDC(), 0, 535, 20, 1);
		backGround[1].DrawSrolledBackground(ResourceManager::backBuffer->GetmemDC(), 0, 470, 20, 1);
		backElephant.DrawMoveBackground(ResourceManager::backBuffer->GetmemDC(), 1100, 470);
		MoveEnemyFire(0);
		if (CameraX >= 0)
			CameraX -= player.playerSpeed;
		break;
	case RIGHT:
		backGround[0].DrawSrolledBackground(ResourceManager::backBuffer->GetmemDC(), 0, 535, 20, 1);
		backGround[1].DrawSrolledBackground(ResourceManager::backBuffer->GetmemDC(), 0, 470, 20, 1);
		backElephant.DrawMoveBackground(ResourceManager::backBuffer->GetmemDC(), 1100, 470);
		MoveEnemyFire(0);
		CameraX += player.playerSpeed;
		break;
	}
	backGround[0].DrawSrolledBackground(ResourceManager::backBuffer->GetmemDC(), 0, 535, 20, 1);
	backGround[1].DrawSrolledBackground(ResourceManager::backBuffer->GetmemDC(), 0, 470, 20, 1);
	backElephant.DrawMoveBackground(ResourceManager::backBuffer->GetmemDC(), 1100, 470);
	//miter.Draw(distance, 970, 670);
	MiterDraw();
	DrawFire(ResourceManager::backBuffer->GetmemDC());
	player.Draw(ResourceManager::backBuffer->GetmemDC());
	goal.Draw(ResourceManager::backBuffer->GetmemDC());
	//AddFontResourceA("godoMaum.ttf");
	CollisionView();
	BitBlt(hdc, 0, 0, 1024, 720, ResourceManager::backBuffer->GetmemDC(), 0, 0, SRCCOPY);
	//Render로 교체
	if (GameOverflag == true)
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
void GameManager::Restart()
{
	isGameOver = false;
	GameOverflag = false;
	player.Reset();
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
void GameManager::MiterInit()
{
	MiterBoard miter[9];

	for (int i = 0; i < 9; i++)
	{
		miter[i].Init();
		miters.push_back(miter[i]);
	}
	
}
void GameManager::MiterDraw()
{
	int x = 500, y = 670;
	int distance = 90;

	int i = 1;

	for (auto it = miters.begin(); it != miters.end(); it++)
	{
		it->Draw(distance, x * i, y, 0, 0);
		i++;
		distance -= 10;
	}
	
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