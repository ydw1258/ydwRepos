#pragma once
#include<Windows.h>
#include<string>
#include<list>
#include<vector>
#include<stack>
#include<iostream>
#include<fstream>
#include"SpriteRenderer.h"
#include"Tile.h"
#include"FontManager.h"
#include "Physics.h"
#include "Player.h"
#include "Bullet.h"

enum BLOCKTYPE
{
	EMPTY,
	BLICK_RIGHT,
	BLICK_DOWN,
	BLICK_LEFT,
	BLICK_UP,
	BLICK_FULL,
	IRON_RIGHT,
	IRON_DOWN,
	IRON_LEFT,
	IRON_UP,
	IRON_FULL,
	WATER,
	BUSH,
	ICE,
	BLOCKEGLE
};
enum JUDGEBLOCKTYPE
{
	JUDGE_EMPTY,
	JUDGE_BRICK,
	JUDGE_BLOCK,
	JUDGE_WATER,
	JUDGE_BUSH,
	JUDGE_STEEL
};

//#include "BlockType.h"

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 600

#define TILE_WIDTH_NUM 13
#define TILE_HEIGHT_NUM 13

#define SMALL_TILE_WIDTH_NUM 26
#define SMALL_TILE_HEIGHT_NUM 26

using namespace std;

enum Scene
{
	TITLE,
	EDIT_MODE,
	INGAME
};

struct MapStackInfo
{
	POINT pt;
	BLOCKTYPE mapValue;
};
class GameManager
{
private:
	POINT startPos;
	int score;
	int remains; //남은 플레이어 기회

	SpriteRenderer backGround;
	SpriteRenderer grayBackGround;

	Tile blocks[15];
	static GameManager* mthis;
	//FontManager TitleFont;
	bool GameOverflag = false;
	
	JUDGEBLOCKTYPE judgeMapTile[SMALL_TILE_HEIGHT_NUM * SMALL_TILE_WIDTH_NUM];
	list<Bullet> bullets;
	list<Bullet> enemyBullets;
	
public:
	const int GameOffsetX = 20;
	const int GameOffsetY = 20;
	const int TileImageSizeX = 32;
	const int TileImageSizeY = 32;
	bool isGameOver = false;
	bool isCollisionViewOn = false;
	//Scene scene = TITLE;
	Scene scene = EDIT_MODE;

	static GameManager* GetInstance()
	{
		if (mthis == nullptr)
			mthis = new GameManager();
		return mthis;
	}
	void GameStart();
	void Draw(HDC hdSc);
	void DrawTitle(HDC hdc);
	void DrawStagelogo(HDC hdc);
	void DrawBlack();
	void DrawTile(HDC hdc, JUDGEBLOCKTYPE blockType, int x, int y);
	bool CollisionCheck(Player& player);
	void CollisionCheckBullet();
	void CollisionBlockandBullet(RECT bulletRect, int colisionBlockX, int colisionBlockY);
	void CollisionDraw(Player player, HDC hdc);
	void Init(HDC hdc, HINSTANCE _g_hInst);
	
	void Shot(Player player);
	void LoadMap(int* mapValues);

	void MoveBullets();
	void DrawBullets(HDC hdc);

	GameManager();
	~GameManager();
};
