#pragma once
#include<Windows.h>
#include"EnemyFire.h"
#include "ResourceManager.h"
#include"Player.h"
#include "MiterBoard.h"
#include<list>
#include<vector>
using namespace std;

enum Scene
{
	TITLE,
	STAGE1
};

class GameManager
{
private:
	int score;
	int remains; //남은 플레이어 기회
	int backgroundOffsetX[10] = { 0 };
	list<EnemyFire> lEnemyFires;
	SpriteRenderer backGround[2];
	SpriteRenderer backElephant;
	MiterBoard _miter;
	static GameManager* mthis;
	float Timer[10] = { 0 };
	float TimerReset[10] = { 0 };
	int distance = 90;
public:
	Player player;
	bool isGameOver = false;
	Scene scene;
	static GameManager* GetInstance()
	{
		if(mthis == nullptr)
			mthis = new GameManager();
		return mthis;
	}

	void Restart();
	void MakeEnemyFire(FIRETYPE EnemyFireType);
	void MoveEnemyFire(int moveSpeed);
	void Draw(HDC hdc);
	void DrawFire(HDC hdc);
	bool CollisionCheck();
	void CollisionView();
	void GameOver();
	void Update(); //배경움직이고 등등
	void Init(HDC hdc, HINSTANCE _g_hInst);
	void SetTimers();
	GameManager();
	~GameManager();
};