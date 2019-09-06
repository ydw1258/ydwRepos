#pragma once
#include<Windows.h>
#include"EnemyFire.h"
#include"Player.h"
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
	Player player;
	static GameManager* mthis;
	float Timer[10] = { 0 };
	float TimerReset[10] = { 0 };
public:
	bool isGameOver = false;
	Scene scene;
	static GameManager* GetInstance()
	{
		if(mthis == nullptr)
			mthis = new GameManager();
		return mthis;
	}
	int distance = 0;

	void Restart();
	void MakeEnemyFire(FIRETYPE EnemyFireType);
	void MakeEnemyFutonFire();
	void MoveEnemyFire(int moveSpeed);
	void MoveBackground(int scrollSpeed);
	void Draw(HDC hdc);
	void DrawBackground(HDC hdc);
	void DrawFire(HDC hdc);
	bool CollisionCheck(RECT rc, Player& player);
	void GameOver(Player& player);
	void Update(); //배경움직이고 등등
	void Init(HDC hdc, HINSTANCE _g_hInst);
	void SetTimers();
	GameManager();
	~GameManager();
};

//SingletonTest::GetInstance()->Func(); //함수 호출시