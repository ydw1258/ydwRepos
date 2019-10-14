#pragma once
#include<list>
#include<Windows.h>
using namespace std;

#define SPAWN_DELAY 5

class Enemy;
class EnemyManager
{
private:
	POINT spawnPos[3];
	list<Enemy*> enemylist;
	int spawnPosIndex = 0;
public:
	
	void Init();
	void SpawnEnemy();
	void DrawEnemy(HDC hdc);
	void Release();
	EnemyManager();
	~EnemyManager();
};