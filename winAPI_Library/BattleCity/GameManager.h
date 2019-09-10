#pragma once
#include<Windows.h>
#include "ResourceManager.h"
#include"SpriteRenderer.h"
#include"Player.h"
#include"FontManager.h"
#include"Physics.h"
#include<list>
#include<vector>
#include"Tile.h"

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 600

#define TILE_WIDTH_NUM 13
#define TILE_HEIGHT_NUM 13

using namespace std;

enum Scene
{
	TITLE,
	EDIT_MODE,
	INGAME
};

class GameManager
{
private:
	int score;
	int remains; //남은 플레이어 기회

	SpriteRenderer backGround;
	Tile blocks[15];

	static GameManager* mthis;
	FontManager TitleFont;
	float Timer[10] = { 0 };
	float TimerReset[10] = { 0 };
	bool GameOverflag = false;
	const int GameOffsetX = 20;
	const int GameOffsetY = 20;
	int mapTile[TILE_HEIGHT_NUM * TILE_WIDTH_NUM] = { 0 };
	int TileImageSizeX = 33;
	int TileImageSizeY = 25;
public:
	Player player;
	bool isGameOver = false;
	//Scene scene = TITLE;
	Scene scene = EDIT_MODE;
	static GameManager* GetInstance()
	{
		if (mthis == nullptr)
			mthis = new GameManager();
		return mthis;
	}
	void GameStart();
	void Restart();
	void Draw(HDC hdc);
	void DrawTitle(HDC hdc);
	void DrawStagelogo(HDC hdc);
	void DrawBlack();
	void EditModeDraw(HDC hdc);
	bool CollisionCheck();
	void CollisionView();
	void GameOver();
	void Update(); //배경움직이고 등등
	void EditModeUpdate();
	void InTitleUpdate();
	void Init(HDC hdc, HINSTANCE _g_hInst);
	void Input(WPARAM wParam);
	void SetTimers();
	GameManager();
	~GameManager();
};
