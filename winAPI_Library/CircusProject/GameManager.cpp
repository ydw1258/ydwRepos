#include "GameManager.h"
#include "ResourceManager.h"
#include<iostream>
using namespace std;

GameManager* GameManager::mthis = nullptr;

void GameManager::DrawBackground(HDC hdc)
{
	//SpriteManager로 교체
	for (int i = -5; i < 20; i++)
	{

		//녹색 배경
		ResourceManager::GetInstance()->Draw(hdc, i * 67, 535, 67, 183, IMAGENUM_BACK);
		//관객
		ResourceManager::GetInstance()->Draw(hdc, i * 64 + backgroundOffsetX[0], 470, 65, 64, BACKNORMAL);
	}
	ResourceManager::GetInstance()->Draw(hdc, 1100 + backgroundOffsetX[1], 470, 66, 67, IMAGENUM_BACKDECO);
}

void GameManager::DrawFire(HDC hdc)
{
	for (auto it = lEnemyFires.begin(); it != lEnemyFires.end(); it++)
	{
		it->Draw(hdc);
	}
}
bool GameManager::CollisionCheck(RECT rc, Player& player)
{
	RECT rcTemp;

	for (auto it = lEnemyFires.begin(); it != lEnemyFires.end(); it++)
	{
		if (Physics::GetInstance()->RECTbyRECTCollisionCheck(rc, it->FireRect))
		{
			GameOver(player);
			return true;
		}
	}
}
void GameManager::GameOver(Player& player)
{
	isGameOver = true;
	player.SetDie();
}
void GameManager::Init(HDC hdc, HINSTANCE _g_hInst)
{
	for (int i = 0; i < 10; i++)
		Timer[i] = 0;
	ResourceManager::GetInstance()->Init(hdc, _g_hInst);
	GetInstance()->MakeEnemyFire(NORMAL);
	Physics::GetInstance()->deltaTimeInit();
	SetTimers();
}
void GameManager::SetTimers()
{
	TimerReset[0] = 0.3f;
	TimerReset[1] = 3.0f;
}
void GameManager::Update()
{
	for (int i = 0; i < 10; i++)
	{
		Timer[i] += Physics::GetInstance()->deltaTime;
	}
	//각 타이머때 마다 해주어야할 로직 작성
	if (Timer[0] >= TimerReset[0])
	{
		cout << "억" << endl;
		Timer[0] = 0;
	}
	cout << Timer[0] << endl;
	if (Timer[1] >= TimerReset[1])
	{
		cout << "악" << endl;
		Timer[0] = 0;
	}
	cout << Timer[1] << endl;
	/*
	GameManager::GetInstance()->MoveEnemyFire(0);
	GameManager::GetInstance()->MakeEnemyFire((FIRETYPE)(NORMAL + rand() % 3));
	player.Move(false);
	GameManager::GetInstance()->MoveBackground(2);
	GameManager::GetInstance()->MoveBackground(-2);
	*/

	//죽은경우 타이머 재설정..
	//GameManager::GetInstance()->Restart();
}

void GameManager::MakeEnemyFire(FIRETYPE EnemyFireType)
{
	EnemyFire enemyFire;
	enemyFire.Init(EnemyFireType);
	
	lEnemyFires.push_back(enemyFire);
}

void GameManager::MakeEnemyFutonFire()
{
	EnemyFire enemyFire;
	enemyFire.Init(FUTON);
	lEnemyFires.push_back(enemyFire);
}


void GameManager::MoveEnemyFire(int moveSpeed)//default == 0
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

void GameManager::MoveBackground(int scrollSpeed)
{
	cout << backgroundOffsetX[0] << endl;
	for(int i =0;i < 10; i++)
		backgroundOffsetX[i] += scrollSpeed;
	if (backgroundOffsetX[0] == -64)
		backgroundOffsetX[0] = 0;
	if(backgroundOffsetX[1] == -3000)
		backgroundOffsetX[1] = 0;
}
void GameManager::Restart()
{
	exit(1);
}

void GameManager::Draw(HDC hdc)
{
	DrawBackground(ResourceManager::backBuffer->GetmemDC());
	DrawFire(ResourceManager::backBuffer->GetmemDC());
	player.Draw(ResourceManager::backBuffer->GetmemDC());
	BitBlt(hdc, 0, 0, 1024, 720, ResourceManager::backBuffer->GetmemDC(), 0, 0, SRCCOPY);
	
}
GameManager::GameManager()
{
}
GameManager::~GameManager()
{
}