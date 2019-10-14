#include "Enemy.h"
#include "EnemyManager.h"
#include "GameManager.h"

void EnemyManager::Init()
{
	spawnPos[0].x = GameManager::GetInstance()->GameOffsetX;
	spawnPos[0].y = GameManager::GetInstance()->GameOffsetY;
	spawnPos[1].x = TILEBLOCK_SIZE * 6 + GameManager::GetInstance()->GameOffsetX;
	spawnPos[1].y = GameManager::GetInstance()->GameOffsetY;
	spawnPos[2].x = TILEBLOCK_SIZE * 12 + GameManager::GetInstance()->GameOffsetX;
	spawnPos[2].y = GameManager::GetInstance()->GameOffsetY;
}

void EnemyManager::SpawnEnemy()
{
	Enemy *enemy = new Enemy();
	enemy->Init(spawnPos[spawnPosIndex].x, spawnPos[spawnPosIndex].y);
	spawnPosIndex++;
	if (spawnPosIndex == sizeof(spawnPos) / sizeof(POINT))
		spawnPosIndex = 0;

	enemylist.push_back(enemy);
}

void EnemyManager::DrawEnemy(HDC hdc)
{
	if (enemylist.size() == 0)
		return;
	for (auto it = enemylist.begin(); it != enemylist.end(); it++)
	{
		(*it)->Draw(hdc);
	}
}

void EnemyManager::Release()
{
	for (auto it = enemylist.begin(); it != enemylist.end();)
	{
		it = enemylist.erase(it);
	}
}

EnemyManager::EnemyManager(){}
EnemyManager::~EnemyManager(){}
