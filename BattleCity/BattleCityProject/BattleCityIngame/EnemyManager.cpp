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
void EnemyManager::Draw(HDC hdc)
{
	DrawEnemy(hdc);
	DrawBullets(hdc);
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

void EnemyManager::Update(float deltaTime)
{
	for (auto it = enemylist.begin(); it != enemylist.end(); it++)
	{
		(*it)->Update(deltaTime);
		if ((*it)->ShotTimer(deltaTime))
			Shot((*it)->x, (*it)->y, (*it)->direction);

		//맵의 요소들과 충돌 확인
		if (GameManager::GetInstance()->CollisionCheck((*it)->x, (*it)->y, (*it)->direction))
		{
			(*it)->DirectionChange();
		}
		(*it)->Move(deltaTime);
	}
	MoveBullets(deltaTime);
}

void EnemyManager::DrawBullets(HDC hdc)
{
	for (list<Bullet>::iterator it = enemybullets.begin(); it != enemybullets.end(); it++)
	{
		it->Draw(hdc);
	}
}
void EnemyManager::MoveBullets(float deltaTime)
{
	for (list<Bullet>::iterator it = enemybullets.begin(); it != enemybullets.end();)
	{
		it->Move(deltaTime);
		if (it->OutofRange())
			it = enemybullets.erase(it);
		else
			it++;
	}
}
void EnemyManager::Shot(int x, int y, DIRECTION direction)
{
	if (enemybullets.size() == 0)
	{
		Bullet bullet;
		bullet.isPlayers = false;
		bullet.Init(x, y, direction);
		enemybullets.push_back(bullet);
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
