#include "GameManager.h"
#include "ResourceManager.h"
#include<iostream>
using namespace std;

GameManager* GameManager::mthis = nullptr;

void GameManager::DrawBackground(HDC hdc)
{
	for (int i = -5; i < 20; i++)
	{
		//³ì»ö ¹è°æ
		ResourceManager::GetInstance()->Draw(hdc, i*67, 535, 67, 183, IMAGENUM_BACK);
		//°ü°´
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
		if (IntersectRect(&rcTemp, &rc, &it->FireRect))
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

GameManager::GameManager()
{
}
GameManager::~GameManager()
{
}