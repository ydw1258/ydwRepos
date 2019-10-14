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
#include "IMAGENUM.h"
#include "EnemyManager.h"

using namespace std;

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
enum JUDGEBLOCKTYPE // 1/4 짜리 타일
{
	JUDGE_EMPTY,
	JUDGE_BRICK,
	JUDGE_BLOCK,
	JUDGE_WATER,
	JUDGE_BUSH,
	JUDGE_STEEL
};

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 600

#define TILE_WIDTH_NUM 13
#define TILE_HEIGHT_NUM 13

#define SMALL_TILE_WIDTH_NUM 26
#define SMALL_TILE_HEIGHT_NUM 26

#define MINIBLOCK_SIZE 16
#define TILEBLOCK_SIZE 32

using namespace std;

enum Scene
{
	TITLE,
	INGAME,
	RESULT
};

struct MapStackInfo
{
	POINT pt;
	BLOCKTYPE mapValue;
};
struct RGBInfo
{
	int r;
	int b;
	int g;
};
class GameManager
{
private:
	Player player;
	POINT startPos;
	int score;
	int remains; //남은 플레이어 기회
	HWND hwnd;
	SpriteRenderer backGround;
	SpriteRenderer blocks[15];
	SpriteRenderer TitleLogo;
	SpriteRenderer flash;
	EnemyManager enemyManager;

	RECT greyScreen[2]; //위아래 스크린
	static GameManager* mthis;
	BitMap backBlackImage;
	float Timer[10] = { 0 };
	float TimerReset[10] = { 0 };
	bool GameOverflag = false;
	SpriteRenderer blockSprites[11];
	stack<MapStackInfo> mapTileUndoStack;
	stack<MapStackInfo> mapTileRedoStack;
	BLOCKTYPE mapTile[TILE_HEIGHT_NUM * TILE_WIDTH_NUM] = { EMPTY };
	JUDGEBLOCKTYPE judgeMapTile[SMALL_TILE_HEIGHT_NUM * SMALL_TILE_WIDTH_NUM];
	list<Bullet> bullets;
	
	bool isScreenScroll = true;
	float logoOffsetY = 600.0f;
public:
	const int GameOffsetX = 20;
	const int GameOffsetY = 20;
	const int TileImageSizeX = 32;
	const int TileImageSizeY = 32;
	bool isGameOver = false;
	bool isScreenMoving = false;
	Scene scene = INGAME;

	static GameManager* GetInstance()
	{
		if (mthis == nullptr)
			mthis = new GameManager();
		return mthis;
	}
	void Draw(HDC hdc);
	void DrawTitle(HDC hdc);
	void DrawIngame(HDC hdc);
	void DrawGrey(HDC hdc);
	void DrawScreen(HDC hdc);
	void DrawBlack(HDC hdc);
	void DrawTile(HDC hdc);
	void DrawGameInfo(HDC hdc);
	bool CollisionCheck();
	void CollisionDraw(HDC hdc);

	void Init(HDC hdc, HINSTANCE _g_hInst, HWND _hwnd);
	void Release();

	void Shot();
	void LoadMap();
	void PlayerMove(DIRECTION dir, float deltaTime);
	void MoveBullets();
	void DrawBullets(HDC hdc);
	void ResourcesInit();
	void SpawnEnemy(float deltaTime);
	void Update(float deltaTime);
	void ScreenScroll(float deltaTime);

	void SceneChange(Scene _scene, float delayTime);

	GameManager();
	~GameManager();
};